#include "minishell.h"

/* 
   Expander:
     - Analyzes tokens for variables, relative paths, and quotes.
     - Performs variable expansion, replacing variable names with their values.
     - Resolves relative paths by converting them to absolute paths.
     - Returns the modified token with expanded values.
*/
int	expander(char **token, t_list **env)
{
	int     param[2]; //0 nº token 1. posisión en el token:
	char	*aux;

	
	param[2] = {-1,0};

	while (token[++param[0]])
	{
		while( token[param[0]][param[1]]|='\0') 
        {
            if ()
        }
	}
	
	return (1);
}
