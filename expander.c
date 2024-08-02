/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jconde-a <jconde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 19:52:48 by jconde-a          #+#    #+#             */
/*   Updated: 2024/08/01 16:46:39 by jconde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_free_char_n(char *par1, char *par2, char *par3, char *par4)
{
	if (par1)
		free(par1);
	if (par2)
		free(par2);
	if (par3)
		free(par3);
	if (par4)
		free(par4);
	return (NULL);
}

/* ft_expander_home:
   - Converts relative paths into absolute paths.
   - Uses the home directory as a reference for expansion.
*/
static char	*ft_expander_home(char *token, t_list **env)
{
	char	*var_env;
	char	*aux;
	char	*aux1;

	token = ft_strdup(token);
	if (!token)
		return (NULL);
	if ((token[0] == '~' && token[1] == '/')
		|| (token[0] == '~' && ft_strlen(token) == 1))
	{
		var_env = ft_get_var_env(env, "HOME");
		if (!var_env)
			return (ft_free_char_n(token, NULL, NULL, NULL));
		aux1 = ft_substr(token, 1,
				ft_strlen(token) - 1);
		if (!aux1)
			return (ft_free_char_n(token, var_env, NULL, NULL));
		aux = token;
		token = ft_strjoin(var_env, aux1);
		ft_free_char_n(aux, var_env, aux1, NULL);
		if (!token)
			return (NULL);
	}
	return (token);
}

/*Se extrae la var que encontramos en str[0] y devolvemos su valor si existe*/
char	*ft_ext_var_env(char *str)
{
	int		var_long;
	int		i;
	char	*aux;

	var_long = 0;
	i = 0;
	while (str[i] && str[i] != ' ')
	{
		var_long++;
		i++;
	}
	aux = ft_substr(str, 0, var_long);
	if (!aux)
		return (NULL);
	return (aux);
}

/*Esta función me devuelve el token expandido en el punto que se manda*/
char	*ft_token_exp(char *token, int j, char *var_env, int lng_var)
{
	char	*str;
	char	*aux;

	str = ft_substr(token, 0, j);
	if (!str)
		return (NULL);
	aux = str;
	str = ft_strjoin(str, var_env);
	if (!str)
		return (NULL);
	free(aux);
	aux = ft_substr(token, j + lng_var + 1,
			ft_strlen(token) - (j + lng_var));
	var_env = str;
	str = ft_strjoin(str, aux);
	if (!str)
		return (NULL);
	ft_free_char_n(aux, var_env, NULL, NULL);
	return (str);
}

// auxiliar en la conversión de las variables de entorno
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
