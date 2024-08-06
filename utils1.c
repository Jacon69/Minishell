/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaimecondea <jaimecondea@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 11:45:27 by jaimecondea       #+#    #+#             */
/*   Updated: 2024/08/06 13:19:31 by jaimecondea      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	**ft_free_list(t_list **list, char *msg_err)
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
			return (ft_free_list(dup_env, NULL));
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


/* Devuelve una copia ordenada de la lista de var de entono*/
t_list	**ft_dup_ord_list(t_list **env)
{
	t_list	**dup_env;
	t_list	*p_env;
	t_list	*p_env_ori;

	p_env_ori = *env;
	if (!p_env_ori)
		return (NULL);
	dup_env = (t_list **)malloc(sizeof(t_list *));
	if (!dup_env)
		return (NULL);
	*dup_env = NULL;
	p_env = ft_lstnew(ft_strdup(p_env_ori->content));
	if (!p_env)
		return (ft_free_list(dup_env, NULL));
	ft_lstadd_back(dup_env, p_env);
	dup_env=ft_aux_dup_ord_list(dup_env, p_env, p_env_ori);
	return (dup_env);
}

int ft_export_print_list_env(t_command *command, t_list **env)
{
	t_list	**dup_env;

	dup_env = ft_dup_ord_list(env);
	if (!dup_env)
	{
		perror("Error MEM export");
		return (-1);
	}
	command -> command = "env";
	ft_print_list_env(command, dup_env);
	ft_free_list(dup_env,NULL);
	dup_env = NULL;
	return (1);
}

/*imprime las var lista entorno. Si export las ordena*/
int	ft_print_list_env(t_command *command, t_list **env)
{
	t_list	*p_env;
	int		ok;

	ok = 1;
	if (!ft_memcmp(command -> command, "export", 6))
		return (ft_export_print_list_env(command, env));
	p_env = *env;
	while (p_env && (ok == 1))
	{
		if (ft_strncmp("..", p_env -> content, 2) != 0)
		{
			ok *= write(command -> file_output,
					p_env -> content, ft_strlen(p_env -> content));
			ok *= write(command -> file_output, "\n", 1);
			if (ok > 0)
				ok = 1;
			else
				ok = -1;
		}
		p_env = p_env -> next;
	}

	return (ok);
}

/*comprueba su el dir existe*/
int	ft_is_dir_ok(const char *path)
{
	struct stat	statbuf;

	if (stat(path, &statbuf) != 0)
	{
		perror("stat");
		return (0);
	}
	if (S_ISDIR(statbuf.st_mode))
		return (1);
	else
		return (0);
}
