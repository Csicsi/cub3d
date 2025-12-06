# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dcsicsak <dcsicsak@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/20 10:41:15 by icseri            #+#    #+#              #
#    Updated: 2025/12/06 19:48:41 by dcsicsak         ###   ########.fr        #
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

web:
	@echo "Preparing WebGL-compatible shaders..."
	@cp -f ./shaders/default.frag ./MLX42/shaders/default.frag
	@echo "Rebuilding MLX42 with WebGL shaders..."
	@bash -c "source ~/emsdk/emsdk_env.sh && \
	rm -rf MLX42/build && \
	emcmake cmake -B MLX42/build -S MLX42 -DCMAKE_BUILD_TYPE=Release && \
	emmake make -C MLX42/build -j4"
	@echo "Building for WebAssembly..."
	@bash -c "source ~/emsdk/emsdk_env.sh && \
	emcc -O3 \
		main_web.c $(COMMON_SRCS) \
		./libft/ft_*.c ./libft/get_next_line*.c \
		$(MLX42_LIB) \
		-I$(MLX42_DIR)/include \
		-o cub3d.html \
		-s USE_GLFW=3 \
		-s USE_WEBGL2=1 \
		-s FULL_ES3=1 \
		-s WASM=1 \
		-s NO_EXIT_RUNTIME=1 \
		-s ALLOW_MEMORY_GROWTH=1 \
		-s EXPORTED_RUNTIME_METHODS='[\"ccall\",\"cwrap\"]' \
		-s TOTAL_STACK=8388608 \
		-s GL_DEBUG=1 \
		-s ASSERTIONS=2 \
		--shell-file shell.html \
		--preload-file maps \
		--preload-file textures"
	@echo "Web build complete! Open cub3d.html in a browser."

.PHONY: all clean fclean bonus re web
