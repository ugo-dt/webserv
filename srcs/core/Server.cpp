/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 21:49:36 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/25 23:31:31 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server()
	: _socket(-1),
	  _listen(),
	  _sockaddr(),
	  _server_names(),
	  _error_pages(),
	  _client_body_buffer_size(8192),
	  _locations()
{
	_listen.host = "";
	_listen.port = -1;
}

Server::~Server()
{
	clean();
}

void
Server::init(void)
{
	int		option;

	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket < 0)
		_throw_errno("socket");
	option = 1;
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)))
		_throw_errno("setsockopt");

	if (_listen.port == -1)
		throw std::invalid_argument("invalid port");
	memset((char *)&_sockaddr, 0, sizeof(_sockaddr)); 
	_sockaddr.sin_family = AF_INET; 
	_sockaddr.sin_addr.s_addr = inet_addr(_listen.host.c_str());
	_sockaddr.sin_port = htons(_listen.port);

	if (bind(_socket, (struct sockaddr *)&_sockaddr, sizeof(_sockaddr)) < 0)
		_throw_errno("bind");

	fcntl(_socket, F_SETFL, O_NONBLOCK);

	if (listen(_socket, MAX_PENDING_CONNECTIONS) < 0)
		_throw_errno("listen");
	
	//	add to fd set(server_fd)
}

void
Server::clean()
{
	if (_socket != -1)
		close(_socket);
}
