/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jconde-a <jconde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 10:19:58 by alexigar          #+#    #+#             */
/*   Updated: 2024/08/09 20:30:37 by jconde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_is_executing = 0;

int	count_nbr_tokens(char **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
		i++;
	return (i);
}

//Returns an ordered copy of the environment variables list
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
	dup_env = ft_aux_dup_ord_list(dup_env, p_env, p_env_ori);
	return (dup_env);
}

int	ft_export_print_list_env(t_command *command, t_list **env)
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
	ft_free_list(dup_env, NULL);
	dup_env = NULL;
	return (1);
}

//Prints the list of environment variables. If the command is export, orders them
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

//Checks if dir exists
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
