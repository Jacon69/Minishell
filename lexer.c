/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jconde-a <jconde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 13:22:20 by jaimecondea       #+#    #+#             */
/*   Updated: 2024/08/13 17:54:27 by jconde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_assig_token(char *line, char **token, int *paran, int *flag)
{
	if ((line[paran[1]] == '"') || (line[paran[1]] == '\''))
		return (ft_aux_assig_token(line, token, paran, flag));
	while (line[paran[1]] && line[paran[1]] != ' ')
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

	if ((paran[0] > 0) && (paran[2] > 0) && (!(line[paran[2] - 1] == ' '
				|| line[paran[2] - 1] == '>' || line[paran[2] - 1] == '<'
				|| line[paran[2] - 1] == '|')))
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

char	**aux_lexer(char *line, char **token, int *flag)
{
	int		paran[3];
	char	*new_token;

	paran[0] = 0;
	paran[1] = 0;
	while (line[paran[1]] != '\0')
	{
		while (line[paran[1]] == ' ')
			paran[1]++;
		paran[2] = paran[1];
		new_token = (char *)malloc(sizeof(char));
		if (!new_token)
			free_and_return(token);
		new_token[0] = '\0';
		ft_assig_token(line, &new_token, paran, flag);
		if (paran[1] == -1 || *flag)
			free_and_return(token);
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
	printf ("nº toke: %d \n", ntoken);
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
	if (!aux_lexer(line, token, flag))
		return (NULL);
	token[ntoken] = NULL;
	i = 0;
	return (token);
}
