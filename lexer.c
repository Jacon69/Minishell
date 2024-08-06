/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaimecondea <jaimecondea@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 13:22:20 by jaimecondea       #+#    #+#             */
/*   Updated: 2024/08/06 18:53:25 by jaimecondea      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	next_delimiter(char const *str, char c, int i)
{
	while ((str[i] && str[i] != c))
		i++;
	return (i);
}

static int	count_tokens(char *line)
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
			while (line[i] != ' ' && line[i])
			{
				if (line[i] == '<')
				{
					if (line[i + 1] == '<')
						i++;
					i++;
					break ;
				}
				if (line[i] == '>')
				{
					if (line[i + 1] == '>')
						i++;
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
	if ((line[i] == '<') || (line[i] == '>') || (line[i] == '|')
		|| (line[i + 1] == '|') || (line[i] == '<') || (line[i] == '>'))
		position = 1;
	if (line[i] == line[i+1] && (line[i] != '|'))
		position = 2;
	return (position);
}

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
	if ((!token[j]))
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
		token[j] = (char *)malloc(sizeof(char));
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


	ntoken = count_tokens(line);
	if (!line)
		return (NULL);
	token = (char **) malloc(sizeof(char *) * (ntoken + 1));
	if (!token)
		return (NULL);
	token = aux_lexer(line, ntoken, token);
	token[ntoken] = NULL;
	return (token);
}
