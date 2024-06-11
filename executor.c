/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexigar <alexigar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 10:12:42 by alexigar          #+#    #+#             */
/*   Updated: 2024/06/11 12:23:18 by alexigar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    executor(t_command **command_list)
{
    int     i;
    char    *function_call;

    i = 0;
    command_list[i] -> input = NULL;
    function_call = NULL;
    while (command_list[i])
    {
        function_call = ft_strjoin("/bin/", command_list[i] -> command);
        if (!function_call)
        {
            free_commands(command_list);
            exit(EXIT_FAILURE);
        }
        //Si el comando es un built-in se ejecuta el built-in (TODO), si no intento llamar a execve
        if (fork() == 0)
            command_list[i] -> returned_output = execve(function_call, command_list[i] -> args, NULL); //Aqui va a haber que cambiar cosas porque execve devuelve int
        else
            wait(NULL);
        //TODO pausar el proceso principal mientras el otro este activo
        if (command_list[i] -> returned_output == -1)
        {
            //Tiro error suave si ha fallado
            free_commands(command_list);
            return ;
        }
        if (command_list[i] -> piped == 1)
            command_list[i + 1] -> input = command_list[i] -> string_output;
        i++;
        free(function_call);
        function_call = NULL;
    }
    free_commands(command_list);
}