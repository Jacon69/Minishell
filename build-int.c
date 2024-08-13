/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build-int.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jconde-a <jconde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 05:32:30 by jaimecondea       #+#    #+#             */
/*   Updated: 2024/08/13 17:47:35 by jconde-a         ###   ########.fr       */
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
	int	ok;

	if (command -> args[1] == NULL)
	{
		ok = ft_print_list_env(command, env);
		return (ok);
	}
	if (ft_memchr(command->args[1], '=', ft_strlen(command -> args[1])) != NULL)
	{
		if (ft_check_export_arg(command->args[1]) == 1)
			return (1);
		printf("variable completa %s \n", command->args[1]);
		ft_add_v_env(command->args[1], env);
		return (0);
	}
	return (0);
}

/* ft_built_unset: Deletes the specified environment 
		variables from the global environment. 
   If the variable doesn't exist, it is ignored. */
int	ft_built_unset(t_command *command, t_list **env)
{
	(void)command;
	(void)env;
	if ((command -> args[1]) && (ft_strlen(command -> args[1])) > 1)
	{
		ft_del_v_env(command -> args[1], env);
		return (0);
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
		return (-2);
	if (command_act -> file_input != 1)
		close(command_act -> file_input);
	if (command_act -> file_output != 1)
		close(command_act -> file_output);
	if (ok < 0)
		perror("Error MEM built_in");
	return (ok);
}
