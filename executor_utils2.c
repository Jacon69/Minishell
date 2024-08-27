/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexigar <alexigar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 12:05:27 by alexigar          #+#    #+#             */
/*   Updated: 2024/08/26 12:05:50 by alexigar         ###   ########.fr       */
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
