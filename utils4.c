/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jconde-a <jconde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 17:38:21 by jconde-a          #+#    #+#             */
/*   Updated: 2024/08/27 18:03:01 by jconde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

/*Check the stream is only number. 1 true*/
int	ft_chck_nbs(char *numero)
{
	if (*numero == '-')
		numero++;
	while (*numero != '\0')
	{
		if (!ft_isdigit(*numero))
			return (0);
		numero++;
	}
	return (1);

}

int	ft_count_argument(char **arg)
{
	int	count;

	count = 0;

	while (arg[count])
		count++;
	return (--count);
}

/*convert arg[1] from command exit to number for exit*/
int	ft_exit(t_command *command_act)
{
	int	nerror;
	int narg;

	g_exit = 0;
	if (!ft_chck_nbs(command_act -> args[1]))
	{
		printf("exit: %s numeric argument required\n", command_act -> args[1]);
		g_exit = 2;
		return (-2);
	}
	narg = ft_count_argument(command_act -> args);
	if (narg > 1)
	{
		printf("exit: too many arguments \n");
		return (0);
	}
	if (narg == 1)
	{
		nerror = ft_atoi (command_act -> args[1]);
		if (nerror < 0)
			nerror += 256;
		nerror %= 256;
		g_exit = nerror;
	}
	return (-2);
}

