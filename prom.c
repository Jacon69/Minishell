#include "minishell.h"

void ft_imprimetoken(char **token)
{
	
	int		i;

	i = 0;
	while (token[i])
	{
		printf("%i token %s \n",i,token[i]);
		i++;
	}
}

void ft_ejecutar(char *line, t_list  **env)
{
	char **args;

	args = ft_split(line, ' ');
	if (!args)
		return;
	if (ft_strncmp(args[0], "env",3) == 0)
	{
		ft_lstiter(*env, print_string); //##PRUEBA## Imprime la lista de variables de entorno
	}
	else if ((ft_strncmp(args[0], "export", 6) == 0) && ((ft_memchr(args[1], '=', ft_strlen(args[1]))!= NULL)))
	{
		ft_add_v_env(args[1], env);
	}
	else if ((ft_strncmp(args[0], "export", 6) == 0) && (args[1] == NULL))
	{
		ft_lstiter(*env, print_string); //##PRUEBA## Imprime la lista de variables de entorno
	}
	else if (ft_strncmp(args[0], "export", 6) == 0)
		return;
	else if (ft_strncmp(args[0], "unset", 5) == 0)
	{
		ft_del_v_env(args[1], env); 
	}
	else if (ft_strncmp(args[0], "cd", 2) == 0)
	{
	  //  ft_chdir(args[1]);                   //Crear un ft_chdir
	}
	else if (ft_strncmp(args[0], "pwd", 3) == 0)
	{
		ft_pwd(env);                         //Crear un ft_pwd
	}
	else if (ft_strncmp(args[0], "echo", 4) == 0)
			ft_printf("%s\n", args[1]);
	else if (ft_strncmp(args[0], "exit", 4) == 0)
	{
		ft_free_char(args);
		ft_free_list(env);
		exit(0);
	}
	else
		printf("Comando no encontrado\n");
}

void prom(t_list  **env) 
{
	char 		*line;
	char		**token;
	t_command	**comands;
	int			last_return;

	
	while (1) {
		// Mostrar el prompt y leer una línea de entrada

//TODO preparar cadena con pwd para añadir al prom con colorines

		line = readline("prom> "); //hace Malloc
		if (!line)
			break; // EOF, probablemente Ctrl+D Ctrl+c
		if (ft_strlen(line) > 0) // Añadir la línea al historial

		if (ft_strncmp(line, "exit", 4) == 0) // Si la línea es "exit", salir del bucle
		{
			free(line);
			break;
		}
		
		token = lexer(line);  //Malloc
		if (!token)
			return; ///Preperaar error memoria
		// ft_imprimetoken(token); //Antes de expandirse
		expander(token, env);
		comands = parser(token);
		last_return = executor(comands,env); //Recibir variables de entornos
		//TODO guardar en env la variable devuelta para poder imprimirla desde echo $?  TEngo que ponerlo en Expander


		//ft_imprimetoken(token); //Expandidof
		// ft_ejecutar(line, env); //PAra pruebas en esta función pongo los comando que quiero probar  // están los built-ins mirar si se pueden lanzar como  procesos

		// Aquí es donde de se tiene que procesar la linea introducida
		ft_free_char(token);
		free(line); // Liberar la memoria de la línea
	}

}
