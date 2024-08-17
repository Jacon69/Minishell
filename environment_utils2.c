/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_utils2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaimecondea <jaimecondea@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 19:11:53 by alexigar          #+#    #+#             */
/*   Updated: 2024/08/17 07:59:09 by jaimecondea      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_item(t_list *p_aux)
{
	free(p_aux->content);
	free(p_aux);
}

void	ft_aux_del_v_env(t_list **env, t_list **p_env
			, t_list **aux2, char *str_aux)
{
	t_list	*aux;
	t_list	*p_aux;
	t_list	*p_aux2;

	p_aux = *p_env;
	p_aux2 = *aux2;
	if ((ft_strncmp(p_aux->content, str_aux, ft_strlen(str_aux)) == 0)
		&& ((ft_pos_chr(p_aux->content, '=') == (int)ft_strlen(str_aux))
			|| ft_strlen(str_aux) == ft_strlen(p_aux->content)))
	{
		aux = p_aux->next;
		ft_free_item(p_aux);
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

char	*ft_join_to_list(char *content, char *var_env)
{
	char	*result;
	char	*aux;

	aux = ft_substr(var_env, (unsigned int)ft_pos_chr(var_env, '=') + 1,
			ft_strlen(var_env) - (size_t)ft_pos_chr(var_env, '=') - 1);
	result = ft_strjoin(content, aux);
	free(aux);
	if (!result)
		return (NULL);
	return (result);
}

/*finds and join the variable*/
int	ft_aux_join_v_env(char *var_env, t_list **env, char	*var)
{
	t_list	*p_env;
	char	*aux;

	p_env = *env;
	while (p_env)
	{
		if (ft_strncmp(p_env->content, var, ft_strlen(var)) == 0)
		{
			aux = p_env->content;
			p_env->content = ft_join_to_list(p_env->content, var_env);
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

/* += join enviroment variable*/
void	ft_join_v_env(char *var_env, t_list **env, int create)
{
	t_list	*p_env;
	char	*var;

	var = ft_strndup((const char *)var_env,
			(size_t)ft_pos_chr(var_env, '+'));
	if (!var)
	{
		ft_free_list(env, "Mem error5 enviroment");
		exit(EXIT_FAILURE);
	}
	if (ft_aux_join_v_env(var_env, env, var) == 0 && create == 1)
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
