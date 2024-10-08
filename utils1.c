/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaimecondea <jaimecondea@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 11:45:27 by jaimecondea       #+#    #+#             */
/*   Updated: 2024/08/16 20:02:23 by jaimecondea      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*Free the list of enviroment variable*/
void	**ft_free_list(t_list **list, char *msg_err)
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
	}
	if (msg_err)
		perror(msg_err);
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
	while (*str != (char) c && str)
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
