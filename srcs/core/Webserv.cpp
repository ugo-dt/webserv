/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madaniel <madaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 21:08:46 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/26 15:38:03 by madaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

Webserv::Webserv(void)
	: _servers(),
	  _fds(0),
	  _running(true),
	  _default_error_pages()
{
	_default_error_pages[STATUS_BAD_REQUEST] = "www/400.html";
	_default_error_pages[STATUS_FORBIDDEN] = "www/403.html";
	_default_error_pages[STATUS_NOT_FOUND] = "www/404.html";
	_default_error_pages[STATUS_METHOD_NOT_ALLOWED] = "www/405.html";
	_default_error_pages[STATUS_PAYLOAD_TOO_LARGE] = "www/413.html";
	_default_error_pages[STATUS_INTERNAL_SERVER_ERROR] = "www/500.html";
	_default_error_pages[STATUS_NOT_IMPLEMENTED] = "www/501.html";
	_default_error_pages[STATUS_HTTP_VERSION_NOT_SUPPORTED] = "www/505.html";
}

Webserv::~Webserv(void)
{
	clean();
}

void
Webserv::init(int argc, const char **argv)
{
	ConfigParser	parser;
	size_t			_sz;

	_fds = 0;
	parser.init(argc, argv);
	parser.run(_servers);
	_sz = _servers.size();
	if (!_sz)
	{
		std::cout << "webserv: no servers found." << std::endl;
		exit(EXIT_SUCCESS);
	}
	_fds = (struct pollfd *)malloc(sizeof(struct pollfd) * _sz);
	if (!_fds)
		_throw_errno("malloc");
	for (size_t i = 0; i < _sz; i++)
	{
		std::cout << _servers[i];
		_servers[i].init_connection();
		memset(&_fds[i], 0, sizeof(struct pollfd));
		_fds[i].fd = _servers[i].get_socket();
		WS_VALUE_LOG("Socket", _fds[i].fd);
	}
}

void
Webserv::run(void)
{
	while (_running)
	{
		
	}
}

void
Webserv::clean(void)
{
	for (std::vector<Server>::iterator it = _servers.begin(); it != _servers.end(); it++)
		(*it).clean();
	if (_fds)
	{
		free(_fds);
		_fds = NULL;
	}
}
