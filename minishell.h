/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jconde-a <jconde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 16:17:26 by aiturria          #+#    #+#             */
/*   Updated: 2024/08/16 11:07:58 by jconde-a         ###   ########.fr       */
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

// Variables Globales
extern int	g_is_executing;
extern int	g_exit;

typedef struct s_command
{
	int		index;
	char	*command;
	char	*path;
	char	**args;
	int		redir1; //0= NULL, < = 1, << = 2
	int		redir2; //0= NULL, > = 1, >> = 2
	int		piped; //0 no pipe, 1 there is pipe
	int		file_input;
	int		file_output;
	char	*input; //Input from files or pipes
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
t_command			**parser(char **tokens, t_list **env, int *flag);
int					count_nbr_tokens(char **tokens);
int					check_piped(t_command **list, int j, t_command **command);
void				check_pipe_token(char *token, t_command **command, int *i);
t_command			**free_commands(t_command **command_list);
t_command			*new_command(char **tokens, t_list **env, int i, int j);
t_command			**end_list(t_command *command, t_command **list, int *dup);
void				fill_args(char **tokens, int *i, t_command **com, int *k);
t_command			**ft_init_com_list(char **tokens);
void				write_in_heredoc(char *token, int *heredoc);
void				right_redir(char **tokens, int *i, t_command **command);
t_command			*left_redir(char **tokens, int *i, t_command **command);
void				put_redir1(t_command **command, char *token);
void				put_redir2(t_command **command, char *token);

//build_in
int					ft_built_echo(t_command *command);
int					ft_built_cd(t_command *command, t_list **env);
int					ft_cd_without_argument(t_command *command_act,
						t_list **env);
int					ft_aux1_buil_cd(t_command *command, t_struct_path *dir);
int					ft_aux2_buil_cd(t_command *command, t_struct_path *dir);
int					ft_built_pwd(t_command *command);
int					ft_built_export(t_command *command, t_list **env);
int					ft_built_unset(t_command *command, t_list **env);
int					ft_built_env(t_command *command, t_list **env);
int					ft_print_list_env(t_command *command, t_list **env);
int					ft_build_int(t_command *command, t_list **env);
int					ft_aux1_build_int(t_command *command_act, t_list **env);
int					is_built_in(char *command);

int					ft_equal_dir(t_struct_path *dir);
int					ft_minus_dir( t_struct_path *dir);
int					ft_cd_dir(t_command *command, t_struct_path *dir);
int					ft_check_export_arg(char *argument);

//Execution
int					executor(t_command **command_list, t_list **env);
t_command			**free_commands(t_command **command_list);
void				prom(t_list **env);

/*Environment variables*/
t_list				**ft_ini_env(char **environment);
char				**ft_get_env(t_list **env);
char				*ft_del_v_env(char *var_env, t_list **env);
void				ft_add_v_env(char *var_env, t_list **env, int create);
char				*ft_get_var_env(t_list **env, char *var_env);
void				ft_save_var_env(char *var, char *val_var, t_list **env,
						int create);
void				ft_aux_del_v_env(t_list **env, t_list **p_env,
						t_list **aux2, char *str_aux);

/*Utils*/
int					ft_free_char(char **ptr);
void				**ft_free_list(t_list **list, char *msg_err);
int					ft_pos_chr(const char *str, int c);
char				*ft_strndup(const char *str, size_t n);
int					ft_is_dir_ok(const char *path);
int					ft_exit(char *num_error);
/*lexer*/
char				**lexer(char *line, int *flag); // Creates
void				free_token(char **token);
int					expander(char **token, t_list **env);
int					ft_aux_assig_token(char *line, char **token,
						int *paran, int *flag);
int					ft_is_asignsymbol(char *line, int i);
int					ft_count_tokens(char *line);
int					add_chartotoken(char *line, char **token, int i);

/*executor*/
char				**get_paths(t_list **env);
char				*get_function_call(char *command, char *path);
void				free_function_call(t_command *com, char *function_call);
void				fail_fork(t_command *com, char *function_call);

t_list				**ft_aux_dup_ord_list(t_list **dup_env, t_list *p_env,
						t_list *p_env_ori);
int					ft_smaller(int val_1, int val_2);

void				ft_free_prom(t_list **env, char **token,
						t_command **commands, char *msg);
void				ft_save_last_return(int last_return, t_list **env);
struct sigaction	create_sigaction(void);
int					ft_free_prom2(t_list **env, char **token,
						t_command **commands, char *msg);
int					sigint_handler(void);
/*expander*/
char				*ft_token_exp(char *token, int j,
						char *var_env, int lng_var);
char				*ft_expander_home(char **token, t_list **env);
char				*ft_expander_pwd(char **token, t_list **env);
char				*ft_expander_home_pwd(char **token, t_list **env);
void				*ft_free_char_n(char *par1, char *par2,
						char *par3, char *par4);
char				*ft_expander_dollar(char **token, t_list **env);
char				*ft_expander_minuspwd(char **token, t_list **env);

#endif