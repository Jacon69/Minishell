/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_int2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jconde-a <jconde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 18:59:47 by alexigar          #+#    #+#             */
/*   Updated: 2024/08/09 20:31:31 by jconde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_checkwithout_n(char *cad)
{
	if (!ft_memcmp(cad, "-n", 2)
		&& ft_strlen(cad) == 2)
		return (1);
	return (0);
}

/* Implements the 'echo' command for the minishell.
   Prints text to standard output or a file.
   Supports the '-n' option and special character escaping.
*/
int	ft_built_echo(t_command *command)
{
	int	i;
	int	ok;

	ok = 0;
	if (!command -> args[1])
		return (ok);
	i = (1 + ft_checkwithout_n(command -> args[1]));
	while (command -> args[i])
	{
		if (0 > write(command -> file_output, command -> args[i],
				ft_strlen(command -> args[i])))
			return (1);
		if (command -> args[i + 1])
			write(command -> file_output, " ", 1);
		i++;
	}
	if (!ft_checkwithout_n(command -> args[1]))
		write(command -> file_output, "\n", 1);
	return (ok);
}

/* Changes the current working directory.
   Supports relative and absolute paths.
   Recognizes the ".." symbol as the previus directory.
   Returns 0 on success, or a non-zero value on error.*/
int	ft_built_cd(t_command *command, t_list **env)
{
	int				ok;
	t_struct_path	*dir;

	dir = malloc(sizeof(t_struct_path));
	if (dir == NULL)
		return (-1);
	dir->route = NULL;
	dir->line_path = NULL;
	dir->num_dir = 0;
	dir->path = NULL;
	ok = 0;
	if (ok != ft_aux1_buil_cd(command, dir))
		return (1);
	ok = ft_aux2_buil_cd(command, dir);
	ft_save_var_env("PWD", dir->route, env);
	chdir(dir->route);
	ft_save_var_env("OLDPWD", dir->line_path, env);
	ft_save_var_env("..PWD", dir->route, env);
	free(dir->route);
	ft_free_char(dir->path);
	free(dir);
	return (ok);
}

/* ft_built_pwd(t_command *command): Returns the absolute
path of the current directory. */
int	ft_built_pwd(t_command *command)
{
	int	ok;

	ok = write(command -> file_output, command -> path,
			ft_strlen(command -> path));
	ok *= write(command -> file_output, "\n", 1);
	if (ok < 1)
		ok = 1;
	else
		ok = 0;
	return (ok);
}
