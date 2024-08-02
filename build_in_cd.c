/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_in_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jconde-a <jconde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 19:42:49 by jconde-a          #+#    #+#             */
/*   Updated: 2024/08/02 11:48:35 by jconde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

 
/*se establece la ruta al nuevo directorio quitando un nivel al actual*/
 int ft_minus_dir( t_struct_path *dir)
 {
	int     i;
	char    *aux;

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
 int ft_equal_dir(t_struct_path *dir)
 {
	int     i;
	char    *aux;

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

/*/*se establece la ruta al nuevo directorio añadiendo a la ruta actual el agumento nuevo*/
int	ft_cd_dir(t_command *command,t_struct_path *dir)
 {
	char	*aux;
	char	*aux2;
	char	*aux3;
	int		i;

	i = 0;
	while ((i < dir->num_dir) && ft_strlen(dir->line_path) > 1)
	{
		aux = dir->route;
		dir->route = ft_strjoin(dir->route, dir->path[i]);
		free(aux);
		if (!dir->route)
					return (-1);
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
	aux2 = aux;
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
	dir->route = ft_strjoin(dir->route, aux3); //malloc  voy añadiendo dir menos el último
		free(aux);
		if (!dir->route)
			return (-1);
		if (!ft_is_dir_ok(dir->route)) //Aqui compruebo si existe el dir
		{
			write(command -> file_output, "Not a directory\n", 16);
			dir->route = aux2;
			return (1);
		}
	return (0);

}

/*están las distintas opciones según lo que viene en arg. desde ahí llamamos a funciones*/
int ft_aux2_buil_cd(t_command *command, t_struct_path *dir)
{
	int ok;
	char *arg;

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
	else if (((arg[0] == '.') && (command -> args[2])) || (((!ft_memcmp(arg, "./", 2) ||
		!ft_memcmp(arg, "../", 3)) && (command -> args[2]))))
		perror("Error cd: too many arguments");
	else if (ft_strlen(arg) > 0)
		ok =ft_cd_dir(command, dir);
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
