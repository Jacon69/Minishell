#include "minishell.h"

static void	ft_ini_empty_env(t_list **env)
{
	char	buffer[1024];

	if (getcwd(buffer, sizeof(buffer)) != NULL)
	{
		ft_save_var_env("PWD", buffer, env);
		ft_save_var_env("SHLVL", "1", env);
		ft_save_var_env("..PWD", ft_get_var_env(env, "PWD"), env);
	}
	else
		perror("getcwd() error");
}

// función que convierte list de var-env en doble cadena de  var-env
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
	{
		perror("Error en malloc");
		ft_free_list(env);
		return (NULL);
	}
	i = 0;
	p_env = *env;
	while (p_env)
	{
		var_entorno[i] = ft_strdup(p_env->content);
		if (!var_entorno[i])
		{
			ft_free_char(var_entorno);
			ft_free_list(env);
			perror("Error en malloc");
			return (NULL);
		}
		i++;
		p_env = p_env->next;
	}
	var_entorno[i] = NULL;
	return (var_entorno);
}

t_list	**ft_ini_env(char **environment)
{
	int		i;
	t_list	**env;
	t_list	*p_env;

	env = (t_list **)malloc(sizeof(t_list *));
	if (!env)
	{
		perror("Error en malloc");
		return (NULL);
	}
	if (*environment == NULL)
	{
		ft_ini_empty_env(env);
		return (env);
	}
	i = 0;
	p_env = ft_lstnew(ft_strdup(environment[i]));
	if (!p_env)
	{
		free(env);
		perror("Error en malloc");
		return (NULL);
	}
	ft_lstadd_back(env, p_env);
	while (environment[++i])
	{
		p_env = ft_lstnew(ft_strdup(environment[i]));
		ft_lstadd_back(env, p_env);
		if (!p_env)
		{
			ft_free_list(env);
			return (NULL);
		}
	}
	ft_save_var_env("..PWD", ft_get_var_env(env, "PWD"), env);
	return (env);
}

void	ft_del_v_env(char *var_env, t_list **env)
{
	t_list	*p_env;
	t_list	*aux;
	t_list	*aux2;
	char	*str_aux;
	size_t	nlong;

	str_aux = ft_strdup((const char *)var_env);
	if (!str_aux)
	{
		ft_free_list(env);
		perror("Error en malloc");
		return ;
	}
	p_env = *env;
	aux2 = NULL;
	while (p_env)
	{
		nlong = ft_pos_chr(p_env->content, '=');
		if ((ft_strncmp(p_env->content, str_aux, ft_strlen(str_aux)) == 0)
			&& (nlong == ft_strlen(str_aux)))
		{
			aux = p_env->next;
			free(p_env->content);
			free(p_env);
			p_env = aux;
			if (aux2)
				aux2->next = p_env;
			else
				*env = p_env;
		}
		else
		{
			aux2 = p_env;
			p_env = p_env->next;
		}
	}
	free(str_aux);
}

char	*ft_get_var_env(t_list **env, char *var_env)
{
	t_list	*p_env;
	char	*var;

	p_env = *env;
	while (p_env)
	{
		if (ft_strncmp(p_env->content, var_env, ft_strlen(var_env)) == 0)
		{
			var = ft_substr(p_env -> content, ft_strlen(var_env) + 1,
					ft_strlen(p_env->content) - (ft_strlen(var_env) + 1));
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

void	ft_add_v_env(char *var_env, t_list **env)
{
	t_list	*p_env;
	char	*aux;
	char	*var;

	if (ft_memchr(var_env, '=', ft_strlen(var_env)) != NULL)
		var = ft_strndup((const char *)var_env,
				(size_t)ft_pos_chr(var_env, '='));
	else
		var = ft_strndup((const char *)var_env, ft_strlen(var_env));
	if (!var)
	{
		ft_free_list(env);
		perror("Error en malloc");
		exit(EXIT_FAILURE);
	}
	p_env = *env;
	while (p_env)
	{
		if (ft_strncmp(p_env->content, var, ft_strlen(var)) == 0)
		{
			aux = p_env->content;
			p_env->content = ft_strdup(var_env);
			if (!p_env->content)
			{
				free(var);
				ft_free_list(env);
				perror("Error en malloc");
				return ;
			}
			free(aux);
			free(var);
			return ;
		}
		p_env = p_env->next;
	}
	p_env = ft_lstnew(ft_strdup(var_env));
	if (!p_env)
	{
		ft_free_list(env);
		free(var);
		perror("Error en malloc");
		return ;
	}
	free(var);
	ft_lstadd_back(env, p_env);
}

void	ft_save_var_env(char *var, char *val_var, t_list **env)
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
	ft_add_v_env(str_var, env);
	free(str_var);
}
