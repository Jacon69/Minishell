/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_in_cd_aux.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexigar <alexigar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 18:55:17 by alexigar          #+#    #+#             */
/*   Updated: 2024/08/08 10:37:00 by alexigar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*están las distintas opciones según lo que viene en arg. 
desde ahí llamamos a funciones*/
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
		ok = ft_cd_dir(command, dir); //leak
	return (ok);
}

/*inicializo la estructura del dir actual que me ha llegado por command*/
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
