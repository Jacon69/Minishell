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

void prom(t_list  **env)
{
	char 		*line;
	char		**token;
	t_command	**commands;
	int			last_return;
	char		str_last_return[20];
	int			control;
	char 		*path_act;
	char		*aux;
	
	control = 1;
	

	while (control==1)
	{
		path_act = ft_get_var_env(env,"PWD"); //malloc
		if  (!path_act)
			return;
		aux= path_act;
		path_act=ft_strjoin(path_act, " XXX$ ");
		free(aux);
		if (!path_act)
			return;
		line = readline(path_act); //hace Malloc

		if (!line)
		{
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
				continue;
			}		

		add_history(line);

		token = lexer(line);  //Malloc
		if (!token)
		{
			free(line);
			free(path_act);
			return;
		}
		expander(token, env);
		commands = parser(token);
		if (!commands)
		{
			free(line);
			free(path_act);
			ft_free_char(token);
			return;
		}
		last_return = executor(commands,env); //Recibir variables de entornos
		snprintf(str_last_return, sizeof(str_last_return), "%i", last_return);//convierto num a cadena
		ft_save_var_env("?", str_last_return,env);
		ft_free_char(token);
		free(line);
		free(path_act);
	}
}
