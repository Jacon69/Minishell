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
			return (ft_free_list_r_char(env, "Mem error2 enviroment"));
		}
		i++;
		p_env = p_env->next;
	}
	var_entorno[i] = NULL;
	return (var_entorno);

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
		return (ft_free_list_r_char(env, "Mem error1 enviroment"));
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
		return (ft_free_list(env, "Mem error2.5 enviroment"));
	ft_lstadd_back(env, p_env);
	while (environment[++i])
	{
		p_env = ft_lstnew(ft_strdup(environment[i]));
		ft_lstadd_back(env, p_env);
		if (!p_env)
			return (ft_free_list(env, "Mem error3 enviroment"));
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
	if (*environment == NULL)
	{
		ft_ini_empty_env(env);
		return (env);
	}
	env = ft_aux_ini_env(environment, env);
	ft_save_var_env("..PWD", ft_get_var_env(env, "PWD"), env);
	return (env);
}

char	*ft_aux_del_v_env(t_list **env, t_list	**p_env, t_list	**aux2, char *str_aux)
{
	t_list	*aux;
	size_t	nlong;
	t_list	*p_aux;
	t_list	*p_aux2;

	p_aux = *p_env;
	p_aux2 = *aux2;

	nlong = ft_pos_chr(p_aux->content, '=');
	if ((ft_strncmp(p_aux->content, str_aux, ft_strlen(str_aux)) == 0)
		&& (nlong == ft_strlen(str_aux)))
	{
	printf("poraquí %s  \n", (char*)p_aux->content);
		aux = p_aux->next;
		free(p_aux->content);
		free(p_aux);
		p_aux = aux;
		if (p_aux2)
		{
			p_aux2->next = p_aux;
			*aux2  = p_aux2;
			*p_env = aux;
		}
		else
			*env = p_aux;
		return (NULL);
	}
	*aux2 = p_aux;
	*p_env = p_aux->next;
	return (NULL);
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
		ft_free_list_r_char(env, "Mem error4 enviroment");
		return (NULL);
	}
	p_env = *env;
	aux2 = NULL;
	while (p_env)
	{

		ft_aux_del_v_env(env, &p_env, &aux2, str_aux);
	}
	free(str_aux);
	return (NULL);

}

/*get enviroment variable*/
char	*ft_get_var_env(t_list **env, char *var_env)
{
	t_list	*p_env;
	char	*var;
	char	*val_var;

	p_env = *env;
	while (p_env)
	{
		val_var = p_env->content;
		if (ft_strncmp(val_var, var_env, ft_strlen(var_env)) == 0
			&& ((val_var[ft_strlen(var_env)]=='=')))
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

/*encuentra y sustituye la variable*/
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
				return (-1);
			}
			free(aux);
			return (1) ;
		}
		p_env = p_env->next;
	}
	return(0);
}

/*add enviroment variable*/
void	ft_add_v_env(char *var_env, t_list **env)
{
	t_list	*p_env;
	char	*var;
	int 	back;

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
	back = ft_aux_add_v_env(var_env, env, var);
	if (back == -1)
		exit(EXIT_FAILURE);
	if (back == 0)
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
