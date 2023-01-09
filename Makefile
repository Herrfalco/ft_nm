NAME	=	ft_nm
SRCS	=	srcs/error.c \
		srcs/sym.c \
		srcs/sym_lst.c \
		srcs/main.c \
		srcs/mem.c \
		srcs/sec.c \
		srcs/arch.c
OBJS	=	$(SRCS:.c=.o)
CC	=	gcc
CFLAGS	=	-Wall -Wall -Wextra

all	:	$(NAME)

$(NAME)	:	$(OBJS)
#		$(CC) $(CFLAGS) -no-pie $^ -o $@ $(LIBS)
#		$(CC) $(CFLAGS) -g $^ -o $@ $(LIBS)
		$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

%.o	:	%.c
		$(CC) $(CFLAGS) -c $< -o $@
#		$(CC) $(CFLAGS) -g -c $< -o $@

clean	:
		$(RM) $(OBJS)

fclean	:	clean
		rm -rf $(NAME)

re	:	fclean all
