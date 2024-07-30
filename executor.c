/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexigar <alexigar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 10:12:42 by alexigar          #+#    #+#             */
/*   Updated: 2024/07/30 12:17:59 by alexigar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    *read_all(int fd)
{
    char    *aux;
    char    *aux2;
    char    *to_return;

    aux = get_next_line(fd);
	if (!aux)
		return (NULL);
    aux2 = aux;
    while (aux2)
    {
        aux2 = get_next_line(fd);
        if (aux2)
        {
			to_return = ft_strjoin(aux, aux2);
            free(aux);
            aux = to_return;
        }
    }
    free(aux2);
    return (to_return);
}

int try_call(char **paths, t_command *com)
{
	int     	i;
	char        *function_call;
	char        *aux;
	//int         pipefd[2];
	pid_t       pid;
	int         returned;
	struct stat buf;

	i = 0;
	//printf("Va a intentar ejecutar %s\n", com -> command);
	while (paths[i])
	{
		if (com -> command[0] == '/')
			function_call = com -> command;
		else
		{
			aux = ft_strjoin(paths[i], "/"); //Malloc
			if (!aux)
				return (-1);
			function_call = ft_strjoin(aux, com -> command); //Malloc
			free(aux);
		}
		if (!function_call )//|| pipe(pipefd) == -1)
			return (-1); //Salida error
		if (stat(function_call, &buf) == 0 && S_ISREG(buf.st_mode) && (buf.st_mode & S_IXUSR))
		{
			//Si llamo a execve hay que hacerlo en un fork aparte y pausar el programa principal
			pid = fork();
			if (pid < 0)
			{
				perror("Fork failed");
				if (function_call != com->command)
					free(function_call);
				is_executing = 0;
				exit(-1);
			}
			if (pid == 0) 
			{
				if (com -> file_input!= 1)
				{
					dup2(com -> file_input, STDIN_FILENO);
					close(com->file_input);
				}
				if (com -> file_output!= 1)
				{
					dup2(com -> file_output, STDOUT_FILENO);
					close(com->file_output);
				}
				if (execve(function_call, com -> args, NULL) == -1)
				{
					perror("Command not found");
					if (function_call != com->command)
						free(function_call);
					is_executing = 0;
					exit(EXIT_FAILURE);
				}
				exit(EXIT_SUCCESS);
				
			}
			else
			{
				waitpid(pid, &returned, 0);
				com -> returned_output = WEXITSTATUS(returned);
				//printf("%s ha devuelto %d\n", com->command, com->returned_output);
				if (function_call != com->command)
					free(function_call);
				if (com -> file_input != 1)
					close(com -> file_input);
				if (com -> file_output != 1)
					close(com -> file_output);
				return (com -> returned_output);
			}
		}
		else
		{
			if (function_call != com->command)
				free(function_call);
			function_call = NULL;
			i++;
		}
	}
	com -> returned_output = 127;
	perror("Error: command not found\n");
	if (function_call != com->command)
		free(function_call);
	return (com -> returned_output);
}

int executor(t_command **command_list, t_list **env)
{
	int                 i;
	char                *function_call;
	char                **paths;
	int                 to_return;
	int                 j;
	
	//printf("Ha llegado al ejecutor\n");
	i = 0;
	j = 0;
	to_return = 0;
	function_call = ft_get_var_env(env, "PATH");
	if (!function_call)
		return (-1);
	paths = ft_split(function_call, ':'); //malloc
	free(function_call);
	if (!paths)
	{
		return (-1); //salida error
	}
	is_executing = 1;
	//Split y funcion para intentar llamar a las funciones
	while (command_list[i])
	{
		if (!command_list[i] -> command)
		{
			is_executing = 0;
			return(0); //Tendria que hacer alguna otra cosa entiendo
		}
		if (command_list[i] -> file_input == -1)
		{
			perror("Error");
			to_return = 1;
		}
		//TODO Manejar bien pipes y redirecciones
		//Si el comando es un built-in se ejecuta el built-in, si no intento llamar a execve
		else if ((ft_strncmp(command_list[i] -> command, "echo", 4) == 0 && ft_strlen(command_list[i] -> command) == 4)
		|| (ft_strncmp(command_list[i] -> command, "cd", 2) == 0 && ft_strlen(command_list[i] -> command) == 2)
		|| (ft_strncmp(command_list[i] -> command, "pwd", 3) == 0 && ft_strlen(command_list[i] -> command) == 3)
		|| (ft_strncmp(command_list[i] -> command, "export", 6) == 0 && ft_strlen(command_list[i] -> command) == 6)
		|| (ft_strncmp(command_list[i] -> command, "unset", 5) == 0 && ft_strlen(command_list[i] -> command) == 5)
		|| (ft_strncmp(command_list[i] -> command, "env", 3) == 0 && ft_strlen(command_list[i] -> command) == 3)
		|| (ft_strncmp(command_list[i] -> command, "exit", 4) == 0 && ft_strlen(command_list[i] -> command) == 4))
		{
			to_return = ft_build_int(command_list[i], env);
			if (to_return != 0)
			{
				is_executing = 0;
				return (to_return); //Si se ha cambiado a algo que no es 0 devuelvo porque ha fallado algo
			}
		}
		else
		{
			//printf("Va a intentar ejecutar %s\n", command_list[i] -> command);
			to_return = try_call(paths, command_list[i]);
		}
		if (command_list[i] -> returned_output == -1)
		{
			is_executing = 0;
			//Tiro error suave si ha fallado
			return (1); //O el codigo de error que sea
		}
		i++;
	}
	is_executing = 0;
	ft_free_char(paths);
	return (to_return); //Si todo ha ido bien devuelvo 0
}