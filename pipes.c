int	check_piped(t_command **list, int j, t_command **command)
{
	int	pipefd[2];

	if (j > 0 && (list[j - 1])-> piped)
	{
		if (pipe(pipefd) == 0)
		{
			(list[j - 1])-> file_output = pipefd[1];
			(*command)-> file_input = pipefd[0];
		}
		else
			return (0);
	}
	return (1);
}

void	check_pipe_token(char *token, t_command **command, int *i)
{
	if (token[0] == '|')
	{
		(*command)-> piped = 1;
		*i += 1;
	}
}
