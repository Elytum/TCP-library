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
NAME	=	server
MYPATH	=	$(HOME)
#--------------Sources----------------------#
FILES	=	server.c			\
			server_events.c		\
			common.c			\
			exchanges.c

INC		=	-I ../includes -I ../htable
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