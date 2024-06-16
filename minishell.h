/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexigar <alexigar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 16:17:26 by aiturria          #+#    #+#             */
/*   Updated: 2024/06/13 16:45:47 by alexigar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define NULL ((void *)0)

# include <stdio.h>	  // For printf
# include <unistd.h>  // For write // For close
# include <stdlib.h>  // For exit
# include <string.h>  // For strerror
# include <errno.h>   // For errno
# include <fcntl.h>   // For open
# include <sys/wait.h>
# include "Libft/libft.h"

# include <stdarg.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <dirent.h>	


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

//global funtions


//parsing
t_command **parser(char **tokens);

//build_in
int		ft_built_echo(t_command command);
int 	ft_built_cd(t_command command, t_list **env);
int 	ft_built_pwd(t_command *command);
int		ft_built_export(t_command *command, t_list **env);
int		ft_built_unset(t_command *command, t_list **env);
int		ft_built_env(t_command *command, t_list **env);

//Desarrollo
int		executor(t_command **command_list, t_list *env); //Recibir variables de entornos
void	free_commands(t_command **command_list); //libera la esructura commando
int		ft_build_int(t_list **env, t_command command_act); //Llama a la ejec de build_in
void	prom(t_list  **env) ; // LLama al prom después de inicializar las var entorno


/*Funciones gestión Variable de entorno*/
t_list  **ft_ini_env(char **environment);
char	**ft_get_env(t_list  **env);    
void    ft_del_v_env(char *var_env, t_list **env);//Borro si existe variable de entorno.
void    ft_add_v_env(char *var_env, t_list **env);//Añado variable de entorno.
char    *ft_get_var_env(t_list **env, char *var_env);
void 	ft_save_var_env(char *var, char *val_var, t_list **env); //guardo pasando variable y valor

/*Utils*/
void    ft_free_char(char **ptr); //Libera la memoria de un array de strings
void	ft_free_list(t_list **list); //Libera la memoria de la lista de strings
void	print_string(void *str); //Imprime un string Para usarlo con ft_lstiter
int		ft_pos_chr(const char *str, int c); //Devuelve la posición de un caracter en un string
char	*ft_strndup(const char *str, size_t n); ///ESTA FUNCION NO ESTA EN LA LIBRERIA LIBFT

/*lexer*/
char    **lexer(char *line); // Crea
void	free_token(char **token);
void	expander(char **token, t_list  **env);


#endif