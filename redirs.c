/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexigar <alexigar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 09:26:44 by alexigar          #+#    #+#             */
/*   Updated: 2024/08/16 11:09:30 by alexigar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	write_in_heredoc(char *token, int *heredoc)
{
	char	*line;

	//g_is_executing = 2;
	line = readline(" heredoc> ");
	if (!line)
		return ;
	while (ft_strncmp(token, line, ft_strlen(token)))
	{
		write(heredoc[1], line, ft_strlen(line));
		write(heredoc[1], "\n", 1);
		free(line);
		line = readline(" heredoc> ");
		if (!line)
			break ;
	}
	close(heredoc[1]);
	free(line);
}

void	right_redir(char **tokens, int *i, t_command **command)
{
	while (tokens[*i] && tokens[*i][0] == '>')
	{
		*i += 1;
		if ((*command)-> file_output != 1)
			close((*command)-> file_output);
		if ((*command)-> redir2 == 1)
		{
			unlink(tokens[*i]);
			(*command)-> file_output = open(tokens[*i],
					O_WRONLY | O_CREAT, 0644);
		}
		else
		{
			(*command)-> file_output = open(tokens[*i],
					O_WRONLY | O_CREAT | O_APPEND, 0644);
		}
		*i += 1;
		if (*i >= count_nbr_tokens(tokens))
			return ;
	}
}

t_command	*left_redir(char **tokens, int *i, t_command **command)
{
	int	heredoc[2];

	*i += 1;
	if ((*command)-> redir1 == 1)
	{
		if ((*command)-> file_input != -1)
		{
			if ((*command)-> file_input != 1)
				close((*command)-> file_input);
			(*command)-> file_input = open(tokens[*i], O_RDONLY);
		}
	}
	else
	{
		if (pipe(heredoc) == 0)
		{
			write_in_heredoc(tokens[*i], heredoc);
			g_is_executing = 0;
			(*command)-> file_input = heredoc[0];
		}
		else
			return (NULL);
		*i += 1;
	}
	return (*command);
}

void	put_redir1(t_command **command, char *token)
{
	if (token[1] == '<')
		(*command)-> redir1 = 2;
	else
		(*command)-> redir1 = 1;
}

void	put_redir2(t_command **command, char *token)
{
	if (token[1] == '>')
		(*command)-> redir2 = 2;
	else
		(*command)-> redir2 = 1;
}
