#include "minishell.h"

/*void ft_imprimetoken(char **token)
{
	
	int		i;

	i = 0;
	while (token[i])
	{
		printf("%i token %s \n",i,token[i]);
		i++;
	}
}*/

static void	signal_handler(int signum)
{
	if (signum == SIGINT) //ctrl-C
	{
		if (is_executing == 0)
		{
			rl_replace_line("", 1);
			printf("\n");
			rl_on_new_line();
			rl_redisplay();
		}
		else if (is_executing == 1)
		{
			is_executing = 0;
			return ;
		}
		else
		{
			printf("%d\n", is_executing);
			is_executing = -1;
			return ;
		}
	}
	if (signum == SIGQUIT) //ctrl-barra
	{
		//printf("\nHa entrado aqui\n");
		if (is_executing == 1)
		{
			printf("Quit");
			is_executing = 0;
			return ;
		}
		else
		{
			return ;
		}
	}
}

void prom(t_list  **env)
{
	char 				*line;
	char				**token;
	t_command			**commands;
	int					last_return;
	char				str_last_return[20];
	int					control;
	char 				*path_act;
	char				*aux;
	struct sigaction	action;

	control = 1;
	
	action.sa_handler = signal_handler;
	action.sa_flags = 0;
	sigemptyset(&action.sa_mask);
	sigaction(SIGINT, &action, NULL);
	sigaction(SIGQUIT, &action, NULL);
	while (control==1)
	{
		path_act = ft_get_var_env(env,"..PWD"); //malloc
		if  (!path_act)
			return;
		aux= path_act;
		path_act=ft_strjoin(path_act, " XXX$ ");
		free(aux);
		if (!path_act)
			return;
		//is_executing = 2;
		line = readline(path_act); //hace Malloc
		if (is_executing == -1)
		{
			free(line);
			free(path_act);
			is_executing = 0;
			continue ;
		}
		else
		{
			is_executing = 0;
			if (!line)
			{
				printf("exit\n");
				printf("Line es nulo\n");
				break; // EOF, probablemente Ctrl+D Ctrl+c
			}
			if (line[0] == '\0') 
				{
					free(line);
					free(path_act);
					continue;
				}
			if (!ft_memcmp(line,"exit",4)) 
				{
					free(line);
					free(path_act);
					control = 0;
					return;
				}		
			add_history(line);
			token = lexer(line);  //Malloc
			if (!token)
			{
				free(line);
				free(path_act);
				ft_free_list(env); //Libero la memoria de la lista de variables de entorno.
				perror("Error Mem en LEXER");
				exit(1);
			}
			if (!expander(token, env)) ///Hago la expansión $ Comillas etc si es 0 es KO
			{
				free(line);
				free(path_act);
				ft_free_char(token);
				ft_free_list(env); //Libero la memoria de la lista de variables de entorno.
				perror("Error Mem en EXPANDER");
				exit(1);
			}
			commands = parser(token, env); //malloc
			if (!commands)
			{
				free(line);
				free(path_act);
				ft_free_char(token);
				ft_free_list(env); //Libero la memoria de la lista de variables de entorno.
				perror("Error Mem en PARSER");
				exit(1);
			}
			last_return = executor(commands,env); //Recibir variables de entornos
			if (last_return == -1)
			{
				free(line);
				free(path_act);
				ft_free_char(token);
				ft_free_list(env); //Libero la memoria de la lista de variables de entorno.
				free_commands(commands);
				perror("Error Mem en EXECUTOR");
				exit(1);
			}
			snprintf(str_last_return, sizeof(str_last_return), "%i", last_return);//convierto num a cadena
			ft_save_var_env("?", str_last_return,env);
			ft_free_char(token);
			free(line);
			free(path_act);
			free_commands(commands);
		}
	}
}
