/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexigar <alexigar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 19:52:48 by jconde-a          #+#    #+#             */
/*   Updated: 2024/08/07 19:08:03 by alexigar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Auxilliary Environment variables conversion
static int	ft_aux_expander_dollar(char **token, t_list **env, int i)
{
	char	*var_env;
	char	*aux;
	char	*aux1;

	aux1 = *token;
	aux = ft_ext_var_env(&aux1[i +1]);
	if (!aux)
	{
		free(*token);
		return (-1);
	}
	var_env = ft_get_var_env(env, aux);
	if (!var_env)
	{
		free(aux);
		free(*token);
		return (-1);
	}
	*token = ft_token_exp(*token, i, var_env, ft_strlen(aux));
	i += ft_strlen(var_env)-1;
	ft_free_char_n(aux, var_env, aux1, NULL);
	if (!*token)
		return (-1);
	return (i);
}

/*Modifies a token by replacing environment variables
 (preceded by '$') with their values. */
static char	*ft_expander_dollar(char *token, t_list **env)
{
	int	i;

	i = 0;
	token = ft_strdup(token);
	if (!token)
		return (NULL);
	while (token[i])
	{
		if (token[i] == '$')
		{
			i = ft_aux_expander_dollar(&token, env, i);
			if (i == -1)
				return (NULL);
		}
		++i;
	}
	return (token);
}

/* 
     - Removes double quotes from a token.
     - Expands environment variables found 
	 	within double quotes (preceded by '$').
     - Single quotes and their contents are left unchanged.
*/
static char	*ft_expander_q(char *token, t_list **env)
{
	char	*aux;
	char	*aux2;
	int		lng_token;

	lng_token = ft_strlen(token);
	aux = ft_substr(token, 1, lng_token - 2);
	if (!aux)
		return (NULL);
	if (token[0] == '\'')
		return (aux);
	aux2 = ft_expander_dollar(aux, env);
	free(aux);
	if (!aux2)
		return (NULL);
	return (aux2);
}

char	*ft_aux_expander(char *token, t_list **env)
{
	char	*aux;

	token = ft_expander_dollar(token, env);
	if (!token)
		return (NULL);
	aux = token;
	token = ft_expander_home(token, env);
	free(aux);
	if (!token)
		return (NULL);
	return (token);
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
	int		i;
	char	*aux;

	i = -1;
	while (token[++i])
	{
		if (token[i][0] == '"' || token[i][0] == '\'')
		{
			aux = token[i];
			token[i] = ft_expander_q(token[i], env);
			free(aux);
			if (!token[i])
				return (0);
		}
		else
		{
			aux = token[i];
			token[i] = ft_aux_expander(token[i], env);
			free(aux);
			if (!token[i])
				return (0);
		}
	}
	return (1);
}
