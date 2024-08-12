/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexigar <alexigar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 10:01:14 by alexigar          #+#    #+#             */
/*   Updated: 2024/08/12 12:40:41 by alexigar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	**ft_aux1_parser(char **tokens, int *i,
	t_command **current_command, t_command **list)
{
	while ((tokens[*i][0] == '>') && (*i < count_nbr_tokens(tokens)))
	{
		put_redir2(current_command, tokens[*i]);
		right_redir(tokens, i, current_command);
		*i += 1;
		if (*i >= count_nbr_tokens(tokens))
			return (current_command);
	}
	while ((tokens[*i][0] == '<') && (*i < count_nbr_tokens(tokens)))
	{
		put_redir1(current_command, tokens[*i]);
		*current_command = left_redir(tokens, i, current_command);
		*i += 1;
		if (!*current_command)
			return (free_commands(list));
		if (*i >= count_nbr_tokens(tokens))
			return (current_command);
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
	if (!current_command || !check_piped(list, dup[0], &current_command))
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
		{
			aux = end_list(current_command, list, dup);
			return (aux);
		}
		check_pipe_token(tokens[dup[2]], &current_command, &dup[2]);
	}
	return (aux);
}

char	**free_and_nl(char **tokens)
{
	char	**empty_tokens;
	int		i;

	i = 1;
	errno = 8;
	perror("Invalid tokens");
	empty_tokens = malloc(sizeof(char *) * count_nbr_tokens(tokens));
	if (!empty_tokens)
		return (NULL);
	empty_tokens[0] = malloc(5);
	if (!empty_tokens[0])
	{
		ft_free_char(empty_tokens);
		return (NULL);
	}
	empty_tokens[0][0] = '\n';
	empty_tokens[0][1] = '\0';
	while (i < count_nbr_tokens(tokens))
	{
		empty_tokens[i] = NULL;
		i++;
	}
	ft_free_char(tokens);
	return (empty_tokens);
}

void	check_directory(char *token)
{
	DIR	*dir;

	dir = opendir(token);
	if (dir)
	{
		perror("minishell");
		errno = EISDIR;
		closedir(dir);
	}
	else if (errno != ENOENT)
	{
		perror(token);
		errno = EACCES;
	}
}

char	**check_tokens(char **tokens)
{
	int	i;

	i = 0;
	if (count_nbr_tokens(tokens) == 1 && tokens[i][0] == '/')
	{
		check_directory(tokens[i]);
		return (free_and_nl(tokens));
	}
	while (i < count_nbr_tokens(tokens))
	{
		if (tokens[i][0] == '|' || tokens[i][0] == '>' || tokens[i][0] == '<')
		{
			if (++i == count_nbr_tokens(tokens))
				return (free_and_nl(tokens));
			else if (tokens[i][0] == tokens[i - 1][0])
				return (free_and_nl(tokens));
		}
		if (++i == count_nbr_tokens(tokens))
			break ;
	}
	return (tokens);
}

/*Recibe token y prepara los comando*/
t_command	**parser(char **tokens, t_list **env)
{
	int			dup[4];
	t_command	*current_command;
	t_command	**list;
	t_command	**aux;

	tokens = check_tokens(tokens);
	ft_ini_dup(dup);
	list = ft_init_com_list(tokens);
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
