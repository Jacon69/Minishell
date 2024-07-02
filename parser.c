/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexigar <alexigar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 10:01:14 by alexigar          #+#    #+#             */
/*   Updated: 2024/07/02 12:25:26 by alexigar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_commands(t_command **command_list)
{
	int	i;

	i = 0;
	while (command_list[i])
	{
		if (command_list[i] -> args)
			free(command_list[i] -> args);
		if (command_list[i] -> file_input != 1)
			close(command_list[i] -> file_input);
		if (command_list[i] -> file_output != 1)
			close(command_list[i] -> file_output);
		if (command_list[i] -> path)
			free(command_list[i] -> path);
		free(command_list[i]);
		i++;
	}
	free(command_list);
}

t_command **parser(char **tokens, t_list **env) //A esta funcion le tiene que llegar NULL como ultimo token, si no va a tirar segfault
{
	int			i;
	int			j;
	int			k;
	int			n_tokens;
	t_command 	*current_command;
	t_command	**command_list;
	int			pipefd[2];

	i = 0;
	j = 0;
	k = 0;
	n_tokens = count_nbr_tokens(tokens);
	command_list = malloc(sizeof(t_command *) * (n_tokens + 1));
	if (!command_list)
		return (NULL); //salida error
	while (tokens[i])
	{
		current_command = malloc(sizeof(t_command)); //malloc
		if (!current_command)
		{
			free_commands(command_list);
			return (NULL); //salida error
		}
		current_command -> args = malloc(sizeof(char *) * (n_tokens - i + 1)); //malloc
		current_command -> command = NULL;
		current_command -> path = ft_get_var_env(env,"PWD"); //malloc
		if (!current_command -> path)
		{
			free_commands(command_list);
			return (NULL); //salida error
		}
		current_command -> redir1 = 0;
		current_command -> redir2 = 0;
		if (j > 0 && command_list[j - 1] -> piped)
		{
			if (pipe(pipefd) == 0)
			{
				command_list[j - 1] -> file_output = pipefd[1]; //1 escritura
				current_command -> file_input = pipefd[0]; //0 lecture
			}
			else
			{
				free_commands(command_list);
				return (NULL);
			}
		}
		else
			current_command -> file_input = 1;
		current_command -> file_output = 1; //Por defecto se escribe en la consola
		current_command -> piped = 0;
		if (!(current_command -> args))
		{
			free_commands(command_list);
			return (NULL); //salida error
		}
		current_command -> index = j;
		while (tokens[i] && tokens[i][0] != '|' && tokens[i][0] != '>' && tokens[i][0] != '<')
		{
			if (!(current_command -> command) && tokens[i][0] != '/')
			{
				current_command -> command = tokens[i];
			}
			//else
			//{
				current_command -> args[k] = tokens[i];
				k++;
				if (tokens[i][0] == '/')
				{
					if (current_command -> file_input != 1 && current_command -> file_output != 1)
					{
						free_commands(command_list);
						return (NULL); //salida error
					}
					if (current_command -> file_output)
						current_command -> file_input = current_command -> file_output;
					current_command -> file_output = open(tokens[i], O_WRONLY | O_CREAT, 0644);
				}
			//}
			i++;
			if (!tokens[i])
			{
				command_list[j] = current_command;
				return (command_list);
				//break ;
			}
		}
		current_command -> args[k] = NULL;
		/*if (!current_command -> path)
		{
			current_command -> path = ft_get_var_env(env,"PWD"); //malloc
			if (!current_command -> path)
			{
				free_commands(command_list);
				return (NULL); //salida error
			}
		}*/
		if (tokens[i])
		{
			if (tokens[i][0] == '>')
			{
				if (tokens[i][1] == '>')
					current_command -> redir2 += 2;
				else
					current_command -> redir2 += 1;
				i++;
				while (tokens[i][0] == '>')
				{
					close(current_command -> file_output);
					if (current_command -> redir2 == 1)
					{
						unlink(tokens[i]);
						current_command -> file_output = open(tokens[i], O_WRONLY | O_CREAT, 0644);
					}
					else
						current_command -> file_output = open(tokens[i], O_WRONLY | O_CREAT | O_APPEND, 0644);
					i++;
				}
				if (current_command -> redir2 == 1)
				{
					unlink(tokens[i]);
					current_command -> file_output = open(tokens[i], O_WRONLY | O_CREAT, 0644);
				}
				else
					current_command -> file_output = open(tokens[i], O_WRONLY | O_CREAT | O_APPEND, 0644);
				if (++i >= n_tokens)
				{
					command_list[j] = current_command;
					return (command_list);
				}
			}
			if (tokens[i][0] == '<')
			{
				if (tokens[i][1] == '<')
					current_command -> redir1 -= 2; //Este tiene que pedir inputs hasta que encuentre el siguiente argumento (input desde la consola, heredoc)
				else
					current_command -> redir1 -= 1; //Este coge un archivo (input desde el archivo)
				i++;
				if (current_command -> redir1 == 1)
				{
					current_command -> file_input = open(tokens[i], O_RDONLY);
					current_command -> input = read_all(current_command -> file_input); //malloc
					close(current_command -> file_input);
					if (!(current_command -> input))
					{
						free_commands(command_list);
						return (NULL); //salida error
					}
				}
				else
				{
					//TODO
				}
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