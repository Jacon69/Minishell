/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiturria <aiturria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 16:17:26 by aiturria          #+#    #+#             */
/*   Updated: 2024/05/30 10:32:20 by aiturria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>	  // For printf
# include <unistd.h>  // For write
# include <stdlib.h>  // For exit
# include <string.h>  // For strerror
# include <errno.h>   // For errno
# include <fcntl.h>   // For open
# include <unistd.h>  // For close
# include "libft/libft.h"


typedef struct s_command
{
	int		index;
	char	*command[50];
	char	*path;
	char	*string;
	int		redir1; //0= NULL, < = 1, << = 2
	int		redir2; //0= NULL, > = 1, >> = 2
	int		file;
	char	*input;
	char	*output;
	int		infile;
	int		outfile;
	int		singleqts;
	int		dobleqts;
}	t_command;

typedef struct s_myshell
{
	char		**env;
	char		*actual_dir;
	int			nbr_command;
	int			pipe;
	int			last_status;
	t_command	*commands;
}	t_myshell;

//global funtions
void	ms_error(t_myshell *myshell, char *error);
void	ms_freeall(t_myshell *myshell);

//parsing
void	ms_lineprocessing(t_myshell *myshell, char *line);
void	getcommands(t_myshell *myshell, t_command *command);
char	*ms_findtoken(t_myshell *myshell, char *string, int *len);
void	ms_redirections(t_command *command, char *token, int *len2);
void	ms_savewords(t_myshell *myshell, t_command *command, char *token,
			int *len2);

#endif