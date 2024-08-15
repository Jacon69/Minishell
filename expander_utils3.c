/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaimecondea <jaimecondea@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 14:19:36 by jaimecondea       #+#    #+#             */
/*   Updated: 2024/08/15 23:06:16 by jaimecondea      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* // Returns the expanded  token if contains "./"
*/
char	*ft_expander_pwd(char **token, t_list **env)
{
	char	*var_env;
	char	*aux;
	char	*aux1;

	var_env = ft_get_var_env(env, "..PWD");
	if (!var_env)
		return ((char *)ft_free_char_n(*token, NULL,
				NULL, "Mem error getting \"PWD\""));
	aux1 = ft_substr(*token, 1, ft_strlen(*token) - 1);
	if (!aux1)
		return ((char *)ft_free_char_n(*token, var_env, NULL, "Mem err1.5"));
	aux = *token;
	*token = ft_strjoin(var_env, aux1);
	ft_free_char_n(aux, var_env, aux1, NULL);
	if (!*token)
		return (NULL);
	return (*token);
}

/* // Returns the expanded  token if contains "~"
*/
char	*ft_expander_home(char **token, t_list **env)
{
	char	*var_env;
	char	*aux;
	char	*aux1;

	var_env = ft_get_var_env(env, "..HOME");
	if (!var_env)
		return ((char *)ft_free_char_n(*token, NULL,
				NULL, "Mem error getting \"HOME\""));
	aux1 = ft_substr(*token, 1, ft_strlen(*token) - 1);
	if (!aux1)
		return ((char *)ft_free_char_n(*token, var_env, NULL, "Mem err1.5"));
	aux = *token;
	*token = ft_strjoin(var_env, aux1);
	ft_free_char_n(aux, var_env, aux1, NULL);
	if (!*token)
		return (NULL);
	return (*token);
}
