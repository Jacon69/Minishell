#include "environment.h"

static int	next_delimiter(char const *str, char c, int i)
{
	while ((str[i] && str[i] != c) )
		i++;
	return (i);
}

static int	count_tokens(char *line)
{
	int		tokens;
	int		i;
	
	i = 0;
	tokens = 0;
	while (line[i] )
	{
		while (line[i]== ' ')
			i++;
		if ((line[i] == '"') || (line[i] == '\''))
		{
			tokens++;
		
			i = next_delimiter(line, line[i] , i+1) + 1;
		}		
		else if (line[i])
		{
					
			while(line[i]!= ' '&& line[i])
			{
				
			
				if ((line[i] == '"') || (line[i] == '\'') ) //aqui solo puede entrar su hay comillas en medio de un texto
				{
					
					break;
				}
					
				if (line[i]=='|' || line[i+1]=='|')
				{
					i++;
					break;
				}
				i++;
			}
			
			//if (((line[i] != '"') && (line[i] != '\'')) || (((line[i] == '"') || (line[i] == '\''))&&(line[i-1] == '\\')))
				tokens++;

		}

	}
	return (tokens);
}


static int ft_assig_token(char *line, char  **token, int j, int i)


{
	char	*aux;
	char	simbol[2];

	simbol[1]='\0';
	simbol[0] = line[i];
	if ((line[i] == '"') || (line[i] == '\''))
	{
		if (!line[next_delimiter(line, line[i], i+1)])
		{
			printf("1no cierra comillas \n");
			return (-1);
		}
		free(token[j]);//viene inicializado como se añade subcadena nueva lo libero
		token[j]= ft_substr(line, i, next_delimiter(line, line[i], i+1)  - i +1); //malloc
		
		if (!token[j])
			return(-1);
		i = next_delimiter(line, line[i] , i+1) + 1;
		
	}
	else
	{
		while (line[i] != ' ' && line[i])
		{
			if (line[i] == '"' || line[i] == '\'') //aqui solo puede entrar su hay comillas en medio de un texto
				break;
			aux = token[j];
			simbol[0] = line[i];
			token[j]= ft_strjoin(token[j],simbol);  //voy añadiendo caracter a caracter  //Malloc
			if (*aux)
				free(aux);
			
			if ((!token[j]))
				return (-1);
			if (line[i+1]=='|' || line[i]=='|') //con esta codición me aseguro de que los simbolos ; | vayan solos
			{	
				i++;
				break;
			}
			i++;
			
		}
	}
	return i;
}

char    **lexer(char    *line)
{
    char	**token;
	int		ntoken;
	int		i;
	int		j;


	ntoken = count_tokens(line);

	printf("num token %i \n",ntoken);
	token = (char **) malloc(sizeof(char *) * (ntoken + 1));
 	if (!line || !token)
		return NULL;
	j = 0;
	i = 0;

	while (j < ntoken)
	{
		while (line[i]== ' ')
			i++;
		token[j]= (char *) malloc(sizeof(char));
		if (!token[j])
		{
			ft_free_char(token);
			return NULL;
		}
		token[j][0]='\0'; //Aquí inicializo antes de asignar el valor del token oara poder usar 
		i = ft_assig_token(line, token, j, i);
		if (i == -1)
		{
			ft_free_char(token);
			return (NULL);
		}
		j++;
	}
	token[j] = NULL;
	return(token);
}
