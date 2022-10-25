# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/24 20:51:30 by ugdaniel          #+#    #+#              #
#    Updated: 2022/10/25 15:38:23 by ugdaniel         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =	webserv

SRCS =	srcs/main.cpp \
		srcs/Config.cpp \
		srcs/Webserv.cpp

OBJS = $(SRCS:.cpp=.o)

CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98
INCLUDE = -I include -I include/core -I include/http -I include/parser

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

.PHONY: all clean fclean re debug
