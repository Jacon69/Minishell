/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prom.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaimecondea <jaimecondea@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 12:43:54 by jconde-a          #+#    #+#             */
/*   Updated: 2024/08/07 07:30:24 by jaimecondea      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*This function intercepts and processes the SIGINT (Ctrl+C)
 and SIGQUIT (Ctrl+barra) signals,enabling custom actions to
  be taken when these signals are received by the shell.*/
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

/*Libera y devuelve el prom*/
int fr_free_prom2(t_list **env, char **token, t_command **commands, char *msg)
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
void fr_free_prom(t_list **env, char **token, t_command **commands, char *msg)
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
void ft_save_last_return(int last_return, t_list **env)
{
	char *str_last_return;

	str_last_return = ft_itoa(last_return);
	ft_save_var_env("?", str_last_return, env);
	free(str_last_return);
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
	last_return = executor(commands, env);
	if (last_return == -1)
		fr_free_prom(env, token, commands, "Error Mem en EXECUTOR");
	if (last_return == -2)
		return (fr_free_prom2(env, token, commands, NULL));
	ft_save_last_return(last_return, env);
	fr_free_prom2(NULL, token, commands, NULL);
	return (0);
}

/*  Main command interpreter loop.
    Initializes signal handlling, reads user input,
    parses and executes commands.*/
void prom(t_list  **env)
{
	char				*line;
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
		free(path_act);
		if (g_is_executing == -1)
		{
			free(line);
			g_is_executing = 0;
			continue ;
		}
		else
		{
			g_is_executing = 0;
			if (!line)
			{
				perror("Line es nulo\n");
				break ;
			}
			if (line[0] == '\0')
			{
				free(line);
				continue ;
			}
			if (ft_memcmp(line, "exit", 4) == 0 && ft_strlen(line) == 4)
			{
				free(line);
				control = 0;
				return ;
			}
			add_history(line);
			if (ft_proces (line, env) == 1)
				return ;
		}
	}
}
