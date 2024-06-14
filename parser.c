/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexigar <alexigar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 10:01:14 by alexigar          #+#    #+#             */
/*   Updated: 2024/06/14 10:29:47 by alexigar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_tokens(char **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
		i++;
	return (i);
}

void	free_commands(t_command **command_list)
{
	int	i;

	i = 0;
	while (command_list[i])
	{
		if (command_list[i] -> args)
			free(command_list[i] -> args);
		if (command_list[i] -> file_input != 1)
			close(command_list -> file_input);
		if (command_list[i] -> file_output != 1)
			close(command_list -> file_output);
		free(command_list[i]);
		i++;
	}
	free(command_list);
}

t_command **parser(char **tokens) //A esta funcion le tiene que llegar NULL como ultimo token, si no va a tirar segfault
{
	int			i;
	int			j;
	int			k;
	int			n_tokens;
	t_command 	*current_command;
	t_command	**command_list;

	if (!tokens || !tokens[0])
		exit(EXIT_FAILURE); //salida error
	i = 0;
	j = 0;
	k = 0;
	n_tokens = count_tokens(tokens);
	command_list = malloc(sizeof(t_command *) * (n_tokens + 1));
	if (!command_list)
		exit(EXIT_FAILURE); //salida error
	while (tokens[i])
	{
		current_command = malloc(sizeof(t_command));
		if (!current_command)
		{
			free_commands(command_list);
			exit(EXIT_FAILURE); //salida error
		}
		current_command -> args = malloc(sizeof(char *) * (n_tokens - i + 1));
		current_command -> command = NULL;
		current_command -> path[0] = '\0';
		current_command -> redir1 = 0;
		current_command -> redir2 = 0;
		current_command -> file_output = 1; //Por defecto se escribe en la consola
		current_command -> piped = 0;
		if (!(current_command -> args))
		{
			free_commands(command_list);
			exit(EXIT_FAILURE); //salida error
		}
		current_command -> index = j;
		while (tokens[i][0] != '|' && tokens[i][0] != '>' && tokens[i][0] != '<')
		{
			if (!(current_command -> command) && tokens[i][0] != '/')
			{
				current_command -> command = tokens[i];
			}
			else
			{
				current_command -> args[k] = tokens[i];
				k++;
				if (tokens[i][0] == '/')
				{
					if (current_command -> file_input && current_command -> file_output)
					{
						free_commands(command_list);
						return (NULL);
					}
					if (current_command -> file_output)
						current_command -> file_input = current_command -> file_output;
					current_command -> file_output = open(tokens[i], O_WRONLY | O_CREAT, 0644);
				}
			}
			i++;
			if (!tokens[i])
				break ;
		}
		current_command -> args[k] = NULL;
		if (current_command -> path[0] == '\0')
			getcwd(current_command -> path, sizeof(current_command -> path));
		if (tokens[i])
		{
			if (tokens[i][0] == '>')
			{
				if (tokens[i][1] == '>')
					current_command -> redir2 += 2;
				else
					current_command -> redir2 += 1;
				i++;
				current_command -> file_output = open(tokens[i], O_WRONLY | O_CREAT, 0644);
					i++;
				//TODO cada > que encuentre machaca el archivo anterior
				//else manejar el archivo
				//TODO manejar input invalido
			}
			if (tokens[i][0] == '<')
			{
				if (tokens[i][1] == '<')
					current_command -> redir1 -= 2; //Este tiene que pedir inputs hasta que encuentre el siguiente argumento (input desde la consola, heredoc)
				else
					current_command -> redir1 -= 1; //Este coge un archivo (input desde el archivo)
				i++;
				//else manejar el archivo y tirar error blando si hay algo que no cuadra
				//TODO manejar input invalido
			}
			if (tokens[i][0] == '|') //El output del comando va a ir al input del siguiente comando
			{
				current_command -> piped = 1;
				i++;
			}
		}
		//El resto de parametros los hace el ejecutor
		command_list[j] = current_command;
		j++;
		k = 0;
	}
	return (command_list);
}

int main(void) //Main de prueba
{
	char *tokens[] = {"command", "hola", "esto", "es", "un comando", ">>", "/home/alexigar/Desktop/Minishell/holiwi.txt", "command", "asdasd", NULL};
	t_command **list = parser((char **)tokens);
	if (list)
		printf("Index %d\nCommand %s\nPath %s\nString %s\nRedir1 %d\nRedir2 %d\nFile %d\n", list[0] -> index, (char *)(list[0] -> command), list[0] -> path, list[0] -> args[0],
	list[0] -> redir1, list[0] -> redir2, list[1] -> file);
}