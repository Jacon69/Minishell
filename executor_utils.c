/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaimecondea <jaimecondea@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 11:25:27 by alexigar          #+#    #+#             */
/*   Updated: 2024/08/07 07:11:26 by jaimecondea      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Checks if the command is a built-in as per the subject
*/
int	is_built_in(char *command)
{
	return ((ft_strncmp(command, "echo", 4) == 0
			&& ft_strlen(command) == 4)
		|| (ft_strncmp(command, "cd", 2) == 0
			&& ft_strlen(command) == 2)
		|| (ft_strncmp(command, "pwd", 3) == 0
			&& ft_strlen(command) == 3)
		|| (ft_strncmp(command, "export", 6) == 0
			&& ft_strlen(command) == 6)
		|| (ft_strncmp(command, "unset", 5) == 0
			&& ft_strlen(command) == 5)
		|| (ft_strncmp(command, "env", 3) == 0
			&& ft_strlen(command) == 3)
		|| (ft_strncmp(command, "exit", 4) == 0
			&& ft_strlen(command) == 4));
}

/*
Gets all paths in the environment variable PATH and
splits them to be readable by the program
*/
char	**get_paths(t_list **env)
{
	char	*function_call;
	char	**paths;

	function_call = ft_get_var_env(env, "PATH");
	if (!function_call)
		return (NULL);
	paths = ft_split(function_call, ':');
	free(function_call);
	if (!paths)
		return (NULL);
	return (paths);
}

/*
Gets the path which will be tried to execute
*/
char	*get_function_call(char *command, char *path)
{
	char	*aux;
	char	*function_call;

	aux = ft_strjoin(path, "/");
	if (!aux)
		return (NULL);
	function_call = ft_strjoin(aux, command);
	free(aux);
	if (!function_call)
		return (NULL);
	return (function_call);
}

/*
Frees the function call if it has been malloc'd
*/
void	free_function_call(t_command *com, char *function_call)
{
	if (function_call != com->command)
		free(function_call);
}

/*
Exits the program if a fork has failed
*/
void	fail_fork(t_command *com, char *function_call)
{
	perror("Fork failed");
	free_function_call(com, function_call);
	exit(-1);
}
