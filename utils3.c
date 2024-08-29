/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexigar <alexigar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 18:47:56 by alexigar          #+#    #+#             */
/*   Updated: 2024/08/29 09:58:31 by alexigar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*Devuelve el puntero a un duplicado de la cadena
apuntada por el puntero pasado y el nº de caracteres indicados*/
char	*ft_strndup(const char *str, size_t n)
{
	char	*dup;
	size_t	len;

	len = ft_strlen(str);
	if (n < len)
	{
		len = n;
	}
	dup = (char *) malloc(len + 1);
	if (dup == NULL)
		return (NULL);
	ft_memcpy(dup, str, len);
	dup[len] = '\0';
	return (dup);
}

void	ft_aux2_dup_ord_list(t_list *p_env
								, t_list *p_env_aux)
{
	int		exit_bucle;
	t_list	**ref_previa;

	exit_bucle = 1;
	while (p_env -> next && (exit_bucle == 1))
	{
		ref_previa = &p_env -> next;
		p_env = p_env -> next;
		if (ft_strncmp(p_env -> content, p_env_aux -> content,
				ft_smaller(ft_strlen(p_env -> content),
					ft_strlen(p_env_aux -> content))) > 0)
		{
			*ref_previa = p_env_aux;
			p_env_aux -> next = p_env;
			exit_bucle = 0;
		}
		else if (p_env -> next == NULL)
		{
			p_env -> next = p_env_aux;
			exit_bucle = 0;
		}
	}
}

t_list	**ft_aux_dup_ord_list(t_list **dup_env, t_list *p_env
								, t_list *p_env_ori)
{
	t_list	*p_env_aux;

	while (p_env_ori -> next)
	{
		p_env_ori = p_env_ori -> next;
		p_env_aux = ft_lstnew(ft_strdup(p_env_ori->content));
		if (!p_env_aux)
			return ((t_list **)ft_free_list(dup_env, NULL));
		p_env = *dup_env;
		if (ft_strncmp(p_env -> content, p_env_aux -> content,
				ft_smaller(ft_strlen(p_env -> content),
					ft_strlen(p_env_aux -> content))) > 0)
		{
			*dup_env = p_env_aux;
			p_env_aux -> next = p_env;
			continue ;
		}
		else if (p_env -> next == NULL)
		{
			p_env -> next = p_env_aux;
			continue ;
		}
		ft_aux2_dup_ord_list(p_env, p_env_aux);
	}
	return (dup_env);
}
