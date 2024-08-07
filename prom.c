/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prom.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexigar <alexigar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 12:43:54 by jconde-a          #+#    #+#             */
/*   Updated: 2024/08/07 19:05:42 by alexigar         ###   ########.fr       */
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

/*procesa line y si hay exit despues de pipe devuelve 1 para salir*/
int	ft_proces(char *line, t_list **env)
{
	char				**token;
	t_command			**commands;
	int					last_return;

	token = lexer(line);
	free(line);
	if (!token)
		fr_free_prom(env, NULL, NULL, "Error Mem en LEXER");
	if (!expander(token, env))
		fr_free_prom(env, token, NULL, "Error Mem en EXPANDER");
	commands = parser(token, env);
	if (!commands)
		fr_free_prom(env, token, NULL, "Error Mem en PARSER");
	g_is_executing = 1;
	last_return = executor(commands, env);
	g_is_executing = 0;
	if (last_return == -1)
		fr_free_prom(env, token, commands, "Error Mem en EXECUTOR");
	if (last_return == -2)
		return (fr_free_prom2(env, token, commands, NULL));
	ft_save_last_return(last_return, env);
	fr_free_prom2(NULL, token, commands, NULL);
	return (0);
}

int	manage_line(char *line, int *control, t_list **env)
{
	if (!line)
		return (0);
	if (line[0] == '\0')
	{
		free(line);
		return (1);
	}
	if (ft_memcmp(line, "exit", 4) == 0 && ft_strlen(line) == 4)
	{
		free(line);
		control = 0;
		return (0);
	}
	add_history(line);
	if (ft_proces (line, env) == 1)
		return (0);
	return (1);
}

/*  Main command interpreter loop.
    Initializes signal handlling, reads user input,
    parses and executes commands.*/
void	prom(t_list **env)
{
	char				*line;
	int					control;
	char				*path_act;
	char				*aux;
	struct sigaction	action;

	control = 1;
	action = create_sigaction();
	while (control == 1)
	{
		path_act = ft_get_var_env(env, "..PWD");
		if (!path_act)
			return ;
		aux = path_act;
		path_act = ft_strjoin(path_act, " XXX$ ");
		free(aux);
		if (!path_act)
			return ;
		line = readline(path_act);
		free(path_act);
		if (!manage_line(line, &control, env))
			return ;
	}
}
