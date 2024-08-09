/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_in_cd_aux.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jconde-a <jconde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 18:55:17 by alexigar          #+#    #+#             */
/*   Updated: 2024/08/09 20:30:23 by jconde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Different options depending on what comes from arg, then call functions
int	ft_aux2_buil_cd(t_command *command, t_struct_path *dir)
{
	int		ok;
	char	*arg;

	arg = command -> args[1];
	ok = 0;
	if ((((!ft_memcmp(arg, "..", 2)
					&& ft_strlen(arg) == 2))
			|| (!ft_memcmp(arg, "../", 3)
				&& (ft_strlen(arg) == 3)))
		&& !(command -> args[2]))
		ok = ft_minus_dir(dir);
	else if (((((arg[0] == '.'
						&& ft_strlen(arg) == 1))
				|| (!ft_memcmp(command->args[1], "./", 2)
					&& ft_strlen(arg) == 2)))
		&& !(command -> args[2]))
		ok = ft_equal_dir(dir);
	else if (((arg[0] == '.') && (command -> args[2]))
		|| (((!ft_memcmp(arg, "./", 2)
					|| !ft_memcmp(arg, "../", 3)) && (command -> args[2]))))
		perror("Error cd: too many arguments");
	else if (ft_strlen(arg) > 0)
		ok = ft_cd_dir(command, dir);
	return (ok);
}

//Initialize structure of current directory coming from command
int	ft_aux1_buil_cd(t_command *command, t_struct_path *dir)
{
	int	i;

	dir->line_path = command->path;
	dir->route = malloc(2);
	if (!dir->route)
	{
		perror("Error built_in");
		return (-1);
	}
	dir->route[0] = '/';
	dir->route[1] = '\0';
	dir->num_dir = 0;
	i = 0;
	while (dir->line_path[i])
	{
		if (dir->line_path[i] == '/')
		{
			dir->num_dir++;
		}
		i++;
	}
	dir->path = ft_split(dir->line_path, '/');
	if (!dir->path)
		return (-1);
	return (0);
}
