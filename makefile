NAME	=	fractol

CC	=	cc

SRC =	fractol.c Mandelbrot.c julia.c minihelper.c ft_atod.c arena.c arena_alloc.c colors.c

OBJ = ${SRC:.c=.o}
CFLAGS = -Wall -Wextra -O3

.c.o:
	$(CC) $(CFLAGS) -I/minilibx-linux -c $< -o $(<:.c=.o)

$(NAME): $(OBJ)
	cd ./minilibx-linux && ./configure && $(MAKE)
	$(CC) ${OBJ} $(CFLAGS) -Iminilibx-linux minilibx-linux/libmlx_Linux.a -lXext -lX11 -lm -lz -o $(NAME)

all: $(NAME)

clean:
	@rm -rf ${OBJ}
	@cd ./minilibx-linux && $(MAKE) clean

fclean: clean
	@rm -rf $(NAME)
	@cd ./minilibx-linux && $(MAKE) clean

re: fclean all

.PHONY: all clean fclean re
