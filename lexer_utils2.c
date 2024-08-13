/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jconde-a <jconde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 17:53:07 by jconde-a          #+#    #+#             */
/*   Updated: 2024/08/13 18:31:59 by jconde-a         ###   ########.fr       */
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

//Checks if line[i] is a symbol <>| >><<
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
