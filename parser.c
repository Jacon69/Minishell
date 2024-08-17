/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaimecondea <jaimecondea@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 10:01:14 by alexigar          #+#    #+#             */
/*   Updated: 2024/08/17 09:57:28 by jaimecondea      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_aux4_parse(char **tokens, t_command **cmd, int *i)
{
	put_redir2(cmd, tokens[*i]);
	right_redir(tokens, i, cmd);
}

t_command	**aux1_par(char **tokens, int *i, t_command **cmd, t_command **list)
{
	int	token_count;

	token_count = count_nbr_tokens(tokens);
	while ((tokens[*i][0] == '>' || tokens[*i][0] == '<') && (*i < token_count))
	{
		if (tokens[*i][0] == '>')
			ft_aux4_parse(tokens, cmd, i);
		else
		{
			put_redir1(cmd, tokens[*i]);
			*cmd = left_redir(tokens, i, cmd);
			if ((*cmd)->file_input == -1)
			{
				(*cmd)->redir1 = -1;
				while (*i < token_count && tokens[*i][0] == '<')
					*i += 2;
			}
			if (!*cmd)
				return (free_commands(list));
		}
		*i += 1;
		if (*i >= token_count)
			break ;
	}
	return (cmd);
}

/*t_command	**ft_aux1_parser(char **tokens, int *i,
	t_command **current_command, t_command **list)
{
	while (((tokens[*i][0] == '>') || (tokens[*i][0] == '<'))
			&& (*i < count_nbr_tokens(tokens)))
	{
		if (tokens[*i][0] == '>')
		{
			put_redir2(current_command, tokens[*i]);
			right_redir(tokens, i, current_command);
			*i += 1;
			if (*i >= count_nbr_tokens(tokens))
				return (current_command);
		}
		else
		{
			put_redir1(current_command, tokens[*i]);
			*current_command = left_redir(tokens, i, current_command);
			if ((*current_command)-> file_input == -1)
			{
				(*current_command)-> redir1 = -1;
				while (*i < count_nbr_tokens(tokens) && tokens[*i][0] == '<')
					*i += 2;
			}
			*i += 1;
			if (!*current_command)
				return (free_commands(list));
			if (*i >= count_nbr_tokens(tokens))
				return (current_command);
		}
	}
	return (current_command);
}*/

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
		if (!aux1_par(tokens, &dup[2], &current_command, list))
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

/*Recibe token y prepara los comando*/
t_command	**parser(char **tokens, t_list **env, int *flag)
{
	int			dup[4];
	t_command	*current_command;
	t_command	**list;
	t_command	**aux;

	tokens = check_tokens(tokens, flag);
	if (*flag != 0)
		return (NULL);
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
