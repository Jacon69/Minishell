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
# include <dirent.h>	
# include "./Libft/libft.h"
# include "./get_next_line/get_next_line.h"
//De la estructura de alexi

typedef struct s_command
{
	int		index;
	char	*command;
	char	path[1024];
	char	**args;
	int		redir1; //0= NULL, < = 1, << = 2
	int		redir2; //0= NULL, > = 1, >> = 2
	int		piped; //0 no hay pipe, 1 hay pipe (salida)
	int		file_input;
	int		file_output;
	char	*input; //Entrada desde archivos o pipes
	char	*string_output;
	int		returned_output;
}	t_command;

typedef struct s_myshell
{
	t_list 		**env;
	char		*actual_dir;
	int			nbr_command;
	int			pipe;
	int			last_status;
	t_command	*commands;
}	t_myshell;


// ft_build_int(t_myshell shell);

void prom(t_list  **env) ;

/*Funciones gestión Variable de entorno*/
t_list  **ft_ini_env(char **environment);
char	**ft_get_env(t_list  **env);    
void    ft_del_v_env(char *var_env, t_list **env);//Borro si existe variable de entorno.
void    ft_add_v_env(char *var_env, t_list **env);//Añado variable de entorno.
void	ft_pwd(t_list **env); //Imprime el directorio actual
char    *ft_get_var_env(t_list **env, char *var_env);



/*Utils*/
void    ft_free_char(char **ptr); //Libera la memoria de un array de strings
void	ft_free_list(t_list **list); //Libera la memoria de la lista de strings
void	print_string(void *str); //Imprime un string Para usarlo con ft_lstiter
int		ft_pos_chr(const char *str, int c); //Devuelve la posición de un caracter en un string
char	*ft_strndup(const char *str, size_t n); ///ESTA FUNCION NO ESTA EN LA LIBRERIA LIBFT
int		count_tokens(char **tokens); //Cuenta el numero de tokens

/*lexer*/
char    **lexer(char *line); // Crea
void	free_token(char **token);
void	expander(char **token, t_list  **env);

int		ft_built_echo(t_command command);
int		ft_built_cd(t_command command, t_list **env);


#endif