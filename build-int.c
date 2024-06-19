#include "minishell.h"


//echo echo -n cd pwd export unset env exit 
int ft_built_echo(t_command *command)
{
	int jump_line;
	int	i;
	int ok;

		i = 0;
		jump_line =1;
	if (!ft_memcmp(command->args[0], "-n", 2) && ft_strlen(command->args[0])==2)
	{
		jump_line = 0;
		i++;
	}	
	while(command->args[i])
	{
		write(command->file_output,command->args[i], ft_strlen(command->args[i]));
		if (command->args[i+1])
			ok = write(command->file_output," ",1);
		i++;
		if (jump_line)
			ok *= write(command->file_output,"\n",1);

		return((ok <= 0) ? 1 : 0); 
	}
	return(0);
}

int ft_built_cd(t_command *command, t_list **env)
{
	int	i;
	char **path;
	char *line_path;
	char *route;
	char *aux;
	char *aux2;

	int	num_dir;
	
	line_path= command->path; //cojo el path actual del guradado en cmd
	route = malloc (2); 
	if (!route)
		exit(EXIT_FAILURE);//error memoria
	route[0]='/';
	route[1]='\0';
	num_dir=0;
	i = 0;
	while  (line_path[i])
	{
		if (line_path[i]=='/')
			num_dir++;
	}
	i=0;
	path = ft_split(line_path, '/'); //Malloc
	if (!path)
	{
		
		perror("Error memoria");
		//cierra_todo();
		exit(EXIT_FAILURE);//error memoria
	}
	if ((((!ft_memcmp(command->args[0], "..", 2) && ft_strlen(command->args[0])==2)) || //cuando tengo .. 
	(!ft_memcmp(command->args[0], "../", 3)&&(ft_strlen(command->args[0])==3))) && !(command->args[1]))
	{
		
		while( i < (num_dir - 1) && ft_strlen(line_path) > 1)  // si line_path es mayor que 1 es no es raiz si es raiz no hace nada
		{
			aux=route;
			route= ft_strjoin(route, path[i]); //malloc  voy añadiendo menos el último
			free(aux);
			if (!route)
			{
				perror("Error memoria");
				free(path);
				//cierra_todo();
				exit(EXIT_FAILURE);//error memoria
			}
			i++;
		}
	}
		  
	else if (((((command->args[0][0]=='.'&& ft_strlen(command->args[0])==1))||(!ft_memcmp(command->args[0], "./", 2) && ft_strlen(command->args[0])==2) ))&& !(command->args[1]))
		;//No hace nada
	else if ((command->args[0][0]== '.') && (command->args[1]))
	{
		perror("Error cd: too many arguments");
		free(path);
		free(route);
		return(1);//Error cd: too many arguments
	}

	else if ((!ft_memcmp(command->args[0], "./", 2) || !ft_memcmp(command->args[0], "../", 3))&& (command->args[1]))
	{
		perror("Error cd: too many arguments");
		free(path);
		free(route);
		return(1);//Error cd: too many arguments
	}

	else if (!ft_memcmp(command->args[0], "./", 2))  ///Es es que comience con ./ o sin barra  Se pone ruta relativa
	{
		while( i < (num_dir) )  //conStruyo la ruta absoluta
		{
			aux=route;
			route= ft_strjoin(route, path[i]); //malloc
			free(aux);
			if (!route)
			{
				perror("Error memoria");
				free(path);
				//cierra_todo();
				exit(EXIT_FAILURE);//error memoria
			}
			if (path[i+1])
			{
				aux = route;
				route= ft_strjoin(route, "/"); //malloc
				free(aux);
				if (!route)
				{
					perror("Error memoria");
					free(path);
					//cierra_todo();
					exit(EXIT_FAILURE);//error memoria
				}
				}
			i++;
		}
		aux2 = ft_strchr(command->args[0], '/'); //posiciono aux2 en lo que tengo que añadir a la ruta absoluta actual
		aux2++;
		
		aux=route;
		route= ft_strjoin(route, aux2); //malloc
		free(aux);
		if (!route)
		{
			perror("Error memoria");
			free(path);
			//cierra_todo();
			exit(EXIT_FAILURE);//error memoria
		}
	}	
	else if (!ft_memcmp(command->args[0], "../", 3))  ///aquí tiene que haber algo detrás de la barra
	{
		while( i < (num_dir - 1) )  //conStruyo la ruta absoluta menos el ultimo dir ya que es ..
		{
			aux=route;
			route= ft_strjoin(route, path[i]); //malloc
			free(aux);
			if (!route)
			{
				perror("Error memoria");
				free(path);
				//cierra_todo();
				exit(EXIT_FAILURE);//error memoria
			}
			if (path[i+1])
			{
				aux = route;
				route= ft_strjoin(route, "/"); //malloc
				free(aux);
				if (!route)
				{
					perror("Error memoria");
					free(path);
					//cierra_todo();
					exit(EXIT_FAILURE);//error memoria
				}
				}
			i++;
		}
		aux2 = ft_strchr(command->args[0], '/'); //posiciono aux2 en lo que tengo que añadir a la ruta absoluta actual
		aux2++;
		
		aux=route;
		route= ft_strjoin(route, aux2); //malloc
		free(aux);
		if (!route)
		{
			perror("Error memoria");
			free(path);
			//cierra_todo();
			exit(EXIT_FAILURE);//error memoria
		}
	}

	else if (command->args[0][0]== '/') //ABSOLUTA
	{
		aux2 = command->args[0]; 
		aux2++;
		
		aux=route;
		route= ft_strjoin(route, aux2); //malloc
		free(aux);
		if (!route)
		{
			perror("Error memoria");
			free(path);
			//cierra_todo();
			exit(EXIT_FAILURE);//error memoria
		}
	}

	ft_save_var_env("pwd", route, env); // aqui guardo lo valores en la variable de entorno. command->path se deberá actualizar
	ft_save_var_env("oldpwd", line_path, env);
	return(0);
}


int ft_built_pwd(t_command *command)
{
	int ok;
	
	ok = write(command->file_output,command->path, ft_strlen(command->path));
	write(1,"\n", 1);
	return((ok <= 0) ? 1 : 0);
}

int ft_built_export(t_command *command, t_list **env)
{
	int	ok;
	
	if (ft_memchr(command->args[0], '=', ft_strlen(command->args[0]))!= NULL)
	{
		ft_add_v_env(command->args[0], env);
		return (0);
	}
	if (command->args[0] == NULL)
	{
		ok = ft_print_list_env(command, env); //TODO ft_print_list_env(t_command *command, t_list **env)
		return(ok);
	}
	return (0);

}

int ft_built_unset(t_command *command, t_list **env)
{
	(void)command; // Esto evita el error de compilación por variable no utilizada.
	(void)env; // Esto evita el error de compilación por variable no utilizada.
	return (0);
}

int ft_built_env(t_command *command, t_list **env)
{
	(void)command; // Esto evita el error de compilación por variable no utilizada.
	(void)env; // Esto evita el error de compilación por variable no utilizada.
	return (0);	
}

int		ft_print_list_env(t_command *command, t_list **env)
{
	(void)command; // Esto evita el error de compilación por variable no utilizada.
	(void)env; // Esto evita el error de compilación por variable no utilizada.
	return (0);	
}

int ft_build_int(t_command *command_act, t_list **env)

{
	char	*comando;
	int		ok;

	comando = command_act->command;
	ok = 0;

	if (ft_memcmp(comando, "echo", 4)== 0)
		ok = ft_built_echo(command_act);
	else if (ft_memcmp(comando, "cd", 2)== 0)
		ok = ft_built_cd(command_act, env);
	else if (ft_memcmp(comando, "pwd", 3)== 0)
		ok = ft_built_pwd(command_act);
	else if (ft_memcmp(comando, "export", 6)== 0)
		ok = ft_built_export(command_act, env);
	else if (ft_memcmp(comando, "unset", 5)== 0)
		ok = ft_built_unset(command_act, env);
	else if (ft_memcmp(comando, "env", 3)== 0)
		ok = ft_built_env(command_act, env);
	else if (ft_memcmp(comando, "exit", 4)== 0)
	{
		free(command_act);
		ft_free_list(env);
		exit(EXIT_SUCCESS); //

	}
	
	return (ok);

}
