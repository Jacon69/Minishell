/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexigar <alexigar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 10:12:42 by alexigar          #+#    #+#             */
/*   Updated: 2024/06/17 12:09:18 by alexigar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "environment.h"

int execute_builtin(t_command *com, t_list *env)
{
    //Ejecuto el comando que sea y devuelvo
    if (ft_strncmp(com -> command, "echo", ft_strlen(com -> command)) == 0)
        return (ft_built_echo(*com));
    else if (ft_strncmp(com -> command, "cd", ft_strlen(com -> command)) == 0)
        return (ft_built_cd(*com, *env));
    else if (ft_strncmp(com -> command, "pwd", ft_strlen(com -> command)) == 0)
        return (ft_built_pwd(*com));
    else if (ft_strncmp(com -> command, "export", ft_strlen(com -> command)) == 0)
        return (ft_built_export(*com, *env));
    else if (ft_strncmp(com -> command, "unset", ft_strlen(com -> command)) == 0)
        return (ft_built_unset(*com, *env));
    else if (ft_strncmp(com -> command, "env", ft_strlen(com -> command)) == 0)
        return (ft_built_env(*com, *env));
    else if (ft_strncmp(com -> command, "exit", ft_strlen(com -> command)) == 0)
    {
		free_commands(com);
		printf("exit\n");
        exit(EXIT_SUCCESS);
    }
    return (0);
}

int try_call(char **paths, t_command *com)
{
    int 	i;
	char	*function_call;

    i = 0;
    while (paths[i])
    {
		function_call = ft_strjoin(paths[i], com -> command);
		if (!function_call)
			return (-1); //Salida error
        //Si llamo a execve hay que hacerlo en un fork aparte y pausar el programa principal
        if (fork() == 0)
            com -> returned_output = execve(function_call, com -> args, NULL); //Aqui va a haber que cambiar cosas porque execve devuelve int
        else
            wait(NULL);
        //USar un archivo temporal para almacenar los strings
		if (com -> returned_output != 127)
			break ;
		i++;
    }
	return (com -> returned_output);
}

int executor(t_command **command_list, t_list *env)
{
    int     i;
    char    *function_call;
    char    **paths;
    int     to_return;

    i = 0;
    to_return = 0;
    command_list[i] -> input = NULL;
    function_call = ft_get_var_env(**env, "PATH");
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
        function_call = ft_strjoin("/bin/", command_list[i] -> command); //Esta linea se cambia
        if (!function_call)
        {
            free_commands(command_list);
            exit(EXIT_FAILURE);
        }
        //Si el comando es un built-in se ejecuta el built-in, si no intento llamar a execve
        if (ft_strncmp(command_list[i] -> command, "echo", ft_strlen(command_list[i] -> command)) != 0
        && ft_strncmp(command_list[i] -> command, "cd", ft_strlen(command_list[i] -> command)) != 0
        && ft_strncmp(command_list[i] -> command, "pwd", ft_strlen(command_list[i] -> command)) != 0
        && ft_strncmp(command_list[i] -> command, "export", ft_strlen(command_list[i] -> command)) != 0
        && ft_strncmp(command_list[i] -> command, "unset", ft_strlen(command_list[i] -> command)) != 0
        && ft_strncmp(command_list[i] -> command, "env", ft_strlen(command_list[i] -> command)) != 0
        && ft_strncmp(command_list[i] -> command, "exit", ft_strlen(command_list[i] -> command)) != 0)
        {
           try_call(paths, command_list[i]);
        }
        else
        {
            to_return = execute_builtin(command_list[i], env);
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