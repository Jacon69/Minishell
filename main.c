#include "minishell.h"

int	main(int narg, char *carg[], char **environment)
{
	t_list			**env;
	struct termios	attributes;

	narg++;
	carg [0] = "hola";
	env = ft_ini_env(environment);
	if (env == NULL)
		return (1);
	ft_save_var_env("?", "0", env);
	tcgetattr(STDIN_FILENO, &attributes);
	attributes.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &attributes);
	prom(env);
	ft_free_list(env);
	return (0);
}
