/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexigar <alexigar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 13:22:20 by jaimecondea       #+#    #+#             */
/*   Updated: 2024/08/08 10:28:59 by alexigar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* añado el caracter line[i] a token[j]*/
int	add_chartotoken(char *line, char **token, int i, int j)
{
	char	*aux;
	char	simbol[2];

	simbol[1] = '\0';
	simbol[0] = line[i];
	aux = token[j];
	token[j] = ft_strjoin(token[j], simbol);
	if (*aux)
		free(aux);
	if (!token[j])
		return (-1);
	return (0);
}

static int	ft_assig_token(char *line, char **token, int j, int i)
{
	if ((line[i] == '"') || (line[i] == '\''))
		return (ft_aux_assig_token(line, token, j, i));
	while (line[i] != ' ' && line[i])
	{
		if (line[i] == '"' || line[i] == '\'')
			break ;
		if (add_chartotoken(line, token, i, j) == -1)
			return (-1);
		if (ft_is_asignsymbol(line, i) == 1)
		{
			i++;
			break ;
		}
		if (ft_is_asignsymbol(line, i) == 2)
		{
			i += 2;
			if (add_chartotoken(line, token, i, j) == -1)
				return (-1);
			break ;
		}
		i++;
	}
	return (i);
}

char	**aux_lexer(char *line, int ntoken, char **token)
{
	int		i;
	int		j;

	j = 0;
	i = 0;
	while (j < ntoken)
	{
		while (line[i] == ' ')
			i++;
		token[j] = (char *)malloc(sizeof(char)); //leak
		if (!token[j])
		{
			ft_free_char(token);
			return (NULL);
		}
		token[j][0] = '\0';
		i = ft_assig_token(line, token, j, i);
		if (i == -1)
		{
			ft_free_char(token);
			return (NULL);
		}
		j++;
	}
	return (token);
}

/*Convierte la linea en tokens*/
char	**lexer(char *line)
{
	char	**token;
	int		ntoken;

	ntoken = ft_count_tokens(line);
	if (!line)
		return (NULL);
	token = (char **) malloc(sizeof(char *) * (ntoken + 1));
	if (!token)
		return (NULL);
	token = aux_lexer(line, ntoken, token); //leak
	token[ntoken] = NULL;
	return (token);
}
