/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 21:08:46 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/30 15:17:54 by ugdaniel         ###   ########.fr       */
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
	_default_error_pages[STATUS_SERVICE_UNAVAILABLE] = "www/503.html";
	_default_error_pages[STATUS_HTTP_VERSION_NOT_SUPPORTED] = "www/505.html";
}

Webserv::~Webserv(void)
{
	clean();
}

bool
Webserv::_host_port_already_used(size_t max, const t_listen& l)
{
	for (size_t i = 0; i < max && i < _servers.size(); i++)
		if (_servers[i].get_listen() == l)
			return (true);
	return (false);
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
	if (!_servers.size())
	{
		std::cout << "webserv: no servers found." << std::endl;
		exit(EXIT_SUCCESS);
	}
	try
	{
		_sockets = (int *)malloc(sizeof(int) * _nsockets);
		if (!_sockets)
			_throw_errno("init: malloc");
		for (size_t i = 0; i < _servers.size(); i++)
		{
			if (_host_port_already_used(i, _servers[i].get_listen()))
				continue ;
			std::cout << _servers[i];
			_servers[i].setup();
			memset(&_sockets[i], 0, sizeof(struct pollfd));
			_sockets[i] = _servers[i].get_socket();
			WS_VALUE_LOG("Socket", _sockets[i]);
			_nsockets++;
		}
	}
	catch (std::runtime_error& e)
	{
		std::cerr << "webserv: error: " << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

inline void	copy_fds(struct pollfd *to, const struct pollfd *from)
{
	for (size_t i = 0; i < MAX_CONNECTIONS; i++)
	{
		to[i].fd = from[i].fd;
		to[i].events = POLLIN;
	}
}

void
Webserv::run(void)
{
	int		_poll_ret;
	nfds_t	_nfds;

	_poll_fds = (struct pollfd *)std::malloc(sizeof(struct pollfd) * MAX_CONNECTIONS * _nsockets);
	if (_poll_fds == NULL)
		_throw_errno("init: malloc");
	for (size_t i = 0; i < _nsockets; i++)
	{
		_poll_fds[i * MAX_CONNECTIONS].fd = _sockets[i];
		_poll_fds[i * MAX_CONNECTIONS].events = POLLIN;
		for (size_t j = 1; j < MAX_CONNECTIONS; j++)
		{
			_poll_fds[i * MAX_CONNECTIONS + j].fd = -1;
			_poll_fds[i * MAX_CONNECTIONS + j].events = POLLIN;
		}
	}
	_nfds = _nsockets * MAX_CONNECTIONS;
	while (_running)
	{
		_poll_ret = poll(_poll_fds, _nfds, 0);
		if (_poll_ret > 0)
			for (size_t i = 0; i < _nsockets; i++)
				_servers[i].handle_connections(&_poll_fds[i * MAX_CONNECTIONS]);
	}
}

void
Webserv::clean(void)
{
	if (_poll_fds)
	{
		free(_poll_fds);
		_poll_fds = NULL;
	}
	for (std::vector<Server>::iterator it = _servers.begin(); it != _servers.end(); it++)
		(*it).clean();
	if (_sockets)
	{
		free(_sockets);
		_sockets = NULL;
	}
}
