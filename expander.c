/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaimecondea <jaimecondea@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 17:56:30 by jconde-a          #+#    #+#             */
/*   Updated: 2024/08/15 19:40:02 by jaimecondea      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_add_quote_aux(char *token, char **aux_str, int *param, char *letter)
{
	char	*ptr;
	char	ctrl;

	ctrl = token[param[2] - 1];
	while (token[param[2]] && (token[param[2]] != ctrl))
	{
		ptr = *aux_str;
		*aux_str = ft_strjoin(*aux_str, letter);
		if (!*aux_str)
			return (0);
		free(ptr);
		param[1]++;
		param[2]++;
		letter[0] = token[param[2]];
	}
	return (1);
}

/*
	 - Removes double quotes from a token.
	 - Single quotes and their contents are left unchanged.
*/
char	*ft_add_quote(char *token, char **aux_str, int *param)
{
	char	letter[2];
	char	ctrl;

	ctrl = token[param[2]];
	param[2]++;
	param[1]++;
	if (token[param[2]])
		letter[0] = token[param[2]];
	else
	{
		perror("do not close quote");
		return (NULL);
	}
	letter[1] = '\0';
	if (ft_add_quote_aux(token, aux_str, param, letter) == 0)
		return (NULL);
	if (ctrl != token[param[2]])
	{
		perror("do not close quote");
		return (NULL);
	}
	param[2]++;
	param[1]++;
	return (*aux_str);
}

char	*ft_add_normal(char *token, char **aux_str, int *param)
{
	char	*ptr;
	char	letter[2];

	letter[0] = token[param[2]];
	letter[1] = '\0';
	while (token[param[2]] && (token[param[2]]
			!= '"' && token[param[2]] != '\''))
	{
		ptr = *aux_str;
		*aux_str = ft_strjoin(*aux_str, letter);
		if (!*aux_str)
			return (NULL);
		free(ptr);
		param[2]++;
		param[1]++;
		letter[0] = token[param[2]];
	}
	return (*aux_str);
}

char	*ft_add_expander(char *token, char *aux_str, t_list **env, int *param)
{
	char	crtl;

	while (token[param[2]])
	{
		crtl = token[param[2]];
		if (crtl != '"' && crtl != '\'')
			aux_str = ft_add_normal(token, &aux_str, param);
		else
			aux_str = ft_add_quote(token, &aux_str, param);
		if ((aux_str) && (crtl != '\''))
			aux_str = ft_expander_dollar(&aux_str, env);
	}
	if ((aux_str))
		aux_str = ft_expander_home_pwd(&aux_str, env);
	return (aux_str);
}

/*
   Expander:
	 - Analyzes tokens for variables, relative paths, and quotes.
	 - Performs variable expansion, replacing variable names with their values.
	 - Resolves relative paths by converting them to absolute paths.
	 - Returns the modified token with expanded values.
*/
int	expander(char **token, t_list **env)
{
	int		param[3];
	char	*aux_str;

	param[0] = 0;
	param[2] = 0;
	while (token[param[0]])
	{
		aux_str = (char *)ft_calloc(1, sizeof(char));
		if (!aux_str)
			return (0);
		aux_str[0] = '\0';
		param[1] = 0;
		while ((token[param[0]][param[1]] != '\0'))
		{
			aux_str = ft_add_expander(token[param[0]], aux_str, env, param);
			if (!aux_str)
				return (0);
			param[2] = 0;
		}
		free(token[param[0]]);
		token[param[0]] = aux_str;
		param[0]++;
	}
	return (1);
}
