# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: injah <injah@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/20 09:45:35 by injah             #+#    #+#              #
#    Updated: 2026/04/22 21:54:25 by injah            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = guimp

CC = cc

SRC =	main.c \
		draws.c \
		utils.c \
		css_parser.c \
		build.c \
		css_apply.c \
		about.c

OBJ = $(SRC:.c=.o)

CFLAGS = -Wall -Wextra -Werror -g

SDL_CFLAGS = $(shell pkg-config --cflags sdl2 SDL2_image SDL2_ttf)
SDL_LDFLAGS = $(shell pkg-config --libs sdl2 SDL2_image SDL2_ttf)

LIBUI_DIR = libui

LIBUI_LIB = -L$(LIBUI_DIR) -lui
LIBFT_LIB = -L$(LIBUI_DIR)/libft -lft

LDFLAGS = $(LIBUI_LIB) $(LIBFT_LIB) $(SDL_LDFLAGS) -lm

all: $(NAME)

$(NAME): $(OBJ)
	make -C $(LIBUI_DIR)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(SDL_CFLAGS) -I$(LIBUI_DIR) -c $< -o $@

clean:
	make clean -C $(LIBUI_DIR)
	rm -f $(OBJ)

fclean: clean
	make fclean -C $(LIBUI_DIR)
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re