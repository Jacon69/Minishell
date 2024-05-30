#include "environment.h"
#include "./Libft/libft.h"



int main(int narg, char * carg[], char **environment)
{
    t_list  **env;
    char **var_entorno;

    narg ++; //para que no salten los warnings que saltan si no se usan las variables.
    carg [0] = "hola"; 
    env = ft_ini_env(environment);  //Malloc
       
    if (env == NULL)
        return (1); //No se ha podido crear la lista con las variables de entorno.*/
    
    prom(env); //##PRUEBA## Imprime el prompt y lee una línea de entrada

    var_entorno = ft_get_env(env); //Malloc
    if (var_entorno == NULL)
		return (1); //No se ha podido crear el array de variables de entorno.*/
	ft_free_char(var_entorno);//Liberar var_entorno después de usarla
    ft_free_list(env); //Libero la memoria de la lista de variables de entorno.
    
    return(0);
}