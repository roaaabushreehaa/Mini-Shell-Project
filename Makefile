CC = cc

FLAGS = -Wall -Wextra -Werror -g

SRC_DIR = src

OBJ_DIR = obj

SRC = read.c token.c handle.c split_handle.c utils.c split_quotes.c cmd.c execute_builtin.c echo.c unset.c expander.c export.c cd.c env.c free.c parsing.c files_open.c execute.c pipex.c
OBSRC = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

LIBFT_DIR = libft

LIBFT_A = $(LIBFT_DIR)/libft.a

NAME = minishell

all: $(NAME)

$(NAME): make_libft $(OBSRC) 
	$(CC) $(FLAGS) $(OBSRC) -L$(LIBFT_DIR) -lft -lreadline -o $@

make_libft:
	@echo "Making libft..."
	@make -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(FLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	@make -C $(LIBFT_DIR) clean
	rm -rf $(OBJ_DIR)

fclean: clean
	@make -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re make_libft

