# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/09 16:23:01 by fcadet            #+#    #+#              #
#    Updated: 2023/01/09 19:31:31 by fcadet           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	ft_nm
SRCS	=	srcs/error.c \
		srcs/sym.c \
		srcs/sym_lst.c \
		srcs/main.c \
		srcs/mem.c \
		srcs/sec.c \
		srcs/arch.c \
		srcs/opts.c \
		srcs/utils.c
OBJS	=	$(SRCS:.c=.o)
CC	=	gcc
CFLAGS	=	-Wall -Wextra -Werror

all	:	$(NAME)

$(NAME)	:	$(OBJS)
		$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

%.o	:	%.c
		$(CC) $(CFLAGS) -c $< -o $@

clean	:
		$(RM) $(OBJS)

fclean	:	clean
		rm -rf $(NAME)

re	:	fclean all
