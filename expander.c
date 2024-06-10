#include "environment.h"




char    **expander(char **token, t_list  **env)
{
    int i;
    int j;

    i = 0;
    j = 0;
    while(token[i])
    {
        while (token[i][j])
        {
            if (token[i][0] =='\'')
                break;
            
            
            j++;
        }
        i++;
    }
        




    }





}