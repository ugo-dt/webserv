# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: madaniel <madaniel@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/24 20:51:30 by ugdaniel          #+#    #+#              #
#    Updated: 2022/10/26 10:43:47 by madaniel         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =	webserv

SRCS =	srcs/main.cpp \
		srcs/core/Webserv.cpp \
		srcs/core/Server.cpp \
		srcs/parser/ConfigParser.cpp \
		srcs/parser/RequestParser.cpp

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

re: fclean all

.PHONY: all clean fclean re debug
