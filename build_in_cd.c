/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_in_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jconde-a <jconde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 19:42:49 by jconde-a          #+#    #+#             */
/*   Updated: 2024/08/09 17:30:36 by jconde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Establish the path to the new directory, which is current minus one level
int	ft_minus_dir( t_struct_path *dir)
{
	int		i;
	char	*aux;

	i = 0;
	while ((i < dir->num_dir - 1) && ft_strlen(dir->line_path) > 1)
	{
		aux = dir->route;
		dir->route = ft_strjoin(dir->route, dir->path[i]);
		free(aux);
		if (!dir->route)
			return (-1);
		if (i < (dir->num_dir - 2))
		{
			aux = dir->route;
			dir->route = ft_strjoin(dir->route, "/");
			free(aux);
			if (!dir->route)
				return (-1);
		}
		i++;
	}
	return (0);
}

//Establish the path to the new directory, which is the same as current
int	ft_equal_dir(t_struct_path *dir)
{
	int		i;
	char	*aux;

	i = 0;
	while ((i < dir->num_dir) && ft_strlen(dir->line_path) > 1)
	{
		aux = dir->route;
		dir->route = ft_strjoin(dir->route, dir->path[i]);
		free(aux);
		if (!dir->route)
			return (-1);
		if (i < (dir->num_dir - 1))
		{
			aux = dir->route;
			dir->route = ft_strjoin(dir->route, "/");
			free(aux);
			if (!dir->route)
				return (-1);
		}
		i++;
	}
	return (0);
}

//Create the path in route
int	ft_aux2_cd_dir(t_struct_path *dir)
{
	int		i;
	char	*aux;

	i = 0;
	while ((i < dir->num_dir) && ft_strlen(dir->line_path) > 1)
	{
		aux = dir->route;
		dir->route = ft_strjoin(dir->route, dir->path[i]);
		free(aux);
		if (!dir->route)
		{
			return (-1);
		}
		if (i < (dir->num_dir))
		{
			aux = dir->route;
			dir->route = ft_strjoin(dir->route, "/");
			free(aux);
			if (!dir->route)
				return (-1);
		}
		i++;
	}
	return (0);
}

int	ft_aux_cd_dir(t_command *command, t_struct_path *dir)
{
	char	*aux;
	char	*aux3;

	aux = dir->route;
	aux3 = command->args[1];
	if (command -> args[1][0] == '/')
	{
		free(aux);
		aux3++;
		dir->route = malloc(2);
		if (!dir->route)
			return (-1);
		dir->route[0] = '/';
		dir->route[1] = '\0';
		aux = dir->route;
	}
	dir->route = ft_strjoin(dir->route, aux3);
	free(aux);
	if (!dir->route)
		return (-1);
	return (0);
}

//Establish the path to the new directory, adding the new argument to the current path
int	ft_cd_dir(t_command *command, t_struct_path *dir)
{
	char	*aux;

	if (ft_aux2_cd_dir(dir))
		return (-1);
	aux = ft_strdup(dir->route);
	if (!aux)
		return (-1);
	aux[ft_strlen(aux)-1] = '\0';
	if (ft_aux_cd_dir(command, dir) == -1)
	{
		free(aux);
		return (-1);
	}
	if (!ft_is_dir_ok(dir->route))
	{
		write(command -> file_output, "Not a directory\n", 16);
		free(dir->route);
		dir->route = aux;
		free(aux);
		return (1);
	}
	free(aux);
	return (0);
}
