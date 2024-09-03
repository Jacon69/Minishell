/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexigar <alexigar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 09:26:57 by alexigar          #+#    #+#             */
/*   Updated: 2024/09/03 18:21:28 by alexigar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_piped(t_command **list, int j, t_command **command)
{
	int	pipefd[2];

	if (j > 0 && (list[j - 1])-> piped && !(list[j - 1]-> redir2))
	{
		if (pipe(pipefd) == 0)
		{
			(list[j - 1])-> file_output = pipefd[1];
			(*command)-> file_input = pipefd[0];
		}
		else
			return (0);
	}
	return (1);
}

void	check_pipe_token(char *token, t_command **command, int *i)
{
	if (token[0] == '|')
	{
		(*command)-> piped = 1;
		*i += 1;
	}
}
