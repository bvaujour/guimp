# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: injah <injah@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/03 16:48:10 by bvaujour          #+#    #+#              #
#    Updated: 2026/04/20 16:51:50 by injah            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libui.a

CC = cc

OBJ_DIR = obj

SRC =	ui_core.c \
		ui_engine.c \
		ui_widget_user.c \
		ui_widget.c \
		ui_window.c \
		ui_utils.c \
		ui_button.c \
		ui_slider.c \
		ui_box.c \
		ui_layer.c \
		ui_layer_user.c \
		ui_input.c \
		ui_text.c \
		ui_image.c \
		ui_prefabs.c \
		ui_canvas.c

OBJ = $(SRC:%.c=$(OBJ_DIR)/%.o)

SDL_CFLAGS = $(shell pkg-config --cflags sdl2 SDL2_image SDL2_ttf)

CFLAGS = -Wall -Wextra -Werror -g
CFLAGS += -I.
CFLAGS += -Ilibft
CFLAGS += $(SDL_CFLAGS)

all: $(NAME)

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	ar -rcs $(NAME) $(OBJ)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re