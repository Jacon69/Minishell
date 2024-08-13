/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jconde-a <jconde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 17:26:12 by jconde-a          #+#    #+#             */
/*   Updated: 2024/08/13 17:39:50 by jconde-a         ###   ########.fr       */
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
