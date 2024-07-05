/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexigar <alexigar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 10:12:42 by alexigar          #+#    #+#             */
/*   Updated: 2024/07/05 12:10:58 by alexigar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    *read_all(int fd)
{
    char    *to_return;
    char    *aux;
    char    *aux2;

    to_return = get_next_line(fd);
    aux = to_return;
    while (aux)
    {
        aux = get_next_line(fd);
        if (aux)
        {
            aux2 = ft_strjoin(to_return, aux);
            free(to_return);
            to_return = aux2;
        }
    }
    //printf("%s\n", aux2);
    //free(aux2);
    free(aux);
    return (aux2);
    //return (to_return);
}

int try_call(char **paths, t_command *com)
{
    int     	i;
	char        *function_call;
    char        *aux;
    int         pipefd[2];
    pid_t       pid;
    int         returned;
    struct stat buf;

    /*for (int j = 0; com -> args[j] != NULL; j++)
        printf("%s\n", com -> args[j]);*/
    i = 0;
    printf("Va a intentar ejecutar %s\n", com -> command);
    while (paths[i])
    {
        aux = ft_strjoin(paths[i], "/");
        if (!aux)
            return (-1);
		function_call = ft_strjoin(aux, com -> command);
        free(aux);
        //printf("%s\n", function_call);
		if (!function_call || pipe(pipefd) == -1)
            return (-1); //Salida error
        if (stat(function_call, &buf) == 0 && S_ISREG(buf.st_mode) && (buf.st_mode & S_IXUSR))
        {
            //Si llamo a execve hay que hacerlo en un fork aparte y pausar el programa principal
            pid = fork();
            if (pid < 0)
            {
                perror("Fork failed");
                free(function_call);
                exit(-1);
            }
            if (pid == 0)
            {
                close(pipefd[0]);
                if (com -> file_input != 1)
                    printf("%s\n", com -> args[2]);
                dup2(pipefd[1], STDOUT_FILENO);
                dup2(pipefd[1], STDERR_FILENO);
                if (execve(function_call, com -> args, NULL) == -1)
                {
                    perror("Command not found");
                    free(function_call);
                    close(pipefd[1]);
                    exit(EXIT_FAILURE);
                }
                else
                    exit(EXIT_SUCCESS);
            }
            else
            {
                close(pipefd[1]);
                waitpid(pid, &returned, 0);
                com -> returned_output = WEXITSTATUS(returned);
                printf("%s ha devuelto %d\n", com -> command, com -> returned_output);
                free(function_call);
                function_call = NULL;
                com -> string_output = read_all(pipefd[0]);
                printf("%d\n", com -> file_output);
                /*if (com -> input)
                    printf("%s\n", com -> input);*/
                if (com -> string_output)
                    write(com -> file_output, com -> string_output, ft_strlen(com -> string_output));
                return (com -> returned_output);
            }
        }
        else
            i++;
    }
    com -> returned_output = 127;
    printf("Error: command not found\n");
    free(function_call);
    return (com -> returned_output);
}

int executor(t_command **command_list, t_list **env)
{
    int     i;
    char    *function_call;
    char    **paths;
    int     to_return;
    int     j;

    //printf("Ha llegado al ejecutor\n");
    i = 0;
    j = 0;
    to_return = 0;
    command_list[i] -> input = NULL;
    function_call = ft_get_var_env(env, "PATH");
    if (!function_call)
        return (-1);
    paths = ft_split(function_call, ':'); //malloc
    if (!paths)
    {
        free(function_call);
        return (-1); //salida error
    }
    //Split y funcion para intentar llamar a las funciones
    while (command_list[i])
    {
        if (!command_list[i] -> command)
        {
            free(function_call);
            return(0); //Tendria que hacer alguna otra cosa entiendo
        }
        //TODO Manejar bien pipes y redirecciones
        //Si el comando es un built-in se ejecuta el built-in, si no intento llamar a execve
        if (ft_strncmp(command_list[i] -> command, "echo", ft_strlen(command_list[i] -> command)) != 0
        && ft_strncmp(command_list[i] -> command, "cd", ft_strlen(command_list[i] -> command)) != 0
        && ft_strncmp(command_list[i] -> command, "pwd", ft_strlen(command_list[i] -> command)) != 0
        && ft_strncmp(command_list[i] -> command, "export", ft_strlen(command_list[i] -> command)) != 0
        && ft_strncmp(command_list[i] -> command, "unset", ft_strlen(command_list[i] -> command)) != 0
        && ft_strncmp(command_list[i] -> command, "env", ft_strlen(command_list[i] -> command)) != 0
        && ft_strncmp(command_list[i] -> command, "exit", ft_strlen(command_list[i] -> command)) != 0)
        {
            //printf("Va a intentar ejecutar %s\n", command_list[i] -> command);
            to_return = try_call(paths, command_list[i]);
        }
        else
        {
            free(function_call);
            function_call = NULL;
            /*printf("Va a ejecutar ft_build\n");
            printf("%s\n", command_list[i] -> path);*/
            to_return = ft_build_int(command_list[i], env);
            //printf("Ha ejecutado ft_build\n");
            if (to_return != 0)
            {
                free(function_call);
                return (to_return); //Si se ha cambiado a algo que no es 0 devuelvo porque ha fallado algo
            }
        }
        if (command_list[i] -> returned_output == -1)
        {
            //Tiro error suave si ha fallado
            free(function_call);
            return (1); //O el codigo de error que sea
        }
        if (command_list[i] -> piped == 1)
        {
            command_list[i + 1] -> input = command_list[i] -> string_output;
            while (command_list[i + 1] -> args[j])
                j++;
            command_list[i + 1] -> args[j] = command_list[i] -> string_output;
            //command_list[i + 1] -> args[j] = ft_itoa(command_list[i] -> file_output);
        }
        i++;
        //printf("Input\n\n %s\n", command_list[i] -> input);
     /*   free(function_call);
        function_call = NULL;*/
    }
    return (to_return); //Si todo ha ido bien devuelvo 0
}