# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/24 20:51:30 by ugdaniel          #+#    #+#              #
#    Updated: 2022/11/02 15:29:18 by ugdaniel         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =	webserv

SRCS =	srcs/main.cpp \
		srcs/core/mkdir_p.cpp \
		srcs/core/string_utils.cpp \
		srcs/core/Location.cpp \
		srcs/core/Server.cpp \
		srcs/core/Webserv.cpp \
		srcs/http/Autoindex.cpp \
		srcs/http/raw.cpp \
		srcs/http/Request.cpp \
		srcs/http/Response.cpp \
		srcs/http/ResponseHeaders.cpp \
		srcs/parser/ConfigParser.cpp \
		srcs/parser/RequestParser.cpp \
		srcs/parser/Token.cpp

OBJS = $(SRCS:.cpp=.o)

CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -pedantic -fsanitize=address
INCLUDE = -I include -I include/core -I include/http -I include/parser

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

debug:
	$(CC) $(INCLUDE) -DDEBUG $(SRCS) -o debug

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME) debug

re: fclean all

docker: fclean
	docker build -t siege .
	docker run -d --name siege -it siege

.PHONY: all clean fclean re debug docker
