/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexigar <alexigar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 17:38:21 by jconde-a          #+#    #+#             */
/*   Updated: 2024/09/02 10:02:10 by alexigar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*Check the stream is only number. 1 true*/
int	ft_chck_nbs(char *numero)
{
	if (*numero == '-')
		numero++;
	while (*numero != '\0')
	{
		if (!ft_isdigit(*numero))
			return (0);
		numero++;
	}
	return (1);
}

int	ft_count_argument(char **arg)
{
	int	count;

	count = 0;
	while (arg[count])
		count++;
	return (--count);
}

int	ft_exit_wrong_args(char *arg, int errcode)
{
	if (errcode == -2)
	{
		printf("exit: %s numeric argument required\n", arg);
		g_exit = 2;
	}
	else if (errcode == 0)
		printf("exit: too many arguments \n");
	return (errcode);
}

/*convert arg[1] from command exit to number for exit*/
int	ft_exit(t_command *command_act)
{
	int	nerror;
	int	narg;

	g_exit = 0;
	if (!command_act -> args[1])
		return (-2);
	if (!ft_chck_nbs(command_act -> args[1]))
		return (ft_exit_wrong_args(command_act -> args[1], -2));
	narg = ft_count_argument(command_act -> args);
	if (narg > 1)
		return (ft_exit_wrong_args(command_act -> args[1], 0));
	if (narg == 1)
	{
		nerror = ft_atoi (command_act -> args[1]);
		if (nerror < 0)
			nerror += 256;
		nerror %= 256;
		g_exit = nerror;
	}
	return (-2);
}

int	no_envs(t_command *com, t_list **env)
{
	char		*function_call;
	struct stat	buf;

	function_call = com -> command;
	if (stat(function_call, &buf) == 0
		&& S_ISREG(buf.st_mode) && (buf.st_mode & S_IXUSR))
		return (fork_function(com, function_call, env));
	else
	{
		printf("Error: command not found\n");
		return (127);
	}
}
