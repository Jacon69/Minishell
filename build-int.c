/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build-int.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jconde-a <jconde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 05:32:30 by jaimecondea       #+#    #+#             */
/*   Updated: 2024/07/31 11:32:32 by alexigar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

/* Implements the 'echo' command for the minishell.
   Prints text to standard output or a file.
   Supports the '-n' option and special character escaping.
*/
int	ft_built_echo(t_command *command)
{
	int	jump_line;
	int	i;
	int	ok;

	ok = 1;
	i = 1;
	jump_line = 1;
	if (!ft_memcmp(command -> args[1], "-n", 2)
		&& ft_strlen(command -> args[1]) == 2)
	{
		jump_line = 0;
		i++;
	}
	while (command -> args[i])
	{
		ok *= write(command -> file_output, command -> args[i],
				ft_strlen(command -> args[i]));
		if (ok > 0)
			ok = 1;
		else
			ok = 0;
		if (command -> args[i + 1])
		{
			if (!jump_line)
			{
				ok *= write(command -> file_output, "\n", 1);
				if (ok > 0)
					ok = 1;
				else
					ok = 0;
			}
			else
			{
				ok *= write(command -> file_output, " ", 1);
				if (ok > 0)
					ok = 1;
				else
					ok = 0;
			}
		}
		i++;
	}
	ok *= write(command->file_output, "\n", 1);
	return((ok <= 0) ? 1 : 0);
}

/* Changes the current working directory.
   Supports relative and absolute paths.
   Recognizes the ".." symbol as the previus directory.
   Returns 0 on success, or a non-zero value on error.*/
int	ft_built_cd(t_command *command, t_list **env)
{
	int		i;
	char	**path;
	char	*line_path;
	char	*route;
	char	*aux;
	char	*aux2;
	char	*aux3;
	int		num_dir;
	int		ok;

	ok = 0;
	line_path = command->path;
	route = malloc(2);
	if (!route)
	{
		perror("Error built_in");
		return (-1);
	}
	route[0] = '/';
	route[1] = '\0';
	num_dir = 0;
	i = 0;
	while (line_path[i])
	{
		if (line_path[i] == '/')
		{
			num_dir++;
		}
		i++;
	}
	i = 0;
	path = ft_split(line_path, '/');
	if (!path)
	{
		free(route);
		perror("Error MEM built_in1");
		return (-1);
	}
	if ((((!ft_memcmp(command -> args[1], "..", 2)
					&& ft_strlen(command -> args[1]) == 2))
			|| (!ft_memcmp(command -> args[1], "../", 3)
				&& (ft_strlen(command -> args[1]) == 3)))
		&& !(command -> args[2]))
	{
		while ((i < num_dir - 1) && ft_strlen(line_path) > 1)
		{
			aux = route;
			route = ft_strjoin(route, path[i]);
			free(aux);
			if (!route)
			{
				perror("Error MEM built_in2");
				ft_free_char(path);
				return (-1);
			}
			if (i < (num_dir - 2))
			{
				aux = route;
				route = ft_strjoin(route, "/");
				free(aux);
				if (!route)
				{
					perror("Error MEM built_in3");
					ft_free_char(path);
					return (-1);
				}
			}
			i++;
		}
	}
	else if (((((command -> args[1][0] == '.'
					&& ft_strlen(command -> args[1]) == 1))
			|| (!ft_memcmp(command->args[1], "./", 2)
				&& ft_strlen(command -> args[1]) == 2)))
			&& !(command -> args[2]))
	{
		while ((i < num_dir) && ft_strlen(line_path) > 1)
		{
			aux = route;
			route = ft_strjoin(route, path[i]);
			free(aux);
			if (!route)
			{
				perror("Error MEM built_in4");
				ft_free_char(path);
				return (-1);
			}
			if (i < (num_dir - 1))
			{
				aux = route;
				route = ft_strjoin(route, "/");
				free(aux);
				if (!route)
				{
					perror("Error MEM built_in5");
					ft_free_char(path);
					return (-1);
				}
			}
			i++;
		}
	}
	else if ((command -> args[1][0] == '.') && (command -> args[2]))
	{
		perror("Error cd: too many arguments1");
		ft_free_char(path);
		free(route);
		return (1);
	}
	else if ((!ft_memcmp(command -> args[1], "./", 2)
			|| !ft_memcmp(command -> args[1], "../", 3))
		&& (command -> args[2]))
	{
		perror("Error cd: too many arguments2");
		ft_free_char(path);
		free(route);
		return (1);
	}
	else if (ft_strlen(command -> args[1]) > 0)
	{
		while ((i < num_dir) && ft_strlen(line_path) > 1)
		{
			aux = route;
			route = ft_strjoin(route, path[i]);
			free(aux);
			if (!route)
			{
				perror("Error MEM built_in7");
				ft_free_char(path);
				return (-1);
			}
			if (i < (num_dir - 1))
			{
				aux = route;
				route = ft_strjoin(route, "/");
				free(aux);
				if (!route)
				{
					perror("Error MEM built_in8");
					ft_free_char(path);
					return (-1);
				}
			}
			if (command -> args[1][0] != '/' && i == (num_dir - 1))
			{
				aux = route;
				route = ft_strjoin(route, "/");
				if (!route)
				{
					perror("Error MEM built_in9");
					ft_free_char(path);
					return (-1);
				}
			}
			i++;
		}
		aux2 = aux;
		aux = route;
		aux3 = command->args[1];
		if (command -> args[1][0] == '/')
		{
			free(aux);
			aux3++;
			route = malloc(2);
			if (!route)
			{
				perror("Error built_in");
				ft_free_char(path);
				return (-1);
			}
			route[0] = '/';
			route[1] = '\0';
			aux = route;
		}
		route = ft_strjoin(route, aux3);
		free(aux);
		if (!route)
		{
			perror("Error MEM built_in10");
			ft_free_char(path);
			return (-1);
		}
		if (!ft_is_dir_ok(route))
		{
			write(command -> file_output, "Not a directory\n", 16);
			free(route);
			route = aux2;
			ok = 1;
		}
	}
	ft_save_var_env("PWD", route, env);
	ft_save_var_env("OLDPWD", line_path, env);
	ft_save_var_env("..PWD", route, env);
	free(route);
	ft_free_char(path);
	return (ok);
}

/* ft_built_pwd(t_command *command): Returns the absolute path of the current directory. */
int	ft_built_pwd(t_command *command)
{
	int	ok;

	ok = write(command -> file_output, command -> path,
			ft_strlen(command -> path));
	write(command -> file_output, "\n", 1);
	return((ok <= 0) ? 1 : 0);
}

/* ft_built_export:
   - Assigns values to existing or new environment variables.
   - If no assignment is specified, prints the entire list of environment variables in "VAR=value" format.
   - Syntax: export [VAR=value]
   - Returns 0 on success, or a non-zero value on error.
*/
int	ft_built_export(t_command *command, t_list **env)
{
	int	ok;

	if (command -> args[1] == NULL)
	{
		ok = ft_print_list_env(command, env);
		return (ok);
	}
	if (ft_memchr(command->args[1], '=', ft_strlen(command -> args[1])) != NULL)
	{
		ft_add_v_env(command->args[1], env);
		return (0);
	}
	return (0);
}

/* ft_built_unset: Deletes the specified environment variables from the global environment. 
   If the variable doesn't exist, it is ignored. */
int	ft_built_unset(t_command *command, t_list **env)
{
	(void)command;
	(void)env;
	if (ft_strlen(command -> args[1]) > 1)
	{
		ft_del_v_env(command -> args[1], env);
		return (0);
	}
	return (0);
}

int	ft_built_env(t_command *command, t_list **env)
{
	return (ft_print_list_env(command, env));
}

/* ft_build_int: Handles built-in commands of the 
minishell: echo, cd, pwd, export, unset, env, exit */
int	ft_build_int(t_command *command_act, t_list **env)
{
	char	*comando;
	int		ok;

	comando = command_act -> command;
	ok = 0;
	if (ft_memcmp(comando, "echo", 4) == 0)
		ok = ft_built_echo(command_act);
	else if (ft_memcmp(comando, "cd", 2) == 0)
		ok = (command_act->args[1]) == NULL ? 1 : ft_built_cd(command_act, env);
	else if (ft_memcmp(comando, "pwd", 3) == 0)
		ok = ft_built_pwd(command_act);
	else if (ft_memcmp(comando, "export", 6) == 0)
		ok = ft_built_export(command_act, env);
	else if (ft_memcmp(comando, "unset", 5) == 0)
		ok = ft_built_unset(command_act, env);
	else if (ft_memcmp(comando, "env", 3) == 0)
		ok = ft_built_env(command_act, env);
	else if (ft_memcmp(comando, "exit", 4) == 0)
	{
		write(command_act -> file_output, "exit built_int\n", 15);
		free(command_act);
		ft_free_list(env);
		return (-2);
	}
	if (command_act -> file_input != 1)
		close(command_act -> file_input);
	if (command_act -> file_output != 1)
		close(command_act -> file_output);
	return (ok);
}
