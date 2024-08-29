# Name of executable
NAME = minishell

# Directories and files
LIBFT_DIR = ./Libft
LIBFT = $(LIBFT_DIR)/libft.a

SRC = main.c \
		prom.c \
		prom_utils.c \
		environment.c \
		environment_utils.c \
		environment_utils2.c \
		utils1.c \
		lexer.c \
		lexer_utils.c \
		lexer_utils2.c \
		expander.c \
		expander_utils1.c \
		expander_utils2.c \
		expander_utils3.c \
		parser.c \
		init_commands.c \
		pipes.c \
		redirs.c \
		parser_checks.c \
		executor.c \
		utils2.c \
		utils3.c \
		utils4.c \
		build-int.c \
		build_int2.c \
		build_int3.c \
		get_next_line/get_next_line.c \
		build_in_cd.c\
		build_in_cd_aux.c \
		executor_utils.c\
		executor_utils2.c \
		executor_children.c \

OBJ = $(SRC:.c=.o)

# Compiler and flags
CC = clang
CFLAGS = -Wall -Wextra -Werror -g -O0
LDFLAGS = -L$(LIBFT_DIR) -lft -lreadline

# Default rule
all: $(NAME)

# Compilation rule
$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) $(LDFLAGS)

# .o files rule
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# libft rule
$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

# .o files clean rule
clean:
	rm -f $(OBJ)
	$(MAKE) -C $(LIBFT_DIR) clean

# clean all rule
fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

# recompile all rule
re: fclean all

.PHONY: all clean fclean re