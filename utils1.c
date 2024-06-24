#include "minishell.h"

void ft_free_list(t_list **list) //Libera la memoria de la lista de variables de entorno
{
	t_list *p_nodo;
	t_list *aux;

	p_nodo = *list;
	while (p_nodo)
	{
		aux = p_nodo->next;
		free(p_nodo->content);
		free(p_nodo);
		p_nodo = aux;
	}
	*list = NULL;  //Aquí dejo la lista apuntando a NULL en vez de la posición que ya está liberada
}

void ft_free_char(char **ptr) //Libera la memoria de un array de strings
{
	int	i;

	i=0;
	if (!ptr)
		return ;
	while (ptr[i] != NULL)
	{
		free(ptr[i]);
		i++;
	}
	free(ptr);
}

void print_string(void *str) 
{
	printf("%s\n", (char *)str);
}

int	ft_pos_chr(const char *str, int c)
{
	int    i;

	i = 0;
	while (*str != (char) c)
	{
		if (!*str)
			return (-1); //Devuelvo -1 si no encuentro el caracter
		str++;
		i++;
	}
	return (i);
}

int	ft_smaller(int val_1,int val_2)
{
	return((val_1 <= val_2) ? val_1 : val_2);
}

//duplica la cadena que se le pase  hasta n caracteres si n es mayor duplica la cadena completa
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
	//dup = (char *)calloc(len + 1, sizeof(char));
	if (dup == NULL)
		return (NULL);
	ft_memcpy(dup, str, len);
	dup[len] = '\0';
	return (dup);
}
/*Ordena una copia de la lista de var entorno alfabeticamente*/
t_list **ft_dup_ord_list(t_list **env)
{
	t_list **dup_env;
	t_list *p_env;
	t_list *p_env_aux;
	t_list *p_env_ori;
	t_list **ref_previa;
	int exit_bucle;


		
	
	p_env_ori = *env;
	if (!p_env_ori)
		return NULL;
	dup_env = (t_list **)malloc(sizeof(t_list *)); // Hago Malloc
	if (!dup_env)								   // Protejo malloc
	{
		perror("Error en malloc");
		return NULL;
	}
	

	p_env = ft_lstnew(ft_strdup(p_env_ori->content)); // Hago Malloc
	if (!p_env)									  // Protejo malloc
	{
		ft_free_list(dup_env);
		perror("Error en malloc");
		return NULL;
	}
	ft_lstadd_back(dup_env, p_env);
	while (p_env_ori->next )
	{
		p_env_ori=p_env_ori->next;//para el próximo bucle
		p_env_aux = ft_lstnew(ft_strdup(p_env_ori->content));//Malloc
		if (!p_env_aux)									  // Protejo malloc
		{
			ft_free_list(dup_env);
			perror("Error en malloc");
			return NULL;
		}
		p_env= *dup_env;
		if (ft_strncmp(p_env->content,p_env_aux->content, ft_smaller(ft_strlen(p_env->content),ft_strlen(p_env_aux->content))) > 0 )// esto es p_env es mayor ejempli B-A
		{
			*dup_env = p_env_aux;  // como es el primero apunto la lista a el nodo que añadimos 
			p_env_aux->next=p_env;
			continue;
		}
		else if(p_env->next==NULL)  /// A-B y no hay mas por lo que ponfo el nodo en su posicion y sigo el bucle
		{
			p_env->next=p_env_aux;
			continue;
		}
		exit_bucle = 1;	
		while (p_env->next && (exit_bucle == 1)) //Aqui entramos a un while para ir buscando la posición de ap_env-aux ya que A-B y hay nodos después
		{
			exit_bucle = 1;	
			ref_previa= &p_env->next;  //guardo la dir del next previo para poder modificarlo cuando metamos ap_env_aux
			p_env=p_env->next;

			if (ft_strncmp(p_env->content,p_env_aux->content, ft_smaller(ft_strlen(p_env->content),ft_strlen(p_env_aux->content))) > 0 )
			{
				*ref_previa=p_env_aux;  //aquí estamos intercalando
				p_env_aux->next=p_env;
				exit_bucle = 0;
			}
			else if(p_env->next==NULL)   //aqui lo ponemos al final
			{
				p_env->next=p_env_aux;
				exit_bucle = 0;
			}
			// Si sigo en el bucle es que p_env_aux es menor alfabeticamente que p_env y avanzo al siguiente nodo
		}
	}
	return dup_env;
}

/*imprime la lista de var de entorno desde env y export*/
int ft_print_list_env(t_command *command, t_list **env) //si hay error devuelve 0 type 0=export 1=env
{
	t_list **dup_env;
	t_list *p_env;
	int ok;

	ok=1;
	if (!ft_memcmp(command->command, "export",6))
	{
		dup_env = ft_dup_ord_list(env); //malloc
		if (!dup_env)
			return (0);
		command->command="env";
		ft_print_list_env(command,dup_env );
		ft_free_list(dup_env);
		return(1);		
	}
	p_env = *env;
	while (p_env )
	{
		ok *= write(command->file_output,p_env->content, ft_strlen(p_env->content));
		ok *= write(command->file_output,"\n", 1);
		if (ok > 0)
			ok = 1;
		else 
			ok = -1;
		p_env= p_env->next;
	}
	return((ok <= 0) ? 1 : 0);
}