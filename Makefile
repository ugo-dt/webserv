# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/24 20:51:30 by ugdaniel          #+#    #+#              #
#    Updated: 2022/10/24 20:55:26 by ugdaniel         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =	webserv

SRCS =	srcs/main.cpp

OBJS = $(SRCS:.cpp=.o)

CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98
INCLUDE = -I include

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

debug:
	$(CC) $(INCLUDE) $(SRCS) -o $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME) debug
	rm -rf target

re: fclean all

.PHONY: all clean fclean re
