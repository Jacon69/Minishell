/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexigar <alexigar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 13:22:20 by jaimecondea       #+#    #+#             */
/*   Updated: 2024/08/12 12:38:23 by alexigar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//adds line[i] to token[j]
int	add_chartotoken(char *line, char **token, int i)
{
	char	*aux;
	char	simbol[2];

	simbol[1] = '\0';
	simbol[0] = line[i];
	aux = *token;
	*token = ft_strjoin(*token, simbol);
	if (aux)
		free(aux);
	if (!*token)
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
		if (add_chartotoken(line, token, paran[1]) == -1)
			return (-1);
		if (ft_is_asignsymbol(line, paran[1]) == 1)
		{
			paran[1]++;
			break ;
		}
		if (ft_is_asignsymbol(line, paran[1]) == 2)
		{
			paran[1]++;
			if (add_chartotoken(line, token, paran[1]) == -1)
				return (-1);
			paran[1]++;
			break ;
		}
		paran[1]++;
	}
	return (paran[1]);
}

int	ft_token_join(char *line, char **token, char *new_token, int *paran)
{
	char	*aux_str;

	if ((paran[0] > 0) && (paran[3] > 0) && (!(line[paran[3] - 1] == ' '
				|| line[paran[3] - 1] == '>' || line[paran[3] - 1] == '<'
				|| line[paran[3] - 1] == '|')))
	{
		aux_str = token[paran[0] - 1];
		token[paran[0] - 1] = ft_strjoin(token[paran[0] - 1], new_token);
		if (!token[paran[0] - 1])
		{
			free(new_token);
			ft_free_char(token);
			return (1);
		}
		free(aux_str);
		paran[0]--;
		paran[2]--;
		return (0);
	}
	if (!(new_token[0] == '\0'))
		token[paran[0]] = new_token;
	return (0);
}

char	**free_and_return(char **token)
{
	ft_free_char(token);
	return (NULL);
}

char	**aux_lexer(char *line, int ntoken, char **token, int *flag)
{
	int		paran[4];
	char	*new_token;

	paran[0] = 0;
	paran[1] = 0;
	paran[2] = ntoken;
	while (paran[0] <= paran[2])
	{
		while (line[paran[1]] == ' ')
			paran[1]++;
		paran[3] = paran[1];
		new_token = (char *)malloc(sizeof(char));
		if (!new_token)
			free_and_return(char token);
		new_token[0] = '\0';
		ft_assig_token(line, &new_token, paran, flag);
		if (paran[1] == -1 || *flag)
			free_and_return(char token);
		if (ft_token_join(line, token, new_token, paran) == 1)
			return (NULL);
		paran[0]++;
	}
	return (token);
}

/*Converts the line into tokens*/
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
	i = 0;
	return (token);
}
