/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_int3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jconde-a <jconde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 18:04:00 by jconde-a          #+#    #+#             */
/*   Updated: 2024/09/04 12:39:00 by jconde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_aux1_cd(t_command *command_act, t_list **env)
{
	int	ok;

	ok = 0;
	if ((command_act->args[1]) == NULL)
	{
		ok = 1;
		if (ft_cd_without_argument(command_act, env) == 1)
			return (1);
	}
	ft_built_cd(command_act, env);
	if (ok != 0)
	{
		free(command_act -> args[1]);
		ok = 0;
	}
	return (ok);
}

int	ft_aux1_build_int(t_command *command_act, t_list **env)
{
	char	*comando;
	int		ok;

	comando = command_act -> command;
	ok = 0;
	if (ft_memcmp(comando, "echo", 4) == 0)
		ok = ft_built_echo(command_act);
	else if (ft_memcmp(comando, "cd", 2) == 0)
		ok = ft_aux1_cd (command_act, env);
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

int	ft_do_error_arg(char *argument)
{
	char	*error;

	error = ft_strjoin(argument, " export: not a valid identifier");
	if (!error)
		return (1);
	perror(error);
	free (error);
	return (1);
}

/*Check that it is ok the variable's name*/
int	ft_check_export_arg(char *argument)
{
	char	*ptr;

	ptr = argument;
	if (argument[0] >= '0' && argument[0] <= '9')
		return (ft_do_error_arg(ptr));
	while (*argument != '=' && *argument != '+' && *argument != '\0')
	{
		if ((ft_isalnum(*argument) == 0))
			return (ft_do_error_arg(ptr));
		argument++;
	}
	if (*argument == '+')
	{
		argument++;
		if (*argument != '=')
			return (ft_do_error_arg(ptr));
	}
	return (0);
}
