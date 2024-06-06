#ifndef ENVIRONMENT_H
# define ENVIRONMENT_H

# include <unistd.h>
# include <stdlib.h>
# include <stdarg.h>
# include <stdio.h>
# include <string.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "./Libft/libft.h"

void prom(t_list  **env) ;

/*Funciones gestión Variable de entorno*/
t_list  **ft_ini_env(char **environment);
char	**ft_get_env(t_list  **env);    
void    ft_del_v_env(char *var_env, t_list **env);//Borro si existe variable de entorno.
void    ft_add_v_env(char *var_env, t_list **env);//Añado variable de entorno.
void	ft_pwd(t_list **env); //Imprime el directorio actual



/*Utils*/
void    ft_free_char(char **ptr); //Libera la memoria de un array de strings
void	ft_free_list(t_list **list); //Libera la memoria de la lista de strings
void	print_string(void *str); //Imprime un string Para usarlo con ft_lstiter
int		ft_pos_chr(const char *str, int c); //Devuelve la posición de un caracter en un string
char	*ft_strndup(const char *str, size_t n); ///ESTA FUNCION NO ESTA EN LA LIBRERIA LIBFT

/*lexer*/
char    **lexer(char *line); // Crea
void	free_token(char **token);





#endif