/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build-int.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaimecondea <jaimecondea@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 05:32:30 by jaimecondea       #+#    #+#             */
/*   Updated: 2024/08/17 08:10:40 by jaimecondea      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ft_built_export:
   - Assigns values to existing or new environment variables.
   - If no assignment is specified, prints the entire list of 
   		environment variables in "VAR=value" format.
   - Syntax: export [VAR=value]
   - Returns 0 on success, or a non-zero value on error.
*/
int	ft_built_export(t_command *command, t_list **env)
{
	int		i;
	char	*ptr;

	i = 1;
	if (command -> args[1] == NULL)
		return (ft_print_list_env(command, env));
	while (command -> args[i])
	{
		ptr = ft_memchr(command->args[i], '+', ft_strlen(command->args[i]));
		if (ft_check_export_arg(command->args[i]) == 1)
			return (1);
		if (ptr != NULL && ptr[1] == '=')
			ft_join_v_env(command->args[i], env, 1);
		else
			ft_add_v_env(command->args[i], env, 1);
		i++;
	}
	return (0);
}

/* ft_built_unset: Deletes the specified environment 
		variables from the global environment. 
   If the variable doesn't exist, it is ignored. */
int	ft_built_unset(t_command *command, t_list **env)
{
	int		i;

	(void)command;
	(void)env;
	i = 0;
	while ((command -> args[i]) && (ft_strlen(command -> args[i])) > 1)
	{
		ft_del_v_env(command -> args[i], env);
		i++;
	}
	return (0);
}

int	ft_built_env(t_command *command, t_list **env)
{
	int	ok;

	if (command->args[1])
	{
		perror("env: No such file or directory");
		return (1);
	}
	ok = ft_print_list_env(command, env);
	return (ok);
}

int	ft_cd_without_argument(t_command *command_act, t_list **env)
{
	command_act->args[1] = ft_get_var_env(env, "HOME");
	if (command_act->args[1][0] == '\0')
	{
		perror("cd: HOME not set");
		return (1);
	}
	return (0);
}

/* ft_build_int: Handles built-in commands of the 
minishell: echo, cd, pwd, export, unset, env, exit */
int	ft_build_int(t_command *command_act, t_list **env)
{
	char	*com;
	int		ok;

	com = command_act -> command;
	ok = 0;
	if ((ft_memcmp(com, "echo", 4) == 0) || (ft_memcmp(com, "cd", 2) == 0)
		|| (ft_memcmp(com, "pwd", 3) == 0) || (ft_memcmp(com, "export", 6) == 0)
		|| (ft_memcmp(com, "unset", 5) == 0) || (ft_memcmp(com, "env", 3) == 0))
		ft_aux1_build_int(command_act, env);
	else if (ft_memcmp(com, "exit", 4) == 0)
	{
		g_exit = ft_exit(command_act -> args[1]);
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
