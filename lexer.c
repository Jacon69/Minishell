/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jconde-a <jconde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 13:22:20 by jaimecondea       #+#    #+#             */
/*   Updated: 2024/08/09 20:06:48 by jconde-a         ###   ########.fr       */
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
	if (aux)
		free(aux);
	if (!token[j])
		return (-1);
	return (0);
}

static int	ft_assig_token(char *line, char **token, int *paran, int *flag)
{
	if ((line[paran[1]] == '"') || (line[paran[1]] == '\''))
		return (ft_aux_assig_token(line, token, paran, flag));
	while (line[paran[1]] != ' ' && line[paran[1]])
	{
		if (line[paran[1]] == '"' || line[paran[1]] == '\'')
			break ;
		if (add_chartotoken(line, token, paran[1], paran[0]) == -1)
			return (-1);
		if (ft_is_asignsymbol(line, paran[1]) == 1)
		{
			paran[1]++;
			break ;
		}
		if (ft_is_asignsymbol(line, paran[1]) == 2)
		{
			paran[1]++;
			if (add_chartotoken(line, token, paran[1], paran[0]) == -1)
				return (-1);
			paran[1]++;
			break ;
		}
		paran[1]++;
	}
	return (paran[1]);
}

char	**aux_lexer(char *line, int ntoken, char **token, int *flag)
{
	int		paran[2];

	paran[0] = 0;
	paran[1] = 0;
	while (paran[0] < ntoken)
	{
		while (line[paran[1]] == ' ')
			paran[1]++;
		token[paran[0]] = (char *)malloc(sizeof(char));
		if (!token[paran[0]])
		{
			ft_free_char(token);
			return (NULL);
		}
		token[paran[0]][0] = '\0';
		ft_assig_token(line, token, paran, flag);
		if (paran[1] == -1 || *flag)
		{
			ft_free_char(token);
			return (NULL);
		}
		paran[0]++;
	}
	return (token);
}

/*Convierte la linea en tokens*/
char	**lexer(char *line, int *flag)
{
	char	**token;
	int		ntoken;
	int		i;

	ntoken = ft_count_tokens(line);
	if (!line)
		return (NULL);
	token = (char **) malloc(sizeof(char *) * (ntoken + 1));
	if (!token)
		return (NULL);
	i = 0;
	while (i <= ntoken)
	{
		token[i] = NULL;
		i++;
	}
	if (!aux_lexer(line, ntoken, token, flag))
		return (NULL);
	token[ntoken] = NULL;
	return (token);
}
