/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexigar <alexigar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 10:01:14 by alexigar          #+#    #+#             */
/*   Updated: 2024/06/07 12:57:32 by alexigar         ###   ########.fr       */
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

t_command **parser(char **tokens)
{
	int			i;
	int			j;
	int			k;
	int			n_tokens; //Lo suyo seria pasarme esto que es el numero de tokens
	t_command 	*current_command;
	t_command	**command_list;

	i = 0;
	j = 0;
	k = 0;
	n_tokens = count_tokens(tokens); //Y asi me ahorro esta funcion
	command_list = malloc(sizeof(t_command *) * (n_tokens + 1));
	while (tokens[i])
	{
		current_command = malloc(sizeof(t_command));
		if (!current_command)
			return (NULL);
		current_command -> args = malloc(sizeof(char *) * (n_tokens - i + 1));
		current_command -> command = NULL;
		current_command -> path[0] = '\0';
		current_command -> redir1 = 0;
		current_command -> redir2 = 0;
		current_command -> file = 0;
		if (!command_list || !(current_command -> args))
			return (NULL); //Va a haber que liberar todo y tirar error
		current_command -> index = j;
		while (tokens[i][0] != '|' && tokens[i][0] != '>' && tokens[i][0] != '<')
		{
			printf("Ha entrado en el bucle\n");
			if (!(current_command -> command))
			{
				printf("No hay comando\n");
				current_command -> command = tokens[i];
				printf("Inicializa comando\n");
			}
			else
			{
				printf("Nuevo argumento del comando\n");
				current_command -> args[k] = tokens[i];
				printf("Lo ha metido bien\n");
				k++;
			}
			i++;
		}
		current_command -> args[k] = NULL;
		if (current_command -> path[0] == '\0')
			getcwd(current_command -> path, sizeof(current_command -> path));
		printf("cwd %s\n", current_command -> path);
		if (tokens[i])
		{
			if (tokens[i][0] == '>')
			{
				printf("Hay redireccion\n");
				if (tokens[i][1] == '>')
					current_command -> redir2 += 2;
				else
					current_command -> redir2 += 1;
				i++;
				printf("La ha manejado\n");
			}
			if (tokens[i][0] == '<')
			{
				printf("Hay redireccion\n");
				if (tokens[i][1] == '<')
					current_command -> redir1 -= 2;
				else
					current_command -> redir1 -= 1;
				i++;
				printf("La ha manejado\n");
			}
			if (tokens[i][0] == '/')
			{
				printf("Ha encontrado un archivo\n");
				current_command -> file = open(tokens[i], O_RDWR); //No se si voy a leer o a escribir asi que le pongo que pueda hacer las dos por si acaso
				i++;
				printf("Lo ha manejado\n");
			}
		}
		//El resto de parametros los hace el ejecutor imagino
		printf("Ha salido del bucle\n");
		command_list[j] = current_command;
		printf("Ha metido el comando en la lista\n");
		j++;
		k = 0;
	}
	return (command_list);
}

int main(void) //Main de prueba
{
	char *tokens[] = {"command", "hola", "esto", "es", "un comando", ">>", "command", "pasan", "cosas"};
	t_command **list = parser((char **)tokens);
	printf("Index %d\nCommand %s\nPath %s\nString %s\nRedir1 %d\nRedir2 %d\nFile %d\n", list[0] -> index, (char *)(list[0] -> command), list[0] -> path, list[0] -> args[0],
	list[0] -> redir1, list[0] -> redir2, list[0] -> file);
}