/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prom.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jconde-a <jconde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 12:43:54 by jconde-a          #+#    #+#             */
/*   Updated: 2024/08/01 10:36:57 by jconde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*This function intercepts and processes the SIGINT (Ctrl+C) and SIGQUIT (Ctrl+barra) signals,
 enabling custom actions to be taken when these signals are received by the shell.*/
static void	signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		if (g_is_executing == 0)
		{
			rl_replace_line("", 1);
			printf("\n");
			rl_on_new_line();
			rl_redisplay();
		}
		else if (g_is_executing == 1)
		{
			g_is_executing = 0;
			return ;
		}
		else
		{
			printf("%d\n", g_is_executing);
			g_is_executing = -1;
			return ;
		}
	}
	if (signum == SIGQUIT)
	{
		if (g_is_executing == 1)
		{
			printf("Quit");
			g_is_executing = 0;
			return ;
		}
		else
			return ;
	}
}
/*  Main command interpreter loop.
    Initializes signal handling, reads user input,
    parses and executes commands.*/
void prom(t_list  **env)
{
	char				*line;
	char				**token;
	t_command			**commands;
	int					last_return;
	char				*str_last_return;
	int					control;
	char				*path_act;
	char				*aux;
	struct sigaction	action;

	control = 1;
	action.sa_handler = signal_handler;
	action.sa_flags = 0;
	sigemptyset(&action.sa_mask);
	sigaction(SIGINT, &action, NULL);
	sigaction(SIGQUIT, &action, NULL);
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
		if (g_is_executing == -1)
		{
			free(line);
			free(path_act);
			g_is_executing = 0;
			continue ;
		}
		else
		{
			g_is_executing = 0;
			if (!line)
			{
				printf("exit\n");
				printf("Line es nulo\n");
				break ;
			}
			if (line[0] == '\0')
			{
				free(line);
				free(path_act);
				continue ;
			}
			if (ft_memcmp(line, "exit", 4) == 0 && ft_strlen(line) == 4)
			{
				free(line);
				free(path_act);
				control = 0;
				return ;
			}
			add_history(line);
			token = lexer(line);
			if (!token)
			{
				free(line);
				free(path_act);
				ft_free_list(env);
				perror("Error Mem en LEXER");
				exit(1);
			}
			if (!expander(token, env))
			{
				free(line);
				free(path_act);
				ft_free_char(token);
				ft_free_list(env);
				perror("Error Mem en EXPANDER");
				exit(1);
			}
			commands = parser(token, env);
			if (!commands)
			{
				free(line);
				free(path_act);
				ft_free_char(token);
				ft_free_list(env);
				perror("Error Mem en PARSER");
				exit(1);
			}
			last_return = executor(commands, env);
			if (last_return == -1)
			{
				free(line);
				free(path_act);
				ft_free_char(token);
				ft_free_list(env);
				free_commands(commands);
				perror("Error Mem in EXECUTOR");
				exit(1);
			}
			if (last_return == -2)
			{
				free(line);
				free(path_act);
				ft_free_char(token);
				ft_free_list(env);
				free_commands(commands);
				return;
			}

			str_last_return= ft_itoa(last_return);
			ft_save_var_env("?", str_last_return,env);
			free(str_last_return);
			ft_free_char(token);
			free(line);
			free(path_act);
			free_commands(commands);
		}
	}
}
