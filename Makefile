# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lsimon <lsimon@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/10/17 11:11:59 by lsimon            #+#    #+#              #
#    Updated: 2019/11/04 10:22:03 by lsimon           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc

CFLAGS = -Werror -Wall -Wextra -g

SRCS_DIR = srcs
INC_DIR = inc
OBJS_DIR = objs

NAME = ft_ssl

SRC_NAMES += ft_ssl.c
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