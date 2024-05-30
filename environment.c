#include "environment.h"
#include "./Libft/libft.h"



char	**ft_get_env(t_list  **env)
{
	t_list *p_env;
	char **var_entorno;
	int i;

	p_env = *env;
	i = 0;
	while (p_env)
	{
		i++;
		p_env = p_env->next;
	}
	var_entorno = (char **)malloc(sizeof(char *) * (i + 1)); //Hago Malloc
	if (!var_entorno)
	{
		perror("Error en malloc");
		ft_free_list(env);
		return NULL;
	}
	i = 0;
	p_env = *env;
	while (p_env)
	{
		var_entorno[i] = ft_strdup(p_env->content); //Malloc
		if (!var_entorno[i]) //Protejo malloc
		{
			ft_free_char(var_entorno);
			ft_free_list(env);
			perror("Error en malloc");
			return NULL;
		}
		i++;
		p_env = p_env->next;
	}
	var_entorno[i] = NULL;
	return (var_entorno);
}


t_list **ft_ini_env(char **environment)
{
    int i;

    t_list **env;
    t_list  *p_env;

    env = (t_list **)malloc(sizeof(t_list *));  //Hago Malloc
    if (!env) //Protejo malloc
	{
		perror("Error en malloc");
        return NULL;
	}
	i = 0;
	p_env = ft_lstnew(ft_strdup(environment[i])); //Hago Malloc
	if (!p_env)  //Protejo malloc
	{
		free(env);
		perror("Error en malloc");
		return NULL;
	}
	ft_lstadd_back(env, p_env);
    while (environment[++i])
    {
        p_env = ft_lstnew(ft_strdup(environment[i])); //Hago Malloc
        ft_lstadd_back(env, p_env);
		if (!p_env)  //Protejo malloc
		{
			ft_free_list(env); //Libero la memoria de la lista de variables de entorno y dejo env apuntando NULL
			return NULL;
		}
	}
 	return env;
}

void ft_del_v_env(char *var_env, t_list **env)//Borro si existe variable de entorno.
{
	t_list *p_env;
	t_list *aux;
	t_list *aux2; //previo
	char *str_aux;
	size_t nlong;

	str_aux = ft_strdup((const char *)var_env); //Hago Malloc
	if (!str_aux) //Protejo malloc
	{
		ft_free_list(env);
		perror("Error en malloc");
		return ;
	}

	printf("str_aux: %s\n", str_aux);
	p_env = *env;
	aux2 = NULL;
	while (p_env)
	{
		nlong  = ft_pos_chr(p_env->content, '='); //Busco la posición del igual
		if ((ft_strncmp(p_env->content, str_aux, ft_strlen(str_aux)) == 0) && (nlong == ft_strlen(str_aux))) //Si la variable de entorno existe la borro
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

void ft_add_v_env(char *var_env, t_list **env)//Añado variable de entorno. Lo hago con export
{
	t_list *p_env;
	char *aux;
	char *var;

	var = ft_strndup((const char *)var_env, (size_t)ft_pos_chr(var_env, '=')); //Hago Malloc
	if (!var) //Protejo malloc
	{
		ft_free_list(env);
		perror("Error en malloc");
		return ;
	}
	p_env = *env;
	while (p_env)
	{
		if (ft_strncmp(p_env->content, var, ft_strlen(var)) == 0) //Recorro la lista para ver si ya existe la variable de entorno
		{
			aux= p_env->content;
			p_env->content = ft_strdup(var_env); //Hago Malloc
			if (!p_env->content) //Protejo malloc
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
	p_env = ft_lstnew(ft_strdup(var_env)); //Hago Malloc
	if (!p_env) //Protejo malloc
	{
		ft_free_list(env); //Libero la memoria de la lista de variables de entorno y dejo env apuntando NULL
		free(var);
		perror("Error en malloc");
		return ;
	}
	free(var);
	ft_lstadd_back(env, p_env); //Aquí añado el nodo nuevo a la lista
}

void	ft_pwd(t_list **env)
{
	t_list	*p_env;
	char 	*var;

	p_env = *env;
	while (p_env)
	{
		if (ft_strncmp(p_env->content, "PWD", 3) == 0) //Recorro la lista para ver si ya existe la variable de entorno
		{
			var = p_env->content;
			var = var + 4;
			ft_printf("%s\n", var);
			
			return ;
		}
	p_env = p_env->next;
	}
}
