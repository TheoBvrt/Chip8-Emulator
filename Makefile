NAME = chip8
SRCS = src/main.c src/display.c src/init.c src/processor.c
OBJS = $(addprefix OBJ/, $(notdir $(SRCS:.c=.o)))
CC = gcc
RM = rm -f
CFLAGS = -Wall -Wextra -Werror

OBJ_DIR = OBJ

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $(NAME) -L./ -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

$(OBJ_DIR)/%.o : src/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) -c $< -o $@

clean:
	$(RM) -r $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all