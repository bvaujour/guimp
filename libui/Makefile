# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: injah <injah@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/03 16:48:10 by bvaujour          #+#    #+#              #
#    Updated: 2025/09/26 18:43:24 by injah            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libui.a

OBJ_DIR = obj

SRC =	ui_utils.c						\
		ui_window.c						\
		ui_core.c						\
		ui_destroy.c					\
		ui_surface.c					\
		ui_widget.c						\
		ui_container.c					\
		ui_render.c						\
		ui_update.c						\

OBJ = $(SRC:%.c=$(OBJ_DIR)/%.o)

CFLAGS = -Wall -Wextra -Werror -g -ISDL/include/SDL2 -ISDL_ttf/include/SDL2 -ISDL_image/include/SDL2

CC = gcc

all: $(NAME)

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	ar -rcs $(NAME) $(OBJ)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
