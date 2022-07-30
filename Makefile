NAME		=	cub3D

SRCS		=	game.c

MLX			=	libmlx.a

MLXDIR		=	./minilibx

MLXFLG		=	./libmlx.a -framework OpenGL -framework Appkit

OBJS		=	${SRCS:.c=.o}

CC			=	gcc -g3

CFLAGS		=	

${NAME}:	${OBJS}
			${MAKE} -C ${MLXDIR}
			cp ${MLXDIR}/${MLX} ./
			${CC} ${CFLAGS} ${OBJS} ${LIBFT} ${MLXFLG} -o ${NAME} -Ofast

all:	${NAME}

vg:		${NAME}
		valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes ./cub3d file.cub

clean:
		rm -rf ${OBJS}
		rm -rf ${MLX}
		${MAKE} clean -C ${MLXDIR}

fclean:	clean
		rm -rf ${NAME}

re: fclean all

.PHONY : all clean re fclean