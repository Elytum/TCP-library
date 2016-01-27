#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: achazal <achazal@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/01/28 22:41:37 by achazal           #+#    #+#              #
#    Updated: 2015/01/28 22:41:42 by achazal          ###   ########.fr        #
#                                                                              #
#******************************************************************************#

.PHONY: all, clean, fclean, mrproper, re, fast, .SILENT

CC = gcc
NAME = htable.a
CFLAGS = -Wall -Werror -Wextra -I./ -O3
SOURCE = htable.c

OBJ = $(SOURCE:.c=.o)


ifeq ($(FAST), 1)
	CFLAGS := -O3 $(CFLAGS)
else
	CFLAGS := -O2 $(CFLAGS)
endif


all : $(NAME)

$(NAME) : $(OBJ)
	@export result=`ar rcs $@ $(OBJ) 2>&1` ; \
	if [ "$$result" != "" ] ; then \
		exit 1 ; \
	fi

ifeq ($(FAST), 1)

%.o : %_fast.c libft.h
	@export result=`$(CC) $(CFLAGS) -c $< -o $@ 2>&1` ; \
	if [ "$$result" != "" ] ; then \
		exit 1 ; \
	fi
endif


%.o : %.c libft.h
	@export result=`$(CC) $(CFLAGS) -c $< -o $@ 2>&1` ; \
	if [ "$$result" != "" ] ; then \
		exit 1 ; \
	fi

fast:
	@make -C . FAST=1

clean :
	@rm -rf $(OBJ)

fclean : clean
	@rm -rf $(NAME)

mrproper: fclean

re : fclean all

.SILENT: