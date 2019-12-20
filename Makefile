CC = gcc

CFLAGS = -Werror -Wall -Wextra

SRCS_DIR = srcs
INC_DIR = inc
OBJS_DIR = objs

NAME = ft_ssl

SRC_NAMES += ft_ssl.c
SRC_NAMES += md5.c
SRC_NAMES += sha256.c
SRC_NAMES += flags.c
SRC_NAMES += content.c
SRC_NAMES += file.c
SRC_NAMES += parsing.c
SRC_NAMES += display.c
SRC_NAMES += string.c
SRC_NAMES += hash.c
SRC_NAMES += hash_utils.c
SRC_NAMES += processes.c
SRC_NAMES += sha_utils.c
SRC_NAMES += usage.c
SRC_NAMES += sha256_compress.c
SRC_NAMES += sha256_pad.c
SRC_NAMES += inputs.c
SRC_NAMES += md5_utils.c

SRCS = $(addprefix $(SRCS_DIR)/, $(NM_SRC_NAMES))

OBJ_NAMES = $(SRC_NAMES:.c=.o)
OBJS = $(addprefix $(OBJS_DIR)/, $(OBJ_NAMES))

all: $(NAME)

libft/libft.a:
	@make -C libft/

$(NAME): $(OBJS) libft/libft.a
	$(CC) -o $@ $^

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p $(OBJS_DIR)
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
