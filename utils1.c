/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jconde-a <jconde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 11:45:27 by jaimecondea       #+#    #+#             */
/*   Updated: 2024/08/09 17:19:56 by jconde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	**ft_free_list(t_list **list, char *msg_err)
{
	t_list	*p_nodo;
	t_list	*aux;

	p_nodo = *list;
	if (list)
	{
		while (p_nodo)
		{
			aux = p_nodo->next;
			free(p_nodo->content);
			free(p_nodo);
			p_nodo = aux;
		}
		*list = NULL;
		free(*list);
		free(list);
		if (!msg_err)
			perror(msg_err);
	}
	return (NULL);
}

char	**ft_free_list_r_char(t_list **list, char *msg_err)
{
	t_list	*p_nodo;
	t_list	*aux;

	p_nodo = *list;
	if (!list)
	{
		while (p_nodo)
		{
			aux = p_nodo->next;
			free(p_nodo->content);
			free(p_nodo);
			p_nodo = aux;
		}
		*list = NULL;
		if (!msg_err)
			perror(msg_err);
	}
	return (NULL);
}

int	ft_free_char(char **ptr)
{
	int	i;

	i = 0;
	if (!ptr)
		return (-1);
	while (ptr[i] != NULL)
	{
		free(ptr[i]);
		i++;
	}
	free(ptr);
	return (0);
}

/*return possition char in stream*/
int	ft_pos_chr(const char *str, int c)
{
	int	i;

	i = 0;
	while (*str != (char) c)
	{
		if (!*str)
			return (-1);
		str++;
		i++;
	}
	return (i);
}

int	ft_smaller(int val_1, int val_2)
{
	if (val_1 <= val_2)
		return (val_1);
	return (val_2);
}
