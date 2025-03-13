CC = cc

FLAGS = -Wall -Wextra -Werror

SRC_DIR = src

OBJ_DIR = obj

SRC = read.c token.c handle.c split_handle.c utils.c split_quotes.c cmd.c execute.c echo.c unset.c expander.c export.c cd.c env.c free.c parsing.c
OBSRC = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

LIBFT_DIR = libft

LIBFT_A = $(LIBFT_DIR)/libft.a

NAME = minishell

all: $(NAME)

$(NAME): $(OBSRC) $(LIBFT_A)
	$(CC) $(FLAGS) $(OBSRC) -L$(LIBFT_DIR) -lft -lreadline -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(FLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(LIBFT_A):
	make -C $(LIBFT_DIR)

clean:
	make clean -C $(LIBFT_DIR)
	rm -rf $(OBJ_DIR)

fclean: clean
	make fclean -C $(LIBFT_DIR)
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re