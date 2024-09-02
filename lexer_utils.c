/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexigar <alexigar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 18:52:23 by alexigar          #+#    #+#             */
/*   Updated: 2024/09/02 09:51:12 by alexigar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Finds the character and returns its position
static int	next_delimiter(char const *str, char c, int i)
{
	while ((str[i] != '\0' && str[i] != c))
		i++;
	return (i);
}

/*Counts tokens by advancing positions*/
int	ft_aux_count_tokens(char *line, int i)
{
	while (line[i] != ' ' && line[i])
	{
		if (((line[i] == '<') || (line[i] == '>')) && (line[i] == line[i + 1]))
		{
			i += 2;
			break ;
		}
		if ((line[i] == '<') || (line[i] == '>'))
		{
			i++;
			break ;
		}
		if ((line[i] == '"') || (line[i] == '\''))
			break ;
		if (line[i] == '|' || line[i + 1] == '|'
			|| line[i + 1] == '<' || line[i + 1] == '>')
		{
			i++;
			break ;
		}
		i++;
	}
	return (i);
}

int	ft_aux2_count_tokens(char *line, int i, int *tokens)
{
	if (i == 0 || line[i - 1] == ' ' || line[i] == '>'
		|| line[i] == '<' || line[i] == '|')
	{
		if ((line[i] == '>' || line[i] == '<' || line[i] == '|')
			&& (size_t)i < (ft_strlen(line) - 1))
			*tokens += 1;
		i = ft_aux_count_tokens(line, i);
		*tokens += 1;
	}
	else
		i = ft_aux_count_tokens(line, i);
	return (i);
}

/*counts tokens*/
int	ft_count_tokens(char *line)
{
	int		tokens;
	int		i;

	i = 0;
	tokens = 0;
	while (line[i])
	{
		while (line[i] == ' ')
			i++;
		if ((line[i] == '"') || (line[i] == '\''))
		{
			if ((i == 0) || ((i > 0) && ((line[i - 1] == ' '
							|| line[i - 1] == '>' || line[i - 1] == '<'
							|| line[i - 1] == '|'))))
				tokens++;
			i = next_delimiter(line, line[i], i + 1) + 1;
		}
		else if (line[i])
			i = ft_aux2_count_tokens(line, i, &tokens);
		if ((size_t)i >= ft_strlen(line))
			break ;
	}
	return (tokens);
}

/* asigns token between ""*/
int	ft_aux_assig_token(char *line, char **token, int *paran, int *flag)
{
	if (!line[next_delimiter(line, line[paran[1]], paran[1] + 1)])
	{
		*flag = 1;
		free(*token);
		perror("do not close quote.");
		return (-1);
	}
	free(*token);
	*token = ft_substr(line, paran[1],
			next_delimiter(line, line[paran[1]], paran[1] + 1) - paran[1] + 1);
	if (!*token)
		return (-1);
	paran[1] = next_delimiter(line, line[paran[1]], paran[1] + 1) + 1;
	return (paran[1]);
}
