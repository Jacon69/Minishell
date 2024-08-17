/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaimecondea <jaimecondea@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 19:11:01 by alexigar          #+#    #+#             */
/*   Updated: 2024/08/17 08:00:10 by jaimecondea      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*finds and changes the variable*/
int	ft_aux_add_v_env(char *var_env, t_list **env, char	*var)
{
	t_list	*p_env;
	char	*aux;

	p_env = *env;
	while (p_env)
	{
		if (ft_strncmp(p_env->content, var, ft_strlen(var)) == 0)
		{
			aux = p_env->content;
			p_env->content = ft_strdup(var_env);
			free(var);
			if (!p_env->content)
			{
				ft_free_list(env, "Mem error6 enviroment");
				exit(EXIT_FAILURE);
			}
			free(aux);
			return (1);
		}
		p_env = p_env->next;
	}
	return (0);
}

/*add enviroment variable*/
void	ft_add_v_env(char *var_env, t_list **env, int create)
{
	t_list	*p_env;
	char	*var;

	if (ft_memchr(var_env, '=', ft_strlen(var_env)) != NULL)
		var = ft_strndup((const char *)var_env,
				(size_t)ft_pos_chr(var_env, '='));
	else
		var = ft_strndup((const char *)var_env, ft_strlen(var_env));
	if (!var)
	{
		ft_free_list(env, "Mem error5 enviroment");
		exit(EXIT_FAILURE);
	}
	if (ft_aux_add_v_env(var_env, env, var) == 0 && create == 1)
	{
		p_env = ft_lstnew(ft_strdup(var_env));
		free(var);
		if (!p_env)
		{
			ft_free_list(env, "Mem error7 enviroment");
			return ;
		}
		ft_lstadd_back(env, p_env);
	}
}

/*guardo variable de entorno si existe o la crea si no existe */
void	ft_save_var_env(char *var, char *val_var, t_list **env, int create)
{
	char	*str_var;
	char	*aux;

	aux = ft_strjoin(var, "=");
	if (!aux)
	{
		perror("Error en malloc");
		exit(EXIT_FAILURE);
	}
	str_var = ft_strjoin(aux, val_var);
	free(aux);
	if (!str_var)
	{
		perror("Error en malloc");
		exit(EXIT_FAILURE);
	}
	ft_add_v_env(str_var, env, create);
	free(str_var);
}

/*get enviroment variable*/
char	*ft_get_var_env(t_list **env, char *var_env)
{
	t_list	*p_env;
	char	*var;
	char	*val_var;

	p_env = *env;
	var = NULL;
	while (p_env)
	{
		val_var = p_env->content;
		if (ft_strncmp(val_var, var_env, ft_strlen(var_env)) == 0
			&& ((val_var[ft_strlen(var_env)] == '=')))
		{
			var = ft_substr(val_var, ft_strlen(var_env) + 1,
					ft_strlen(val_var) - (ft_strlen(var_env) + 1));
			if (!var)
				return (NULL);
			return (var);
		}
		p_env = p_env->next;
	}
	var = malloc(sizeof(char));
	if (!var)
		return (NULL);
	var[0] = '\0';
	return (var);
}

/*delete enviroment variable*/
char	*ft_del_v_env(char *var_env, t_list **env)
{
	t_list	*p_env;
	t_list	*aux2;
	char	*str_aux;

	str_aux = ft_strdup((const char *)var_env);
	if (!str_aux)
	{
		ft_free_list(env, "Mem error4 enviroment");
		return (NULL);
	}
	p_env = *env;
	aux2 = NULL;
	while (p_env)
		ft_aux_del_v_env(env, &p_env, &aux2, str_aux);
	free(str_aux);
	return (NULL);
}
