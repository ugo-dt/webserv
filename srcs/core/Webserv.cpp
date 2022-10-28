/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 21:08:46 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/28 15:50:27 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

Webserv::Webserv(void)
	: _servers(),
	  _sockets(0),
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

int
Webserv::init(int argc, const char **argv)
{
	ConfigParser	parser;

	try
	{
		parser.init(argc, argv);
		parser.run(_servers);
	}
	catch (std::invalid_argument& e)
	{
		std::cout << e.what() << "\e[0m" << std::endl;
		return (EXIT_FAILURE);
	}
	_nsockets = _servers.size();
	if (!_nsockets)
	{
		std::cout << "webserv: no servers found." << std::endl;
		exit(EXIT_SUCCESS);
	}
	try
	{
		_sockets = (int *)malloc(sizeof(int) * _nsockets);
		if (!_sockets)
			_throw_errno("malloc");
		for (size_t i = 0; i < _nsockets; i++)
		{
			std::cout << _servers[i];
			_servers[i].setup();
			memset(&_sockets[i], 0, sizeof(struct pollfd));
			_sockets[i] = _servers[i].get_socket();
			WS_VALUE_LOG("Socket", _sockets[i]);
		}
	}
	catch (std::runtime_error& e)
	{
		std::cerr << "webserv: error: " << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

void
Webserv::run(void)
{
	while (_running)
		for (size_t i = 0; i < _nsockets; i++)
			_servers[i].wait_connections();
}

void
Webserv::clean(void)
{
	for (std::vector<Server>::iterator it = _servers.begin(); it != _servers.end(); it++)
		(*it).clean();
	if (_sockets)
	{
		free(_sockets);
		_sockets = NULL;
	}
}
