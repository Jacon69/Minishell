/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prom.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaimecondea <jaimecondea@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 12:43:54 by jconde-a          #+#    #+#             */
/*   Updated: 2024/08/28 08:08:04 by jaimecondea      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	no_token(t_list **env, int flag)
{
	if (flag == 0)
		ft_free_prom(env, NULL, NULL, "Error Mem en LEXER");
	printf("\033[0;31m");
	return (0);
}

int	ft_aux_proces(t_list **env, char **token, t_command **commands)
{
	int	last_return;

	g_is_executing = 1;
	last_return = executor(commands, env);
	g_is_executing = 0;
	if (last_return == -1)
		ft_free_prom(env, token, commands, "Error Mem en EXECUTOR");
	if (last_return == -2)
	{
		ft_free_prom2(NULL, token, commands, NULL);
		return (1);
	}
	ft_save_last_return(last_return, env);
	ft_free_prom2(NULL, token, commands, NULL);
	return (0);
}

//Processes line and returns 1 if there's exit after a pipe
int	ft_proces(char *line, t_list **env)
{
	char				**token;
	t_command			**commands;
	int					flag;

	flag = 0;
	token = lexer(line, &flag);
	free(line);
	if (!token)
		return (no_token(env, flag));
	if (!expander(token, env))
		ft_free_prom(env, token, NULL, "Error Mem en EXPANDER");
	commands = parser(token, env, &flag);
	if (!commands && flag == 0)
		ft_free_prom(env, token, NULL, "Error Mem en PARSER");
	else if (commands)
	{
		if (ft_aux_proces(env, token, commands) == 1)
			return (1);
	}
	else
		ft_save_last_return(1, env);
	return (0);
}

/*if the function returns 0 them it goes out*/
int	manage_line(char *line, t_list **env)
{
	if (!line)
		return (0);
	if (line[0] == '\0')
	{
		free(line);
		g_exit = 1;
		return (1);
	}
	if (ft_memcmp(line, "exit", 4) == 0 && (ft_strlen(line) == 5))
	{
		free(line);
		g_exit = 0;
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
		path_act = ft_strjoin(path_act, " XXX$ \033[0;37m");
		free(aux);
		if (!path_act)
			return ;
		line = readline(path_act);
		free(path_act);
		if (!manage_line(line, env))
			return ;
	}
}
