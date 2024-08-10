/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jconde-a <jconde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 11:45:02 by jaimecondea       #+#    #+#             */
/*   Updated: 2024/08/10 17:04:26 by jconde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*inicializo cuando no recibo variables de entorno*/
static void	ft_ini_empty_env(t_list **env)
{
	char	buffer[1024];

	if (getcwd(buffer, sizeof(buffer)) != NULL)
	{
		ft_save_var_env("PWD", buffer, env);
		ft_save_var_env("SHLVL", "1", env);
		ft_save_var_env("..PWD", ft_get_var_env(env, "PWD"), env);
		ft_save_var_env("..HOME", ft_get_var_env(env, "PWD"), env);
	}
	else
		perror("getcwd() error");
}

char	**ft_aux_get_env(t_list **env, char	**var_entorno)
{
	t_list	*p_env;
	int		i;

	i = 0;
	p_env = *env;
	while (p_env)
	{
		var_entorno[i] = ft_strdup(p_env->content);
		if (!var_entorno[i])
		{
			ft_free_char(var_entorno);
			return ((char **)ft_free_list(env, "Mem error2 enviroment"));
		}
		i++;
		p_env = p_env->next;
	}
	var_entorno[i] = NULL;
	return (var_entorno);
}

//converts a list of var-env into double chain of var-env
char	**ft_get_env(t_list **env)
{
	t_list	*p_env;
	char	**var_entorno;
	int		i;

	p_env = *env;
	i = 0;
	while (p_env)
	{
		i++;
		p_env = p_env->next;
	}
	var_entorno = (char **)malloc(sizeof(char *) * (i + 1));
	if (!var_entorno)
		return ((char **)ft_free_list(env, "Mem error1 enviroment"));
	var_entorno = ft_aux_get_env(env, var_entorno);
	if (!var_entorno)
		return (NULL);
	return (var_entorno);
}

t_list	**ft_aux_ini_env(char **environment, t_list	**env)
{
	int		i;
	t_list	*p_env;

	i = 0;
	p_env = ft_lstnew(ft_strdup(environment[i]));
	if (!p_env)
		return ((t_list **)ft_free_list(env, "Mem error2.5 enviroment"));
	ft_lstadd_back(env, p_env);
	while (environment[++i])
	{
		p_env = ft_lstnew(ft_strdup(environment[i]));
		ft_lstadd_back(env, p_env);
		if (!p_env)
			return ((t_list **)ft_free_list(env, "Mem error3 enviroment"));
	}
	return (env);
}

/*inicializate enviroment variable*/
t_list	**ft_ini_env(char **environment)
{
	t_list	**env;

	env = (t_list **)malloc(sizeof(t_list *));
	if (!env)
		ft_free_list(NULL, "Mem error 2 enviroment");
	env[0] = NULL;
	if (*environment == NULL)
	{
		ft_ini_empty_env(env);
		return (env);
	}
	env = ft_aux_ini_env(environment, env);
	ft_save_var_env("..PWD", ft_get_var_env(env, "PWD"), env);
	ft_save_var_env("..HOME", ft_get_var_env(env, "HOME"), env);
	return (env);
}
