#include "environment.h"


//echo echo -n cd pwd export unset env exit 
ft_built_echo(t_command command)
{
	int jump_line;
	int	i;
	int ok;

		i = 0;
		jump_line =1;
	if (!ft_memcmp(command.args[0], "-n", 2))
	{
		jump_line = 0;
		i++;
	}	
	while(command.args[i])
	{
		write(command.file_output,command.args[i], ft_strlen(command.args[i]));
		if (command.args[i+1])
			ok = write(command.file_output," ",1);
		i++;
		if (jump_line)
			ok *= write(command.file_output,"\n",1);

		return (ok);
	}
}
ft_built_cd(t_command command, t_list **env)
{
	int jump_line;
	int	i;
	int ok;
	char **path;
	char *line_path;
	DIR *dir;
	char *route;
	char *aux;
	char *aux2;

	int	num_dir;
	
	line_path=ft_get_var_env(env, command.path);
	route = malloc (2);
	if (!route)
		exit;//error mem
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
	if (num_dir >1) 
	

	if (((!ft_memcmp(command.args[0], "..", 2) && ft_strlen(command.args[0]==2)) || 
	(!ft_memcmp(command.args[0], "../", 3)&&(ft_strlen(command.args[0])==3))) && !(command.args[1]))
	{
		
		while( i < (num_dir - 1) && ft_strlen(line_path) > 1)  // si line :path es mayor que 1 es no es raiz si es raiz no hace nada
		{
			aux=route;
			route= ft_strjoin(route, path[i]); //malloc
			free(aux);
			if (!route)
				exit;//error memoria
			i++;
		}
	}
		  
	else if (((command.args[0][0]=='.'&& ft_strlen(command.args[0]==1))||(!ft_memcmp(command.args[0], "./", 2) && ft_strlen(command.args[0]==2) ))&& !(command.args[1]))
		;//No hace nada
	else if ((command.args[0][0]== '.') && (command.args[1]))
	{
		ferror("Error cd: too many arguments");
		exit;//Error cd: too many arguments
	}

	else if ((!ft_memcmp(command.args[0], "./", 2) || !ft_memcmp(command.args[0], "../", 3))&& (command.args[1]))
		{
			ferror("//Error cd: too many arguments");
			;//Error cd: too many arguments
		}

	else if (!ft_memcmp(command.args[0], "./", 2))  ///Es es que comience con ./ o sin barra  Se pone rua relativa
	{
		while( i < (num_dir) )  //conStruyo la ruta absoluta
		{
			aux=route;
			route= ft_strjoin(route, path[i]); //malloc
			free(aux);
			if (!route)
				exit;//error memoria
			if (path[i+1])
			{
				route= ft_strjoin(route, "/"); //malloc
				free(aux);
				if (!route)
					exit;//error memoria
			}
			i++;
		}
		aux2 = *ft_strchr(command.args[0], '/');
		aux2++;
		
		aux=route;
		route= ft_strjoin(route, aux2); //malloc
		free(aux);
		if (!route)
			exit;//error memoria
	}	

	else if (!ft_memcmp(command.args[0], "../", 3))  ///aquí tiene que haber algo detrás de la barra
	{
		;
	}
pENDIENTE DE HACER CD ../pROYECTO

					Relativo


else if (command.args[0][0]== '/') //ABSOLUTA
{
	aux2 = command.args[0]; 
	aux2++;
	
	aux=route;
	route= ft_strjoin(route, aux2); //malloc
	free(aux);
	if (!route)
		exit;//error memoria
}

	// TODO cambio a dirección relativa directorio anterior
	//TODO Cambio dirección absoluta





/*Variables de  enviroment que tengo que controlar
	pwd
	oldpwd
	
	*/




	while(command.args[i])
	{
		if 
		
		
		write(command.file_output,command.args[i], ft_strlen(command.args[i]));
		if (command.args[i+1])
			ok = write(command.file_output," ",1);
		i++;
		

		return (ok);
	}
}

void ft_build_int(t_myshell shell, t_command command_act)

{
	char	*comando;
	int		fd;

	comando = command_act.command;



	if (ft_memcmp(comando, "echo", 4)== 0)
		ft_built_echo(command_act);
	else if (ft_memcmp(comando, "cd", 2)== 0)
		ft_built_cd(command_act, shell.env);
	else if (ft_memcmp(comando, "pwd", 3)== 0)
		ft_built_pwd(command_act);
	else if (ft_memcmp(comando, "export", 6)== 0)
	ft_built_export(command_act);
	else if (ft_memcmp(comando, "unset", 5)== 0)
		ft_built_unset(command_act);
	else if (ft_memcmp(comando, "env", 3)== 0)
		ft_built_env(command_act);
	else if (ft_memcmp(comando, "exit", 4)== 0)
		exit;

}
