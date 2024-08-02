/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build-int.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jconde-a <jconde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 05:32:30 by jaimecondea       #+#    #+#             */
/*   Updated: 2024/08/02 09:45:08 by jconde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"


int	ft_checkwithout_n(char *cad)
{
	if (!ft_memcmp(cad, "-n", 2)
		&& ft_strlen(cad) == 2)
		return (1);
	return (0);
}

/* Implements the 'echo' command for the minishell.
   Prints text to standard output or a file.
   Supports the '-n' option and special character escaping.
*/
int	ft_built_echo(t_command *command)
{
	int	i;
	int	ok;

	ok = 0;
	i = (1 + ft_checkwithout_n(command -> args[1]));
	while (command -> args[i])
	{
		if( 0 > write(command -> file_output, command -> args[i], ft_strlen(command -> args[i])))
			return(1);
		if (command -> args[i+1])
			write(command -> file_output, " ", 1);
		i++;
	}
		if (!ft_checkwithout_n(command -> args[1]))
			 write(command -> file_output, "\n", 1);
	return(ok);
}



/* Changes the current working directory.
   Supports relative and absolute paths.
   Recognizes the ".." symbol as the previus directory.
   Returns 0 on success, or a non-zero value on error.*/
int	ft_built_cd(t_command *command, t_list **env)
{
	int		ok;
	t_struct_path *dir;

	dir = malloc(sizeof(t_struct_path));
	if (dir == NULL)
		return (-1);
	dir->route = NULL;
	dir->line_path = NULL;
	dir->num_dir = 0;
	dir->path = NULL;
	ok = 0;
	if( ok != ft_aux1_buil_cd(command, dir))
		return(1); 
	ok = ft_aux2_buil_cd(command, dir);
	ft_save_var_env("PWD", dir->route, env);
	ft_save_var_env("OLDPWD", dir->line_path, env);
	ft_save_var_env("..PWD", dir->route, env);
	free(dir->route);
	ft_free_char(dir->path);
	free(dir);
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

int ft_aux1_build_int(t_command *command_act, t_list **env)
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
	return (ok);
}


/* ft_build_int: Handles built-in commands of the 
minishell: echo, cd, pwd, export, unset, env, exit */
int	ft_build_int(t_command *command_act, t_list **env)
{
	char	*comando;
	int		ok;

	comando = command_act -> command;
	ok = 0;
	if ((ft_memcmp(comando, "echo", 4) == 0) || (ft_memcmp(comando, "cd", 2) == 0) ||
	(ft_memcmp(comando, "pwd", 3) == 0) || (ft_memcmp(comando, "export", 6) == 0) ||
	(ft_memcmp(comando, "unset", 5) == 0) || (ft_memcmp(comando, "env", 3) == 0))
		ft_aux1_build_int(command_act, env);
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
	if (ok < 0)
		perror("Error MEM built_in");
	return (ok);
}
