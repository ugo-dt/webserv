# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Dockerfile                                         :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/02 14:36:22 by ugdaniel          #+#    #+#              #
#    Updated: 2022/11/02 15:28:45 by ugdaniel         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

FROM    debian:buster

RUN     apt-get update -y
RUN     apt-get install -y siege curl make git

RUN     mkdir -p     /webserv
COPY    ./srcs/      /webserv
COPY    ./include/   /webserv
COPY    ./config/    /webserv
COPY    ./www/       /webserv
COPY    ./Makefile   /webserv
