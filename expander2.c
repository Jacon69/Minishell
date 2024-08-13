/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexigar <alexigar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 16:47:17 by alexigar          #+#    #+#             */
/*   Updated: 2024/08/13 16:47:33 by alexigar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Returns the expanded token in the point it is sent
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
			return ((char *)ft_free_char_n(token, NULL, NULL,
					"Mem error getting \"HOME\""));
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

// Auxilliary Environment variables conversion
static int	ft_aux_expander_dollar(char **token, t_list **env, int i)
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
static char	*ft_expander_dollar(char *token, t_list **env)
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

/*
	 - Removes double quotes from a token.
	 - Single quotes and their contents are left unchanged.
*/
char	*ft_add_quote(char *token, char *aux_str, int *param)
{
	char	*ptr;
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
	while (token[param[2]] && (token[param[2]] != ctrl))
	{
		ptr = aux_str;
		aux_str = ft_strjoin(aux_str, letter);
		if (!aux_str)
			return (NULL);
		free(ptr);
		param[1]++;
		param[2]++;
		letter[0] = token[param[2]];
	}
	if (ctrl != token[param[2]])
	{
		perror("do not close quote");
		return (NULL);
	}
	param[2]++;
	param[1]++;
	return (aux_str);
}

char	*ft_add_normal(char *token, char *aux_str, int *param)
{
	char	*ptr;
	char	letter[2];

	letter[0] = token[param[2]];
	letter[1] = '\0';
	while (token[param[2]]
		&& (token[param[2]] != '"' && token[param[2]] != '\''))
	{
		ptr = aux_str;
		aux_str = ft_strjoin(aux_str, letter);
		if (!aux_str)
			return (NULL);
		free(ptr);
		param[2]++;
		param[1]++;
		letter[0] = token[param[2]];
	}
	return (aux_str);
}

char	*ft_add_expander(char *token, char *aux_str, t_list **env, int *param)
{
	char	crtl;

	while (token[param[2]])
	{
		crtl = token[param[2]];
		if (crtl != '"' && crtl != '\'')
			aux_str = ft_add_normal(token, aux_str, param);
		else
			aux_str = ft_add_quote(token, aux_str, param);
		if ((aux_str) && (crtl != '\''))
			aux_str = ft_expander_dollar(aux_str, env);
	}
	if ((aux_str) && (param[1] == 0))
		aux_str = ft_expander_home(aux_str, env);
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
	char	*ptr;

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
		ptr = token[param[0]];
		token[param[0]] = aux_str;
		free(ptr);
		param[0]++;
	}
	return (1);
}
