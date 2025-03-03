CC = cc

FLAGS = -Wall -Wextra -Werror

SRC = read.c token.c handle.c split_handle.c utils.c split_quotes.c cmd.c define_word.c excutable.c expander.c

OBSRC = $(SRC:.c=.o)

LIBFT_DIR = libft

LIBFT_A = $(LIBFT_DIR)/libft.a

NAME = minishell

all: $(NAME)

$(NAME): $(OBSRC) $(LIBFT_A)
	$(CC) $(FLAGS) $(OBSRC) -L$(LIBFT_DIR) -lft -lreadline -o $@

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

$(LIBFT_A):
	make -C $(LIBFT_DIR)

clean:
	make clean -C $(LIBFT_DIR)
	rm -f $(OBSRC)

fclean: clean
	make fclean -C $(LIBFT_DIR)
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re