/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_checks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexigar <alexigar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 11:23:07 by alexigar          #+#    #+#             */
/*   Updated: 2024/08/26 12:16:10 by alexigar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**free_and_nl(char **tokens, int *flag)
{
	errno = 8;
	perror("Invalid tokens");
	ft_free_char(tokens);
	*flag += 1;
	return (NULL);
}

char	**check_directory(char *token, char **tokens, int *flag)
{
	DIR	*dir;

	dir = opendir(token);
	if (dir)
	{
		errno = EISDIR;
		perror("minishell");
		closedir(dir);
		return (free_and_nl(tokens, flag));
	}
	else if (errno != ENOENT)
	{
		if (access(token, X_OK) != 0)
		{
			errno = EACCES;
			perror(token);
			return (free_and_nl(tokens, flag));
		}
	}
	return (tokens);
}

char	**check_tokens(char **tokens, int *flag)
{
	int	i;

	i = 0;
	if (count_nbr_tokens(tokens) == 1 && tokens[i][0] == '/')
		return (check_directory(tokens[i], tokens, flag));
	while (i < count_nbr_tokens(tokens))
	{
		if (tokens[i][0] == '|' || tokens[i][0] == '>' || tokens[i][0] == '<')
		{
			if ((i + 1) == count_nbr_tokens(tokens))
				return (free_and_nl(tokens, flag));
			else if (tokens[i][0] == tokens[i + 1][0])
				return (free_and_nl(tokens, flag));
		}
		if (++i == count_nbr_tokens(tokens))
			break ;
	}
	return (tokens);
}

void	ft_ini_dup(int *dup)
{
	dup[0] = 0;
	dup[1] = 0;
	dup[2] = 0;
	dup[3] = 0;
}
