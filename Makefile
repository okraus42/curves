# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/03 17:31:18 by okraus            #+#    #+#              #
#    Updated: 2025/04/05 10:08:07 by okraus           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Program name (output executable)
NAME        := curves

# Compiler and flags  //old gdwarf for valgrind
CC          := cc
CFLAGS      := -Wall -Wextra -Werror -g -gdwarf-4
RM          := rm -rf

# Directories
SRC_DIR     := src
OBJ_DIR     := obj
MLX_DIR     := minilibx
# SDL_DIR     := sdl2
INCLUDE_DIR := include

# Source and object files
SRCS        :=	main.c \
				graphics/game.c \
				graphics/digit.c \
				utils/utils.c
OBJS        := $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))
DEPS        := $(OBJS:.o=.d)

# MiniLibX settings (Linux version)
MLX_REPO    := https://github.com/42Paris/minilibx-linux.git
MLX_FLAGS   := -I$(MLX_DIR) -L$(MLX_DIR) -lmlx -lXext -lX11
MLX_LIB     := $(MLX_DIR)/libmlx.a


# Default rule to build the program
all: CFLAGS += -DLIB=0 -I$(MLX_DIR)

all: $(NAME)

# Default rule using MiniLibX
$(NAME): $(MLX_LIB) $(OBJS)
	$(CC) $(CFLAGS) -lm -I$(INCLUDE_DIR) $^ $(MLX_FLAGS) -o $@

# Ensure MiniLibX is downloaded and built before compiling
$(MLX_LIB):
	@if [ ! -d "$(MLX_DIR)" ]; then \
		echo "Cloning MiniLibX..."; \
		git clone $(MLX_REPO) $(MLX_DIR); \
	fi
	@echo "Building MiniLibX..."
	@make -C $(MLX_DIR)

# Rule to compile each source file into an object file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -MMD -MP -c $< -o $@

# Clean up object files
miniclean:
	$(RM) $(OBJ_DIR)

# Clean up object files
clean:
	$(RM) $(OBJ_DIR)
	make -C $(MLX_DIR) clean || true

# Clean up both object files and the executable
fclean: clean
	$(RM) $(NAME) $(GAME_NAME)
	$(RM) $(MLX_DIR)

# Clean and rebuild the program
re: fclean all

-include $(DEPS)

# Mark these targets as not actual files
.PHONY: all miniclean clean fclean re
