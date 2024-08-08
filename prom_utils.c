/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prom_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexigar <alexigar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 19:01:12 by alexigar          #+#    #+#             */
/*   Updated: 2024/08/08 10:02:16 by alexigar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*Libera y devuelve el prom*/
int	fr_free_prom2(t_list **env, char **token, t_command **commands, char *msg)
{
	if (env)
		ft_free_list(env, msg);
	if (token)
		ft_free_char(token);
	if (commands)
		free_commands(commands);
	return (1);
}

/*Libera y cierra programa*/
void	fr_free_prom(t_list **env, char **token,
	t_command **commands, char *msg)
{
	if (env)
		ft_free_list(env, msg);
	if (token)
		ft_free_char(token);
	if (commands)
		free_commands(commands);
	exit(1);
}

/*Si la guarda la salida de la ejecución*/
void	ft_save_last_return(int last_return, t_list **env)
{
	char	*str_last_return;

	str_last_return = ft_itoa(last_return);
	ft_save_var_env("?", str_last_return, env);
	free(str_last_return);
}

/*This function intercepts and processes the SIGINT (Ctrl+C)
 and SIGQUIT (Ctrl+\) signals,enabling custom actions to
  be taken when these signals are received by the shell.*/
static void	signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		if (sigint_handler())
			return ;
	}
	if (signum == SIGQUIT)
	{
		if (g_is_executing == 1)
		{
			printf("Quit\n");
			g_is_executing = 0;
			return ;
		}
		else
			return ;
	}
}

struct sigaction	create_sigaction(void)
{
	struct sigaction	action;

	action.sa_handler = signal_handler;
	action.sa_flags = 0;
	sigemptyset(&action.sa_mask);
	sigaction(SIGINT, &action, NULL);
	sigaction(SIGQUIT, &action, NULL);
	return (action);
}
