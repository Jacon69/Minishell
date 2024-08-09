t_command	**free_commands(t_command **command_list)
{
	int	i;

	i = 0;
	while (command_list[i])
	{
		if (command_list[i]-> args)
			free(command_list[i]-> args);
		if (command_list[i]-> file_input != 1)
			close(command_list[i]-> file_input);
		if (command_list[i]-> file_output != 1)
			close(command_list[i]-> file_output);
		if (command_list[i]-> path)
			free(command_list[i]-> path);
		if (command_list[i]-> string_output)
			free(command_list[i]-> string_output);
		free(command_list[i]);
		i++;
	}
	free(command_list);
	return (NULL);
}

t_command	*new_command(char **tokens, t_list **env, int i, int j)
{
	t_command	*current_command;
	int			n_tokens;

	n_tokens = count_nbr_tokens(tokens);
	current_command = malloc(sizeof(t_command));
	if (!current_command)
		return (NULL);
	current_command -> args = malloc(sizeof(char *) * (n_tokens - i + 1));
	current_command -> command = NULL;
	current_command -> string_output = NULL;
	current_command -> path = ft_get_var_env(env, "..PWD");
	if (!current_command -> path || !current_command -> args)
		return (NULL);
	current_command -> redir1 = 0;
	current_command -> redir2 = 0;
	current_command -> file_input = 1;
	current_command -> file_output = 1;
	current_command -> piped = 0;
	current_command -> index = j;
	current_command -> returned_output = 0;
	dup2(STDIN_FILENO, current_command -> file_input);
	dup2(STDOUT_FILENO, current_command -> file_output);
	return (current_command);
}

t_command	**end_list(t_command *command, t_command **list, int *dup)
{
	command -> args[dup[1]] = NULL;
	list[dup[0]] = command;
	list[++dup[0]] = NULL;
	dup[3] = 1;
	return (list);
}

void	fill_args(char **tokens, int *i, t_command **com, int *k)
{
	while (tokens[*i] && tokens[*i][0] != '|'
		&& tokens[*i][0] != '>' && tokens[*i][0] != '<')
	{
		if (!((*com)-> command))
			(*com)-> command = tokens[*i];
		(*com)-> args[*k] = tokens[*i];
		*k += 1;
		(*com)-> args[*k] = NULL;
		*i += 1;
		if (!tokens[*i])
			break ;
	}
}

t_command	**ft_init_com_list(char **tokens)
{
	t_command	**list;
	int			nbr_tokens;
	int			i;

	nbr_tokens = count_nbr_tokens(tokens);
	list = malloc(sizeof(t_command *) * (nbr_tokens + 1));
	if (!list)
		return (NULL);
	i = 0;
	while (i < nbr_tokens)
	{
		list[i] = NULL;
		i++;
	}
	return (list);
}
