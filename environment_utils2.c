/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_utils2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexigar <alexigar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 19:11:53 by alexigar          #+#    #+#             */
/*   Updated: 2024/08/07 19:14:35 by alexigar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_aux_del_v_env(t_list **env, t_list	**p_env
	, t_list	**aux2, char *str_aux)
{
	t_list	*aux;
	t_list	*p_aux;
	t_list	*p_aux2;

	p_aux = *p_env;
	p_aux2 = *aux2;
	if ((ft_strncmp(p_aux->content, str_aux, ft_strlen(str_aux)) == 0)
		&& (ft_pos_chr(p_aux->content, '=') == (int)ft_strlen(str_aux)))
	{
		aux = p_aux->next;
		free(p_aux->content);
		free(p_aux);
		p_aux = aux;
		if (p_aux2)
		{
			p_aux2->next = p_aux;
			*aux2 = p_aux2;
			*p_env = aux;
		}
		else
			*env = p_aux;
		return ;
	}
	*aux2 = p_aux;
	*p_env = p_aux->next;
}
