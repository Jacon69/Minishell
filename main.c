/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jconde-a <jconde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 12:43:45 by jconde-a          #+#    #+#             */
/*   Updated: 2024/08/16 11:03:02 by jconde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	sigint_handler(void)
{
	if (g_is_executing == 0)
	{
		rl_replace_line("", 1);
		printf("\n");
		rl_on_new_line();
		rl_redisplay();
		return (0);
	}
	else
	{
		g_is_executing = 0;
		return (1);
	}
}

int	main(int narg, char *carg[], char **environment)
{
	t_list			**env;
	struct termios	attributes;

	printf("\033[0;32m");
	narg++;
	carg [0] = "hola";
	env = ft_ini_env(environment);
	if (env == NULL)
		return (1);
	ft_save_var_env("?", "0", env, 1);
	tcgetattr(STDIN_FILENO, &attributes);
	attributes.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &attributes);
	prom(env);
	ft_free_list(env, NULL);
	return (g_exit);
}
