/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_children.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexigar <alexigar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 12:07:52 by alexigar          #+#    #+#             */
/*   Updated: 2024/08/29 13:08:41 by alexigar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_count_commands(t_command **command_list)
{
	int	i;

	i = 0;
	while (command_list[i])
		i++;
	return (i);
}

void	ft_kill_children(pid_t *children, int n_commands)
{
	int	i;

	i = 0;
	while (i < n_commands)
	{
		kill(children[i], SIGKILL);
		i++;
	}
}

int	no_envs_child(t_command *com, t_list **env)
{
	char		*function_call;
	struct stat	buf;

	function_call = com -> command;
	if (stat(function_call, &buf) == 0
		&& S_ISREG(buf.st_mode) && (buf.st_mode & S_IXUSR))
	{
		execute_function(com, function_call, env);
		return (0);
	}
	else
	{
		printf("Error: command not found\n");
		return (127);
	}
}

int	try_call_child(char **paths, t_command *com, t_list **env)
{
	int			i;
	char		*function_call;
	struct stat	buf;

	i = -1;
	if (!paths[0] && com -> command[0] == '/')
		return (no_envs_child(com, env));
	while (paths[++i])
	{
		if (com -> command[0] == '/')
			function_call = com -> command;
		else
			function_call = get_function_call(com -> command, paths[i]);
		if (!function_call)
			return (-1);
		if (stat(function_call, &buf) == 0
			&& S_ISREG(buf.st_mode) && (buf.st_mode & S_IXUSR))
		{
			execute_function(com, function_call, env);
			return (com -> returned_output);
		}
		else
			free_function_call(com, function_call);
	}
	printf("Error: command not found\n");
	return (127);
}

int	execute_child(t_command *com, t_list **env, char **paths)
{
	if (is_built_in(com -> command))
		return (ft_build_int(com, env));
	else
		return (try_call_child(paths, com, env));
}

int	manage_pipes(t_command **command_list,
	t_list **env, char **paths)
{
	int		n_commands;
	pid_t	*children;
	pid_t	pid;
	int		i;
	int		to_return;
	int		empty;
	int		status;

	n_commands = ft_count_commands(command_list);
	children = malloc(sizeof(pid_t) * (n_commands + 1));
	if (!children)
		return (-1);
	i = 0;
	while (i < n_commands)
	{
		pid = fork();
		if (pid == 0)
			exit(execute_child(command_list[i], env, paths));
		else
			children[i] = pid;
		i++;
	}
	i = 0;
	to_return = 2;
	empty = 1;
	status = 0;
	while (to_return == 2 && i < n_commands && empty != -1)
	{
		empty = waitpid(children[i], &status, 0);
		if (WIFEXITED(status))
		{
			command_list[i]-> returned_output = WEXITSTATUS(status);
			to_return = WEXITSTATUS(status);
		}
		i++;
	}
	ft_kill_children(children, n_commands);
	free(children);
	return (to_return);
}
