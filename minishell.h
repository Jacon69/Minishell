/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jconde-a <jconde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 16:17:26 by aiturria          #+#    #+#             */
/*   Updated: 2024/08/04 09:58:21 by jconde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

//# define NULL ((void *)0)
//# define EXIT_FAILURE 1
//# define EXIT_SUCCESS 0	

# include <stdio.h>	  // For printf
# include <unistd.h>  // For write // For close
# include <stdlib.h>  // For exit
# include <string.h>  // For strerror
# include <errno.h>   // For errno
# include <fcntl.h>   // For open
# include <sys/wait.h>
# include <sys/stat.h> //  For use "stat()" en cd
# include "Libft/libft.h"
# include "get_next_line/get_next_line.h"

# include <stdarg.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <dirent.h>
# include <termios.h>

extern int	g_is_executing; //variable  global

typedef struct s_command
{
	int		index;
	char	*command;
	char	*path;
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

typedef struct s_path
{
	char	*route;
	char	*line_path;
	int		num_dir;
	char	**path;

}	t_struct_path;

//global funtions
//parsing
t_command	**parser(char **tokens, t_list **env);
int			count_nbr_tokens(char **tokens);
char		*read_all(int fd);

//build_in
int			ft_built_echo(t_command *command);
int			ft_built_cd(t_command *command, t_list **env);
int			ft_aux1_buil_cd(t_command *command, t_struct_path *dir);
int 		ft_aux2_buil_cd(t_command *command, t_struct_path *dir);
int			ft_built_pwd(t_command *command);
int			ft_built_export(t_command *command, t_list **env);
int			ft_built_unset(t_command *command, t_list **env);
int			ft_built_env(t_command *command, t_list **env);
int			ft_print_list_env(t_command *command, t_list **env);
int			ft_build_int(t_command *command, t_list **env);

//Desarrollo
int			executor(t_command **command_list, t_list **env);
void		free_commands(t_command **command_list);
void		prom(t_list **env);

/*Funciones gestión Variable de entorno*/
t_list		**ft_ini_env(char **environment);
char		**ft_get_env(t_list **env);
char		*ft_del_v_env(char *var_env, t_list **env);
void		ft_add_v_env(char *var_env, t_list **env);
char		*ft_get_var_env(t_list **env, char *var_env);
void		ft_save_var_env(char *var, char *val_var, t_list **env);

/*Utils*/
int			ft_free_char(char **ptr);
t_list		**ft_free_list(t_list **list, char *msg_err);
char		**ft_free_list_r_char(t_list **list, char *msg_err);
void		print_string(void *str);
int			ft_pos_chr(const char *str, int c);
char		*ft_strndup(const char *str, size_t n);
int			ft_is_dir_ok(const char *path);
char		*ft_free_char_n(char *par1, char *par2, char *par3, char *par4);

/*lexer*/
char		**lexer(char *line); // Crea
void		free_token(char **token);
int			expander(char **token, t_list **env);

char		*read_all(int fd);
#endif