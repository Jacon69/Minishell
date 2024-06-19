#include "minishell.h"


int main(int narg, char * carg[], char **environment)
{
	t_list  **env;

	narg ++; //para que no salten los warnings que saltan si no se usan las variables.
	carg [0] = "hola"; 
	env = ft_ini_env(environment);  //Malloc
	   
	if (env == NULL)
		return (1); //No se ha podido crear la lista con las variables de entorno.*/
	
	ft_save_var_env("?", 0,env);// Inicializo $?
	prom(env); //##PRUEBA## Imprime el prompt y lee una línea de entrada

   
   

	ft_free_list(env); //Libero la memoria de la lista de variables de entorno.
	
	return(0);
}