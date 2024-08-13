/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jconde-a <jconde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 19:07:40 by alexigar          #+#    #+#             */
/*   Updated: 2024/08/13 20:10:26 by jconde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*frees var and tex_error*/
void	*ft_free_char_n(char *par1, char *par2, char *par3, char *txt_err)
{
	if (par1)
		free(par1);
	if (par2)
		free(par2);
	if (par3)
		free(par3);
	if (txt_err)
		perror(txt_err);
	return (NULL);
}

char	*ft_expander_PWD(char *token, t_list **env)
{
	char	*var_env;
	char	*aux;
	char	*aux1;

	token = ft_strdup(token);
	if (!token)
		return (NULL);
	if (token[0] == '.' && token[1] == '/')
	{
		var_env = ft_get_var_env(env, "..PWD");
		if (!var_env)
			return ((char *)ft_free_char_n(token, NULL,
					NULL, "Mem error getting \"HOME\""));
		aux1 = ft_substr(token, 1, ft_strlen(token) - 1);
		if (!aux1)
			return ((char *)ft_free_char_n(token, var_env, NULL, "Mem err1.5"));
		aux = token;
		token = ft_strjoin(var_env, aux1);
		ft_free_char_n(aux, var_env, aux1, NULL);
		if (!token)
			return (NULL);
	}
	return (token);
}

/* ft_expander_home:
   - Converts relative paths into absolute paths.
   - Uses the home directory as a reference for expansion.
*/
char	*ft_expander_home(char *token, t_list **env)
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
		var_env = ft_get_var_env(env, "..HOME");
		if (!var_env)
			return ((char *)ft_free_char_n(token, NULL,
					NULL, "Mem error getting \"HOME\""));
		aux1 = ft_substr(token, 1, ft_strlen(token) - 1);
		if (!aux1)
			return ((char *)ft_free_char_n(token, var_env, NULL, "Mem err1"));
		aux = token;
		token = ft_strjoin(var_env, aux1);
		ft_free_char_n(aux, var_env, aux1, NULL);
		if (!token)
			return (NULL);
	}
	return (token);
}

// Extracts var from str[0] and returns its value if it exists
static char	*ft_ext_var_env(char *str)
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

// Auxilliary Environment variables conversion
static	int	ft_aux_expander_dollar(char **token, t_list **env, int i)
{
	char	*var_env;
	char	*aux;
	char	*aux1;

	aux1 = *token;
	aux = ft_ext_var_env(&aux1[i + 1]);
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
	if (ft_strlen(var_env) != 0)
		i += ft_strlen(var_env) - 1;
	ft_free_char_n(aux, var_env, aux1, NULL);
	if (!*token)
		return (-1);
	return (i);
}

/*Modifies a token by replacing environment variables
 (preceded by '$') with their values. */
char	*ft_expander_dollar(char *token, t_list **env)
{
	int	i;

	i = 0;
	token = ft_strdup(token);
	if (!token)
		return (NULL);
	while ((size_t)i < ft_strlen(token))
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
