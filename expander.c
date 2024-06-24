#include "minishell.h"

static char *ft_expander_home(char *token, t_list  **env) 
{
	char	*var_env;
	int		i;
	int		j;
	char	*aux;
	char	*aux1;
	char	*aux2;
	int 	var_long;
	
	i = 0 ;
	token = ft_strdup(token);//Malloc duplico porque en origen libero token original
	if (!token)
		return NULL;
	while (token[i])
	{
		
		if ((token[i] == '~' && token[i+1] == '/' )||  (token[i] == '~' && ft_strlen(token) == 1 ))
		{
			j = i;
			var_long = 1;
			i++;
						
			var_env = ft_get_var_env(env, "HOME"); //Malloc
			
			
			if (!var_env) //esto es si falla malloc
			{
				free(token);
				return NULL;
			}	
			aux = token; //para liberar
			aux1 = ft_substr(token, 0, j ); //lo que va por delante // Malloc
			if (!aux1)
			{
				free(token);
				free(var_env)
				return NULL;
			}
			aux2 = ft_substr(token, j + var_long , ft_strlen(token)-(j + var_long) ); // lo que va por detrás // Malloc
			if (!aux2)
			{
				free(aux1);
				free(token);
				free(var_env)
				return NULL;
			}
			token = ft_strjoin (aux1, var_env);
			free (aux);
			if (!token)
			{
				free (aux1);
				free (aux2);
				free(var_env);
				return NULL
			}

			aux = token;
			token = ft_strjoin(token,aux2);
			free (aux);
			free (aux1);
			free (aux2);
			free(var_env);
			if (!token)
				return NULL
		}
		i++;	
	}
	return (token);
}

static char *ft_expander_$(char *token, t_list  **env) 
{
	char	*var_env;
	int		i;
	int		j;
	char	*aux;
	char	*aux1;
	char	*aux2;
	int 	var_long;
	
	i = 0 ;
	token = ft_strdup(token);//malloc duplico porque en origen libero token original //
	if (!token)
		return NULL;
	while (token[i])
	{
		
		if (token[i] == '$')
		{
			j = i;
			var_long = 0;
			while (token[i] && token[i]!= ' ') //cuento tamaño de lo que hay que expandir
			{
				var_long++;
				i++;
			}
			aux = ft_substr(token, j + 1, var_long - 1 ) ; // Malloc Lo que viene después de dolar puedo no existir entonces no se hace nada se sustituye por nada
			if (!aux) //esto es si falla malloc
			{
				free(token);
				return NULL;
			}
				
			var_env = ft_get_var_env(env, aux); //Malloc
			free(aux);
			if (!var_env) //esto es si falla malloc
			{
				free(token);
				return NULL;
			}
			
			aux = token; //para liberar

			aux1 = ft_substr(token, 0, j ); //lo que va por delante // Malloc
			if (!aux1)
			{
				free(token);
				free(var_env);
				return NULL;
			}
			aux2 = ft_substr(token, j + var_long , ft_strlen(token)-(j + var_long) ); // lo que va por detrás // Malloc
			if (!aux2)
			{
				free(aux1);
				free(token);
				free(var_env);
				return NULL;
			}		
			token = ft_strjoin (aux1, var_env);//malloc
			free (aux);
			free(var_env);
			if (!token)
			{
				free (aux1);
				free (aux2);				
				return NULL;
			}
			aux = token;
			token = ft_strjoin(token,aux2); //malloc
			free (aux);
			free (aux1);
			free (aux2);
			if (!token)
				return NULL;
			aux = token;
			token = ft_expander_$(token, env); //malloc
			free (aux);
			if (!token)
				return NULL;
		}
		i++;	
	}
	return (token);
}


static char *ft_expander_q(char *token, t_list  **env) //Aquí le quito las comillas antes de expandirlo
{
	char	*aux;
	char	*aux2;
	int		lng_token;


	lng_token = ft_strlen(token);
	aux = ft_substr(token,1,lng_token-2 ); //Malloc
	if(!aux)
		return NULL;
	if (token[0] =='\'')  //Si es comillas simples no expando.
		return (aux);
	aux2 = ft_expander_$(aux, env);  //malloc
	free(aux);
	if (!aux2)
		return NULL;
	return (aux2);
}

//Si devuelve 1 ok o fallo
int	expander(char **token, t_list  **env)
{
	int 	i;
	char	*aux;

	i = 0;
	while(token[i])
	{
	
		if (token[i][0]=='"' || token[i][0] =='\'')
		{
			aux = token[i];
			token[i]=ft_expander_q(token[i], env);  //malloc
			if (!token[i])
				return(0);  
			free(aux);
		}
		else
		{
			aux = token[i];
			token[i]=ft_expander_$(token[i], env);  //malloc			
			if (!token[i])
				return(0);
			free(aux);
			aux = token[i];
			token[i]=ft_expander_home(token[i], env);  //malloc
			if (!token[i])
				return(0);
			free(aux);
		}
		i++;
	}
}