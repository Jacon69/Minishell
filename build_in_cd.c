/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_in_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexigar <alexigar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 19:42:49 by jconde-a          #+#    #+#             */
/*   Updated: 2024/08/08 10:10:55 by alexigar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*se establece la ruta al nuevo directorio quitando un nivel al actual*/
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

/*se establece la ruta al nuevo directorio dejandolo como el actual actual*/
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

/*crea la ruta en  route*/
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

/*/*se establece la ruta al nuevo directorio añadiendo
 a la ruta actual el agumento nuevo*/
int	ft_cd_dir(t_command *command, t_struct_path *dir)
{
	char	*aux;

	if (ft_aux2_cd_dir(dir))
		return (-1);
	aux = ft_strdup(dir->route); //leak
	aux[ft_strlen(aux)-1] = '\0';
	if (ft_aux_cd_dir(command, dir) == -1)
		return (-1);
	if (!ft_is_dir_ok(dir->route))
	{
		write(command -> file_output, "Not a directory\n", 16);
		free(dir->route);
		dir->route = aux;
		return (1);
		free(aux);
	}
	return (0);
}
