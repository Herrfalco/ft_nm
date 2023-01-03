NAME	=	ft_nm
SRCS	=	error.c \
		list.c \
		main.c \
		mem.c \
		parse.c
OBJS	=	$(SRCS:.c=.o)
CC	=	gcc
CFLAGS	=	-Wall -Wall -Wextra

all	:	$(NAME)

$(NAME)	:	$(OBJS)
		$(CC) $(CFLAGS) -no-pie $^ -o $@ $(LIBS)
#		$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

%.o	:	%.c
		$(CC) $(CFLAGS) -c $< -o $@

clean	:
		$(RM) $(OBJS)

fclean	:	clean
		rm -rf $(NAME)

re	:	fclean all
