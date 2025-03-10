# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marvin <marvin@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/10 15:35:20 by marvin            #+#    #+#              #
#    Updated: 2025/03/10 15:35:20 by marvin           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = raycasting

CC = cc

SRC = test.c

OBJS = $(SRC:.c=.o)

MLX_DIR = minilibx-linux/

all: $(NAME)

$(NAME): $(OBJS)
	make -C $(MLX_DIR)
	$(CC) $(OBJS) $(MLX_DIR)libmlx.a -lmlx -lXext -lX11 -L $(MLX_DIR) $(MLX_DIR)libmlx.a -lm -o $(NAME)

clean:
	rm -rf ./*.o

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re