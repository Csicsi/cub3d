# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dcsicsak <dcsicsak@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/20 10:41:15 by icseri            #+#    #+#              #
#    Updated: 2025/12/06 20:13:28 by dcsicsak         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = cub3D

BONUS_NAME = cub3D_bonus

MLX42_DIR = MLX42
MLX42_LIB = $(MLX42_DIR)/build/libmlx42.a
MLX42_FLAGS = -lglfw -ldl -pthread -lm

COMMON_SRCS = init.c \
				parsing.c \
				parsing_utils.c \
				parsing_textures.c \
				texture_utils.c \
				parsing_map.c \
				cleanup.c \
				cast_rays.c \
				cast_rays_horizontal.c \
				cast_rays_vertical.c \
				minimap.c \
				draw_utils.c \
				movement_utils.c \
				mlx_utils.c \
				movement.c \
				mlx.c

SRCS = main.c  ${COMMON_SRCS}

BONUS_SRCS = main_bonus.c ${COMMON_SRCS}

OBJS = ${SRCS:.c=.o}

BONUS_OBJS = ${BONUS_SRCS:.c=.o}

COMP = cc

CFLAGS = -Wall -Wextra -Werror

all: ${NAME}

.c.o:
	${COMP} ${CFLAGS} -c $< -o $@

${NAME}: ${OBJS}
	@${MAKE} -C ./libft --no-print-directory
	@${COMP} ${CFLAGS} ${OBJS} ./libft/libft.a $(MLX42_LIB) -o ${NAME} $(MLX42_FLAGS)

bonus: ${BONUS_OBJS}
	@${MAKE} -C ./libft --no-print-directory
	@${COMP} ${CFLAGS} ${BONUS_OBJS} ./libft/libft.a $(MLX42_LIB) -o ${BONUS_NAME} $(MLX42_FLAGS)

clean:
	@${MAKE} -C ./libft --no-print-directory fclean
	@rm -rf ${OBJS} ${BONUS_OBJS}

fclean: clean
	@rm -rf ${NAME} ${BONUS_NAME}

re: fclean all

.PHONY: all clean fclean bonus reNAME = cub3D
