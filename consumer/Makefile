# **************************************************************************** #
#                                                                             #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: achazal <achazal@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2014/11/03 11:54:29 by achazal           #+#    #+#              #
#    Updated: 2014/12/13 12:55:35 by achazal          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# -------------Compilateur------------------#
CC		=	gcc
#--------------Name-------------------------#
NAME	=	consumer
MYPATH	=	$(HOME)
#--------------Sources----------------------#
FILES	=	common.c			\
			exchanges.c			\
			requests/request.c	\
			requests/requests.c	\
			consumer/consumer.c	\
			login.c				\
			sockets/sockets.c	\
			sockets/add_child.c	\
			producer/producer.c

INC		=	-I ../includes -I ../htable -I ../srcs/requests
CCFLAGS	=	-Wall -Wextra -g -O3

SRCS	=	$(addprefix ../srcs/, $(FILES))
OBJS	=	main.o $(SRCS:.c=.o)

LIBS	=	../htable/htable.a
#--------------Actions----------------------#

.PHONY: LIBRARIES $(NAME) clean fclean re

all: $(NAME)

LIBRARIES:
	make re -C ../htable

$(NAME): LIBRARIES $(OBJS)
	$(CC) $(CCFLAGS) $(INC) $(OBJS) -o $(NAME) $(LIBS) -O3

%.o: %.c
	$(CC) $(CCFLAGS) -c  $(INC) $< -o $@
	
clean:
	rm -f $(OBJS)
	
fclean:	clean
	rm -f $(NAME)

re: fclean all
	make re -C ../htable
	make