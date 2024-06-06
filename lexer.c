#include "environment.h"

static int	next_delimiter(char const *str, char c, int i)
{
	while (str[i] && str[i] != c)
		i++;
	return (i);
}

static int	count_tokens(char *str)
{
	int		tokens;
	int		i;
	int 	sp;

	i = 0;
	sp = 1;
	tokens = 0;
	while (str[i] && sp == 1)
	{
		while (str[i]== ' ')
			i++;
		if ((str[i] == '"') || (str[i] == '\''))
		{
			tokens++;
			i = next_delimiter(str, str[i] , i+1) + 1;
			sp = 0;
		}
		else if (str[i])
		{
			while(str[i]!= ' '&& str[i])
			{
				if ((str[i] == '"') || (str[i] == '\'')) //aqui solo puede entrar su hay comillas en medio de un texto
				break;
				if ((str[i]==';'||str[i]=='|') || (str[i+1]==';'||str[i+1]=='|'))
				{
					i++;
					break;
				}
				i++;
			}
			
			if ((str[i] |= '"') && (str[i] != '\'')) 
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
	if ((line[i] == '"') || (line[i] == '\''))
	{
		if (!line[next_delimiter(line, line[i], i+1)])
		{
			printf("no cierra comillas \n");
			return (-1);
		}
		token[j]= ft_substr(line, i, next_delimiter(line, line[i], i+1)  - i +1);
		if (!token[j])
			return(-1);
		i = next_delimiter(line, line[i] , i+1) + 1;
		
	}
	else
	{
		while (line[i] != ' ')
		{
			if ((line[i] == '"') || (line[i] == '\'')) //aqui solo puede entrar su hay comillas en medio de un texto
				break;
			aux = token[j];
			simbol[0] = line[i];
			token[j]= ft_strjoin(token[j],simbol);  //voy añadiendo caracter a caracter
			free(aux);
			if ((!token[j]))
				return (-1);
			if ((line[i+1]==';'||line[i+1]=='|') || (line[i]==';'||line[i]=='|')) //con esta codición me aseguro de que los simbolos ; | vayan solos
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
	token = (char **) malloc(sizeof(char *) * (ntoken + 1));
 	if (!line || !token)
		return NULL;
	j = 0;
	i = 0;
	while (j < ntoken)
	{
		while (line[i]== ' ')
			i++;
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
