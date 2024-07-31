/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jconde-a <jconde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 19:52:48 by jconde-a          #+#    #+#             */
/*   Updated: 2024/07/30 19:53:02 by jconde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ft_expander_home:
   - Converts relative paths into absolute paths.
   - Uses the home directory as a reference for expansion.
*/
static char *ft_expander_home(char *token, t_list  **env) 
{
	char	*var_env;
	int		i;
	int		j;
	char	*aux;
	char	*aux1;
	char	*aux2;
	int		var_long;

	i = 0;
	token = ft_strdup(token);
	if (!token)
		return (NULL);
	while (token[i])
	{
		if ((token[i] == '~' && token[i + 1] == '/')
			|| (token[i] == '~' && ft_strlen(token) == 1))
		{
			j = i;
			var_long = 1;
			i++;
			var_env = ft_get_var_env(env, "HOME");
			if (!var_env)
			{
				free(token);
				return (NULL);
			}
			aux = token;
			aux1 = ft_substr(token, 0, j);
			if (!aux1)
			{
				free(token);
				free(var_env);
				return (NULL);
			}
			aux2 = ft_substr(token, j + var_long,
					ft_strlen(token) - (j + var_long));
			if (!aux2)
			{
				free(aux1);
				free(token);
				free(var_env);
				return (NULL);
			}
			token = ft_strjoin(aux1, var_env);
			free (aux);
			if (!token)
			{
				free(aux1);
				free(aux2);
				free(var_env);
				return (NULL);
			}
			aux = token;
			token = ft_strjoin(token, aux2);
			free(aux);
			free(aux1);
			free(aux2);
			free(var_env);
			if (!token)
				return (NULL);
		}
		i++;
	}
	return (token);
}

/* ft_expander_$: Modifies a token by replacing environment variables
 (preceded by '$') with their values. */
static char *ft_expander_dollar(char *token, t_list  **env) 
{
	char	*var_env;
	int		i;
	int		j;
	char	*aux;
	char	*aux1;
	char	*aux2;
	int		var_long;

	i = 0;
	token = ft_strdup(token);
	if (!token)
		return (NULL);
	while (token[i])
	{
		if (token[i] == '$')
		{
			j = i;
			var_long = 0;
			while (token[i] && token[i] != ' ')
			{
				var_long++;
				i++;
			}
			aux = ft_substr(token, j + 1, var_long - 1);
			if (!aux)
			{
				free(token);
				return (NULL);
			}
			var_env = ft_get_var_env(env, aux);
			free(aux);
			if (!var_env)
			{
				free(token);
				return (NULL);
			}
			aux = token;
			aux1 = ft_substr(token, 0, j);
			if (!aux1)
			{
				free(token);
				free(var_env);
				return (NULL);
			}
			aux2 = ft_substr(token, j + var_long,
					ft_strlen(token) - (j + var_long));
			if (!aux2)
			{
				free(aux1);
				free(token);
				free(var_env);
				return (NULL);
			}
			token = ft_strjoin(aux1, var_env);
			free(aux);
			free(var_env);
			if (!token)
			{
				free(aux1);
				free(aux2);
				return (NULL);
			}
			aux = token;
			token = ft_strjoin(token, aux2);
			free(aux);
			free(aux1);
			free(aux2);
			if (!token)
				return (NULL);
			aux = token;
			token = ft_expander_dollar(token, env);
			free (aux);
			if (!token)
				return (NULL);
		}
		i++;
	}
	return (token);
}

/* 
   ft_expander_q:
     - Removes double quotes from a token.
     - Expands environment variables found within double quotes (preceded by '$').
     - Single quotes and their contents are left unchanged.
*/
static char *ft_expander_q(char *token, t_list  **env)
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

/* 
   Expander:
     - Analyzes tokens for variables, relative paths, and quotes.
     - Performs variable expansion, replacing variable names with their values.
     - Resolves relative paths by converting them to absolute paths.
     - Returns the modified token with expanded values.
*/
int	expander(char **token, t_list  **env)
{
	int		i;
	char	*aux;

	i = 0;
	while (token[i])
	{
		if (token[i][0] == '"' || token[i][0] == '\'')
		{
			aux = token[i];
			token[i] = ft_expander_q(token[i], env);
			if (!token[i])
				return (0);
			free(aux);
		}
		else
		{
			aux = token[i];
			token[i] = ft_expander_dollar(token[i], env);
			if (!token[i])
				return (0);
			free(aux);
			aux = token[i];
			token[i] = ft_expander_home(token[i], env);
			if (!token[i])
				return (0);
			free(aux);
		}
		i++;
	}
	return (1);
}
