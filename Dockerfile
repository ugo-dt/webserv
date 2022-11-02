# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Dockerfile                                         :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/02 14:36:22 by ugdaniel          #+#    #+#              #
#    Updated: 2022/11/02 15:55:15 by ugdaniel         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

FROM    debian:buster

RUN     apt-get update -y
RUN     apt-get install -y siege curl netcat make build-essential

RUN     mkdir -p     /webserv/ /webserv/srcs /webserv/include /webserv/config /webserv/www
COPY    ./srcs/      /webserv/srcs
COPY    ./include/   /webserv/include
COPY    ./config/    /webserv/config
COPY    ./www/       /webserv/www
COPY    ./Makefile   /webserv/
