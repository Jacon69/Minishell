# Nombre del ejecutable final
NAME = prom

# Directorios y archivos
LIBFT_DIR = ./Libft
LIBFT = $(LIBFT_DIR)/libft.a

SRC = main.c \
		prom.c \
		environment.c \
		utils1.c \
		lexer.c \
		expander.c \
		parser.c \
		executor.c \
		utils2.c \
		build-int.c \
		get_next_line/get_next_line.c \
		build_in_cd.c\

OBJ = $(SRC:.c=.o)

# Compilador y flags
CC = clang
CFLAGS = -Wall -Wextra -Werror -g
LDFLAGS = -L$(LIBFT_DIR) -lft -lreadline

# Regla por defecto
all: $(NAME)

# Regla para compilar el ejecutable
$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) $(LDFLAGS)

# Regla para compilar los archivos .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regla para compilar la librería libft
$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

# Regla para limpiar los archivos .o
clean:
	rm -f $(OBJ)
	$(MAKE) -C $(LIBFT_DIR) clean

# Regla para limpiar todo, incluidos los ejecutables y la librería
fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

# Regla para recompilar todo
re: fclean all

.PHONY: all clean fclean re