/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jconde-a <jconde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 18:52:23 by alexigar          #+#    #+#             */
/*   Updated: 2024/08/09 19:26:00 by jconde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* busca el caracter en la cadena y devuelve la posicion*/
static int	next_delimiter(char const *str, char c, int i)
{
	while ((str[i] != '\0' && str[i] != c))
		i++;
	return (i);
}

/*avanza posiciones para contar tokens*/
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
		if ((size_t)i >= ft_strlen(line))
			break ;
	}
	return (tokens);
}

/* asigna token entre comillas*/
int	ft_aux_assig_token(char *line, char **token, int *paran, int *flag)
{
	if (!line[next_delimiter(line, line[paran[1]], paran[1] + 1)])
	{
		*flag = 1;
		perror("do not close quote.");
		return (-1);
	}
	free(token[paran[0]]);
	token[paran[0]] = ft_substr(line, paran[1],
			next_delimiter(line, line[paran[1]], paran[1] + 1) - paran[1] + 1);
	if (!token[paran[0]])
		return (-1);
	paran[1] = next_delimiter(line, line[paran[1]], paran[1] + 1) + 1;
	return (paran[1]);
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
