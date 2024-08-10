/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jconde-a <jconde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 19:07:40 by alexigar          #+#    #+#             */
/*   Updated: 2024/08/10 17:31:17 by jconde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*frees var and tex_error*/
char	*ft_free_char_n(char *par1, char *par2, char *par3, char *txt_err)
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
			return (ft_free_char_n(token, NULL, NULL
					, "Mem error getting \"HOME\""));
		aux1 = ft_substr(token, 1,
				ft_strlen(token) - 1);
		if (!aux1)
			return (ft_free_char_n(token, var_env, NULL, "Mem err1"));
		aux = token;
		token = ft_strjoin(var_env, aux1);
		ft_free_char_n(aux, var_env, aux1, NULL);
		if (!token)
			return (NULL);
	}
	return (token);
}

//Extracts var from str[0] and returns its value if it exists
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

//Returns the expanded token in the point it is sent
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
