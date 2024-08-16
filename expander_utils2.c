/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexigar <alexigar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 17:26:12 by jconde-a          #+#    #+#             */
/*   Updated: 2024/08/16 10:46:53 by alexigar         ###   ########.fr       */
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

static int	ft_count_dir(char *str)
{
	int		ndir;
	char	*aux;

	ndir = 0;
	aux = str;
	while (aux++)
	{
		aux = ft_strchr(aux, '/' );
		ndir++;
	}
	return (ndir);
}

char	*ft_aux_decrease_dir(char **dir, char *nw_dir, int ndir)
{
	int		i;
	char	*aux;

	i = 0;
	while (i < (ndir - 1))
	{
		aux = nw_dir;
		nw_dir = ft_strjoin(nw_dir, dir[i]);
		free(aux);
		if (!nw_dir)
			return (NULL);
		aux = nw_dir;
		if (i < (ndir - 2))
		{
			nw_dir = ft_strjoin(nw_dir, "/");
			free(aux);
			if (!nw_dir)
				return (NULL);
		}
		i++;
	}
	return (nw_dir);
}

static char	*ft_decrease_dir(char *str)
{
	char	**dir;
	char	*nw_dir;
	int		ndir;

	ndir = ft_count_dir(str);
	dir = ft_split(str, '/');
	if (!dir)
		return (NULL);
	nw_dir = (char *) malloc(sizeof(char) * 2);
	if (!nw_dir)
	{
		ft_free_char(dir);
		return (NULL);
	}
	nw_dir[0] = '/';
	nw_dir[1] = '\0';
	nw_dir = ft_aux_decrease_dir(dir, nw_dir, ndir);
	ft_free_char(dir);
	if (!nw_dir)
	{
		free(nw_dir);
		return (NULL);
	}
	return (nw_dir);
}

/* // Returns the expanded  token if contains "../"*/
char	*ft_expander_minuspwd(char **token, t_list **env)
{
	char	*var_env;
	char	*aux;
	char	*aux1;

	aux = ft_get_var_env(env, "..PWD");
	if (!aux)
		return ((char *)ft_free_char_n(*token, NULL,
				NULL, "Mem error getting \"..PWD\""));
	var_env = (char *) ft_decrease_dir(aux);
	free(aux);
	if (!var_env)
		return ((char *)ft_free_char_n(*token, NULL,
				NULL, "Mem error getting \"PWD\""));
	aux1 = ft_substr(*token, 2, ft_strlen(*token));
	if (!aux1)
		return ((char *)ft_free_char_n(*token, var_env, NULL, "Mem err1.5"));
	aux = *token;
	*token = ft_strjoin(var_env, aux1);
	ft_free_char_n(aux, var_env, aux1, NULL);
	if (!*token)
		return (NULL);
	return (*token);
}
