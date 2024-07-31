/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexigar <alexigar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 10:01:14 by alexigar          #+#    #+#             */
/*   Updated: 2024/07/31 10:52:08 by alexigar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_commands(t_command **command_list)
{
	int	i;

	i = 0;
	while (command_list[i])
	{
		if (command_list[i]-> args)
		{
			free(command_list[i]-> args);
			command_list[i]-> args = NULL;
		}
		if (command_list[i]-> file_input != 1)
			close(command_list[i]-> file_input);
		if (command_list[i]-> file_output != 1)
			close(command_list[i]-> file_output);
		if (command_list[i]-> path)
		{
			free(command_list[i]-> path);
			command_list[i]-> path = NULL;
		}
		if (command_list[i]-> string_output)
		{
			free(command_list[i]-> string_output);
			command_list[i]-> string_output = NULL;
		}
		free(command_list[i]);
		i++;
	}
	free(command_list);
}

t_command	**parser(char **tokens, t_list **env)
{
	int			i;
	int			j;
	int			k;
	int			n_tokens;
	t_command	*current_command;
	t_command	**command_list;
	int			pipefd[2];
	int			heredoc[2];
	char		*line;

	i = 0;
	j = 0;
	k = 0;
	n_tokens = count_nbr_tokens(tokens);
	command_list = malloc(sizeof(t_command *) * (n_tokens + 1));
	if (!command_list)
		return (NULL);
	while (tokens[i])
	{
		current_command = malloc(sizeof(t_command));
		if (!current_command)
		{
			free_commands(command_list);
			return (NULL);
		}
		current_command -> args = malloc(sizeof(char *) * (n_tokens - i + 1));
		current_command -> command = NULL;
		current_command -> string_output = NULL;
		current_command -> path = ft_get_var_env(env, "..PWD");
		if (!current_command -> path)
		{
			free_commands(command_list);
			return (NULL);
		}
		current_command -> redir1 = 0;
		current_command -> redir2 = 0;
		current_command -> file_input = 1;
		current_command -> file_output = 1;
		dup2(STDIN_FILENO, current_command -> file_input);
		dup2(STDOUT_FILENO, current_command -> file_output);
		if (j > 0 && command_list[j - 1]-> piped)
		{
			if (pipe(pipefd) == 0)
			{
				command_list[j - 1]-> file_output = pipefd[1];
				current_command -> file_input = pipefd[0];
			}
			else
			{
				free_commands(command_list);
				return (NULL);
			}
		}
		else
			current_command -> file_input = 1;
		current_command -> file_output = 1;
		current_command -> piped = 0;
		if (!(current_command -> args))
		{
			free_commands(command_list);
			return (NULL);
		}
		current_command -> index = j;
		while (tokens[i] && tokens[i][0] != '|'
			&& tokens[i][0] != '>' && tokens[i][0] != '<')
		{
			if (!(current_command -> command))
			{
				current_command -> command = tokens[i];
			}
			current_command -> args[k] = tokens[i];
			k++;
			if (tokens[i][0] == '/' && current_command -> command[0] != '/')
			{
				if (current_command -> file_input != 1
					&& current_command -> file_output != 1)
				{
					free_commands(command_list);
					return (NULL);
				}
				if (current_command -> file_output)
				{
					current_command -> file_input
						= current_command -> file_output;
				}
				current_command -> file_output
					= open(tokens[i], O_WRONLY | O_CREAT, 0644);
			}
			i++;
			if (!tokens[i])
			{
				current_command -> args[k] = NULL;
				command_list[j] = current_command;
				command_list[++j] = NULL;
				return (command_list);
			}
		}
		current_command -> args[k] = NULL;
		if (tokens[i])
		{
			while (tokens[i][0] == '>')
			{
				if (tokens[i][1] == '>')
					current_command -> redir2 = 2;
				else
					current_command -> redir2 = 1;
				i++;
				while (tokens[i][0] == '>')
				{
					close(current_command -> file_output);
					if (current_command -> redir2 == 1)
					{
						unlink(tokens[i]);
						current_command -> file_output
							= open(tokens[i], O_WRONLY | O_CREAT, 0644);
					}
					else
					{
						current_command -> file_output
							= open(tokens[i],
								O_WRONLY | O_CREAT | O_APPEND, 0644);
					}
					i++;
				}
				if (current_command -> redir2 == 1)
				{
					unlink(tokens[i]);
					current_command -> file_output
						= open(tokens[i], O_WRONLY | O_CREAT, 0644);
				}
				else
				{
					current_command -> file_output
						= open(tokens[i], O_WRONLY | O_CREAT | O_APPEND, 0644);
				}
				if (++i >= n_tokens)
				{
					current_command -> args[k] = NULL;
					command_list[j] = current_command;
					command_list[++j] = NULL;
					return (command_list);
				}
			}
			while (tokens[i][0] == '<')
			{
				if (tokens[i][1] == '<')
					current_command -> redir1 = 2;
				else
					current_command -> redir1 = 1;
				i++;
				if (current_command -> redir1 == 1)
				{
					if (current_command -> file_input != -1)
					{
						if (current_command -> file_input != 1)
						{
							close(current_command -> file_input);
						}
						current_command -> file_input
							= open(tokens[i], O_RDONLY);
					}
				}
				else
				{
					if (pipe(heredoc) == 0)
					{
						line = readline(" heredoc> ");
						if (!line)
							break ;
						while (ft_strncmp(tokens[i],
								line, ft_strlen(tokens[i])))
						{
							write(heredoc[1], line, ft_strlen(line));
							write(heredoc[1], "\n", 1);
							free(line);
							line = readline(" heredoc> ");
							if (!line)
								break ;
						}
						i++;
						close(heredoc[1]);
						free(line);
						current_command -> file_input = heredoc[0];
					}
					else
					{
						free_commands(command_list);
						return (NULL);
					}
				}
				if (++i >= n_tokens)
				{
					current_command -> args[k] = NULL;
					command_list[j] = current_command;
					command_list[++j] = NULL;
					return (command_list);
				}
			}
			if (tokens[i][0] == '|')
			{
				current_command -> piped = 1;
				i++;
			}
		}
		current_command -> args[k] = NULL;
		command_list[j] = current_command;
		j++;
		k = 0;
	}
	command_list[j] = NULL;
	return (command_list);
}
