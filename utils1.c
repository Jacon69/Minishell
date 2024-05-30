#include "environment.h"

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