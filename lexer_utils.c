/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexigar <alexigar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 18:52:23 by alexigar          #+#    #+#             */
/*   Updated: 2024/08/07 18:54:22 by alexigar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* busca el caracter en la cadena y devuelve la posicion*/
static int	next_delimiter(char const *str, char c, int i)
{
	while ((str[i] && str[i] != c))
		i++;
	return (i);
}

/*avanza posiciones para contar tokens*/
int	ft_aux_count_tokens(char *line, int i )
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

/*cuenta tokens*/
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
			tokens++;
			i = next_delimiter(line, line[i], i + 1) + 1;
		}
		else if (line[i])
		{
			i = ft_aux_count_tokens(line, i);
			tokens++;
		}
	}
	return (tokens);
}

/* asigna token entre comillas*/
int	ft_aux_assig_token(char *line, char **token, int j, int i)
{
	if (!line[next_delimiter(line, line[i], i + 1)])
	{
		perror("do not close quote.");
		return (-1);
	}
	free(token[j]);
	token[j] = ft_substr(line, i,
			next_delimiter(line, line[i], i + 1) - i + 1);
	if (!token[j])
		return (-1);
	i = next_delimiter(line, line[i], i + 1) + 1;
	return (i);
}

/*indica si line[i] es simbolo token <>| >><<*/
int	ft_is_asignsymbol(char *line, int i)
{
	int	position;

	position = 0;
	if (((line[i] == '<') && (line[i + 1] != '<')) || ((line[i] == '>')
			&& (line[i + 1] != '>')) || (line[i] == '|')
		|| (line[i + 1] == '|'))
		position = 1;
	if (line[i] == line[i + 1] && ((line[i] == '<') || (line[i] == '>')))
		position = 2;
	return (position);
}
