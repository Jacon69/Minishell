/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexigar <alexigar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 10:01:14 by alexigar          #+#    #+#             */
/*   Updated: 2024/08/07 17:16:03 by alexigar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	**free_commands(t_command **command_list)
{
	int	i;

	i = 0;
	while (command_list[i])
	{
		if (command_list[i]-> args)
			free(command_list[i]-> args);
		if (command_list[i]-> file_input != 1)
			close(command_list[i]-> file_input);
		if (command_list[i]-> file_output != 1)
			close(command_list[i]-> file_output);
		if (command_list[i]-> path)
			free(command_list[i]-> path);
		if (command_list[i]-> string_output)
			free(command_list[i]-> string_output);
		free(command_list[i]);
		i++;
	}
	free(command_list);
	return (NULL);
}

t_command	*new_command(char **tokens, t_list **env, int i, int j)
{
	t_command	*current_command;
	int			n_tokens;

	n_tokens = count_nbr_tokens(tokens);
	current_command = malloc(sizeof(t_command));
	if (!current_command)
		return (NULL);
	current_command -> args = malloc(sizeof(char *) * (n_tokens - i + 1));
	current_command -> command = NULL;
	current_command -> string_output = NULL;
	current_command -> path = ft_get_var_env(env, "..PWD");
	if (!current_command -> path || !current_command -> args)
		return (NULL);
	current_command -> redir1 = 0;
	current_command -> redir2 = 0;
	current_command -> file_input = 1;
	current_command -> file_output = 1;
	current_command -> piped = 0;
	current_command -> index = j;
	dup2(STDIN_FILENO, current_command -> file_input);
	dup2(STDOUT_FILENO, current_command -> file_output);
	return (current_command);
}

void	write_in_heredoc(char *token, int *heredoc)
{
	char	*line;

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
			(*command)-> file_input = heredoc[0];
		}
		else
			return (NULL);
		*i += 1;
	}
	return (*command);
}

int	check_piped(t_command ***list, int j, t_command **command)
{
	int	pipefd[2];

	if (j > 0 && (*list[j - 1])-> piped)
	{
		if (pipe(pipefd) == 0)
		{
			(*list[j - 1])-> file_output = pipefd[1];
			(*command)-> file_input = pipefd[0];
		}
		else
			return (0);
	}
	return (1);
}

t_command	**end_list(t_command *command, t_command **list, int *dup)
{
	command -> args[dup[1]] = NULL;
	list[dup[0]] = command;
	list[++dup[0]] = NULL;
	dup[3] = 1;
	return (list);
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

void	check_pipe_token(char *token, t_command **command, int *i)
{
	if (token[0] == '|')
	{
		(*command)-> piped = 1;
		*i += 1;
	}
}

void	fill_args(char **tokens, int *i, t_command **com, int *k)
{
	while (tokens[*i] && tokens[*i][0] != '|'
		&& tokens[*i][0] != '>' && tokens[*i][0] != '<')
	{
		if (!((*com)-> command))
			(*com)-> command = tokens[*i];
		(*com)-> args[*k] = tokens[*i];
		*k += 1;
		(*com)-> args[*k] = NULL;
		*i += 1;
		if (!tokens[*i])
			break ;
	}
}

t_command	**ft_aux1_parser(char **tokens, int *i,
	t_command **current_command, t_command **list)
{
	while ((tokens[*i][0] == '>') && (++*i < count_nbr_tokens(tokens)))
	{
		put_redir2(current_command, tokens[*i]);
		right_redir(tokens, i, current_command);
	}
	while ((tokens[*i][0] == '<') && (++*i < count_nbr_tokens(tokens)))
	{
		put_redir1(current_command, tokens[*i]);
		*current_command = left_redir(tokens, i, current_command);
		if (!*current_command)
			return (free_commands(list));
	}
	return (current_command);
}

void	ft_ini_dup(int *dup)
{
	dup[0] = 0;
	dup[1] = 0;
	dup[2] = 0;
	dup[3] = 0;
}

void	ft_aux2_parser(t_command **list, t_command *current_command, int *dup)
{
	list[dup[0]] = current_command;
	dup[0]++;
	dup[1] = 0;
}

t_command	**ft_aux3_parser(t_command **list, t_command *current_command
			, int *dup, char **tokens )
{
	t_command	**aux;

	aux = NULL;
	if (!current_command || !check_piped(&list, dup[0], &current_command))
	{
		free_commands(list);
		dup[3] = 1;
	}
	fill_args(tokens, &dup[2], &current_command, &dup[1]);
	if (!tokens[dup[2]])
		aux = end_list(current_command, list, dup);
	else
	{
		if (!ft_aux1_parser(tokens, &dup[2], &current_command, list))
			dup[3] = 1;
		if (dup[2] >= count_nbr_tokens(tokens))
			aux = end_list(current_command, list, dup);
		check_pipe_token(tokens[dup[2]], &current_command, &dup[2]);
	}
	return (aux);
}

t_command	**parser(char **tokens, t_list **env)
{
	int			dup[4];
	t_command	*current_command;
	t_command	**list;
	t_command	**aux;

	ft_ini_dup(dup);
	list = malloc(sizeof(t_command *) * (count_nbr_tokens(tokens) + 1));
	if (!list)
		return (NULL);
	while (tokens[dup[2]])
	{
		current_command = new_command(tokens, env, dup[2], dup[0]);
		aux = ft_aux3_parser(list, current_command, dup, tokens);
		if (dup[3] == 1)
			return (aux);
		ft_aux2_parser(list, current_command, dup);
	}
	list[dup[0]] = NULL;
	return (list);
}
