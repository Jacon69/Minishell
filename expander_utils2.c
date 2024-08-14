/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexigar <alexigar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 17:26:12 by jconde-a          #+#    #+#             */
/*   Updated: 2024/08/14 18:00:06 by alexigar         ###   ########.fr       */
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

char	*ft_expander_pwd(char *token, t_list **env)
{
	char	*var_env;
	char	*aux;
	char	*aux1;

	var_env = ft_get_var_env(env, "..PWD");
	if (!var_env)
		return ((char *)ft_free_char_n(token, NULL,
				NULL, "Mem error getting \"PWD\""));
	aux1 = ft_substr(token, 1, ft_strlen(token) - 1);
	if (!aux1)
		return ((char *)ft_free_char_n(token, var_env, NULL, "Mem err1.5"));
	aux = token;
	token = ft_strjoin(var_env, aux1);
	ft_free_char_n(aux, var_env, aux1, NULL);
	if (!token)
		return (NULL);
	return (token);
}

char	*ft_expander_home(char *token, t_list **env)
{
	char	*var_env;
	char	*aux;
	char	*aux1;

	printf("aqui \n");
	var_env = ft_get_var_env(env, "..HOME");
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
	return (token);
}

/*char	*ft_expander_minuspwd(char *token, t_list **env)
{
	char	*var_env;
	char	*aux;
	char	*aux1;

	aux = ft_get_var_env(env, "..PWD");
	if (!aux)
		return ((char *)ft_free_char_n(token, NULL,
				NULL, "Mem error getting \"PWD\""));
	var_env = ft_decrease_dir(aux);    //TODO
	free(aux);
	if (!var_env)
		return ((char *)ft_free_char_n(token, NULL,
				NULL, "Mem error getting \"PWD\""));
	aux1 = ft_substr(token, 1, ft_strlen(token) - 1);
	if (!aux1)
		return ((char *)ft_free_char_n(token, var_env, NULL, "Mem err1.5"));
	aux = token;
	token = ft_strjoin(var_env, aux1);
	ft_free_char_n(aux, var_env, aux1, NULL);
	if (!token)
		return (NULL);
	return (token);
}*/
