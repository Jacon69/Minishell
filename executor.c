/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexigar <alexigar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 10:12:42 by alexigar          #+#    #+#             */
/*   Updated: 2024/08/16 11:19:22 by alexigar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Duplicates both input and output if necessary, then executes a command
*/
void	execute_function(t_command *com, char *function_call, t_list **env)
{
	if (com -> file_input != 1)
	{
		dup2(com -> file_input, STDIN_FILENO);
		close(com -> file_input);
	}
	if (com -> file_output != 1)
	{
		dup2(com -> file_output, STDOUT_FILENO);
		close(com -> file_output);
	}
	if (execve(function_call, com -> args, ft_get_env(env)) == -1)
	{
		perror("Command not found");
		if (function_call != com->command)
			free(function_call);
	}
}

/*
Father process. Waits for the function to finish,
closes what's needed and returns
*/
int	function_return(t_command *com, char *function_call, pid_t pid)
{
	int	returned;

	returned = 0;
	waitpid(pid, &returned, 0);
	com -> returned_output = WEXITSTATUS(returned);
	if (function_call != com->command)
		free(function_call);
	if (com -> file_input != 1)
		close(com -> file_input);
	if (com -> file_output != 1)
		close(com -> file_output);
	return (com -> returned_output);
}

/*
Creates a child process which will execute something.
The parent process waits by calling function_return.
Returns -1 if something fails
*/
int	fork_function(t_command *com, char *function_call, t_list **env)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		fail_fork(com, function_call);
	if (pid == 0)
		execute_function(com, function_call, env);
	else
		return (function_return(com, function_call, pid));
	return (-1);
}

/*
Tries to execute the command passed as an
argument using the paths in the other argument.
Returns -1 in case of a memory-related error and 127 if the command is not found
*/
int	try_call(char **paths, t_command *com, t_list **env)
{
	int			i;
	char		*function_call;
	struct stat	buf;

	i = 0;
	while (paths[i])
	{
		if (com -> command[0] == '/')
			function_call = com -> command;
		else
			function_call = get_function_call(com -> command, paths[i]);
		if (!function_call)
			return (-1);
		if (stat(function_call, &buf) == 0
			&& S_ISREG(buf.st_mode) && (buf.st_mode & S_IXUSR))
			return (fork_function(com, function_call, env));
		else
		{
			free_function_call(com, function_call);
			i++;
		}
	}
	printf("Error: command not found\n");
	return (127);
}

/*
For each command received as an argument, checks if it's a built-in or not
and proceeds accordingly by calling either ft_built_in or try_call.
Returns -1 in case of memory-related errors, 0 it there's nothing, 1
if something else fails and -2 if there is 'exit'
*/
int	executor(t_command **command_list, t_list **env)
{
	int		i;
	char	**paths;
	int		to_return;

	i = 0;
	to_return = 0;
	paths = get_paths(env);
	if (!paths)
		return (-1);
	while (command_list[i])
	{
		if (!command_list[i]-> command)
			return (0);
		if (command_list[i]-> file_input == -1)
			to_return = 1;
		else if (is_built_in(command_list[i]-> command))
			to_return = ft_build_int(command_list[i], env);
		else
			to_return = try_call(paths, command_list[i], env);
		if (command_list[i]-> returned_output == -1)
			return (1);
		i++;
	}
	ft_free_char(paths);
	return (to_return);
}
