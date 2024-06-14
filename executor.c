/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexigar <alexigar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 10:12:42 by alexigar          #+#    #+#             */
/*   Updated: 2024/06/14 10:29:49 by alexigar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int try_call(char **paths, t_command *command)
{
    int i;

    i = 0;
    while (paths[i])
    {
        //TODO
    }
}

int executor(t_command **command_list, t_list **env) //Recibir variables de entornos
{
    int     i;
    char    *function_call;
    char    **paths;
    int     to_return;

    i = 0;
    to_return = 0;
    command_list[i] -> input = NULL;
    function_call = ft_get_var_env(env, "PATH"); //malloc
    if (!function_call)
        {
            free_commands(command_list);
            exit(EXIT_FAILURE);
        }
    paths = ft_split(function_call, ':'); //malloc
    if (!paths)
    {
        free_commands(command_list);
        exit(EXIT_FAILURE); //salida error
    }
    //Split y funcion para intentar llamar a las funciones
    while (command_list[i])
    {
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
            //Si llamo a execve hay que hacerlo en un fork aparte y pausar el programa principal
            if (fork() == 0)
                command_list[i] -> returned_output = execve(function_call, command_list[i] -> args, NULL); //Aqui va a haber que cambiar cosas porque execve devuelve int
            else
                wait(NULL);
            //USar un archivo temporal para almacenar los strings
        }
        else
        {
            to_return = ft_build_int(env, *(command_list[i]));
            if (to_return != 0)
                return (to_return); //Si se ha cambiado a algo que no es 0 devuelvo porque ha fallado algo
        }
        if (command_list[i] -> returned_output == -1)
        {
            //Tiro error suave si ha fallado
            free_commands(command_list);
            return (1); //O el codigo de error que sea
        }
        if (command_list[i] -> piped == 1)
            command_list[i + 1] -> input = command_list[i] -> string_output;
        i++;
        free(function_call);
        function_call = NULL;
    }
    free_commands(command_list);
    return (to_return); //Si todo ha ido bien devuelvo 0
}