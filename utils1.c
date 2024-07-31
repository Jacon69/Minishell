#include "minishell.h"

void	ft_free_list(t_list **list)
{
	t_list	*p_nodo;
	t_list	*aux;

	p_nodo = *list;
	while (p_nodo)
	{
		aux = p_nodo->next;
		free(p_nodo->content);
		free(p_nodo);
		p_nodo = aux;
	}
	*list = NULL;
}

void	ft_free_char(char **ptr)
{
	int	i;

	i = 0;
	if (!ptr)
		return ;
	while (ptr[i] != NULL)
	{
		free(ptr[i]);
		i++;
	}
	free(ptr);
}

void	print_string(void *str)
{
	printf("%s\n", (char *)str);
}

int	ft_pos_chr(const char *str, int c)
{
	int	i;

	i = 0;
	while (*str != (char) c)
	{
		if (!*str)
			return (-1);
		str++;
		i++;
	}
	return (i);
}

int	ft_smaller(int val_1, int val_2)
{
	return((val_1 <= val_2) ? val_1 : val_2);
}

char	*ft_strndup(const char *str, size_t n)
{
	char	*dup;
	size_t	len;

	len = ft_strlen(str);
	if (n < len)
	{
		len = n;
	}
	dup = (char *) malloc(len + 1);
	if (dup == NULL)
		return (NULL);
	ft_memcpy(dup, str, len);
	dup[len] = '\0';
	return (dup);
}

t_list	**ft_dup_ord_list(t_list **env)
{
	t_list	**dup_env;
	t_list	*p_env;
	t_list	*p_env_aux;
	t_list	*p_env_ori;
	t_list	**ref_previa;
	int		exit_bucle;

	p_env_ori = *env;
	if (!p_env_ori)
		return (NULL);
	dup_env = (t_list **)malloc(sizeof(t_list *));
	if (!dup_env)
		return (NULL);
	*dup_env = NULL;
	p_env = ft_lstnew(ft_strdup(p_env_ori->content));
	if (!p_env)
	{
		ft_free_list(dup_env);
		return (NULL);
	}
	ft_lstadd_back(dup_env, p_env);
	while (p_env_ori -> next)
	{
		p_env_ori = p_env_ori -> next;
		p_env_aux = ft_lstnew(ft_strdup(p_env_ori->content));
		if (!p_env_aux)
		{
			ft_free_list(dup_env);
			return (NULL);
		}
		p_env = *dup_env;
		if (ft_strncmp(p_env -> content, p_env_aux -> content,
				ft_smaller(ft_strlen(p_env -> content),
					ft_strlen(p_env_aux -> content))) > 0)
		{
			*dup_env = p_env_aux;
			p_env_aux -> next = p_env;
			continue ;
		}
		else if (p_env -> next == NULL)
		{
			p_env -> next = p_env_aux;
			continue ;
		}
		exit_bucle = 1;
		while (p_env -> next && (exit_bucle == 1))
		{
			exit_bucle = 1;
			ref_previa = &p_env -> next;
			p_env = p_env -> next;
			if (ft_strncmp(p_env -> content, p_env_aux -> content,
					ft_smaller(ft_strlen(p_env -> content),
						ft_strlen(p_env_aux -> content))) > 0)
			{
				*ref_previa = p_env_aux;
				p_env_aux -> next = p_env;
				exit_bucle = 0;
			}
			else if (p_env -> next == NULL)
			{
				p_env -> next = p_env_aux;
				exit_bucle = 0;
			}
		}
	}
	return (dup_env);
}

int	ft_print_list_env(t_command *command, t_list **env)
{
	t_list	**dup_env;
	t_list	*p_env;
	int		ok;

	ok = 1;
	if (!ft_memcmp(command -> command, "export", 6))
	{
		dup_env = ft_dup_ord_list(env);
		if (!dup_env)
		{
			perror("Error MEM export");
			return (-1);
		}
		command -> command = "env";
		ft_print_list_env(command, dup_env);
		ft_free_list(dup_env);
		dup_env = NULL;
		return (1);
	}
	p_env = *env;
	while (p_env)
	{
		if (ft_strncmp("..", p_env -> content, 2) != 0)
		{
			ok *= write(command -> file_output,
					p_env -> content, ft_strlen(p_env -> content));
			ok *= write(command -> file_output, "\n", 1);
			if (ok > 0)
				ok = 1;
			else
				ok = -1;
		}
		p_env = p_env -> next;
	}
	return((ok <= 0) ? 1 : 0);
}

int	ft_is_dir_ok(const char *path)
{
	struct stat	statbuf;

	if (stat(path, &statbuf) != 0)
	{
		perror("stat");
		return (0);
	}
	if (S_ISDIR(statbuf.st_mode))
		return (1);
	else
		return (0);
}
