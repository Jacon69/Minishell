/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jconde-a <jconde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 12:05:27 by alexigar          #+#    #+#             */
/*   Updated: 2024/09/04 15:15:30 by jconde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_executor_aux(t_command **command_list,
	t_list **env, char	**paths, int i)
{
	if (is_built_in(command_list[i]-> command))
		return (ft_build_int(command_list[i], env));
	else
		return (try_call(paths, command_list[i], env));
}

void	ft_print_up(int i)
{
	(void ) i;
	errno = 2;
	perror("Error");
}

int	ft_1executor(t_command **command_list, char **paths, int i)
{
	if (!command_list[i]-> command)
	{
		ft_free_char(paths);
		return (0);
	}
	return (1);
}
