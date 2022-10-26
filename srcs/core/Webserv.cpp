/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 21:08:46 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/26 11:03:51 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

Webserv::Webserv(void)
	: _servers(),
	  _fds(NULL),
	  _running(true)
{
}

Webserv::~Webserv(void)
{
	clean();
}

void
Webserv::init(int argc, const char **argv)
{
	ConfigParser	parser(argc, argv);
	size_t			_sz;

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
		std::cout << _servers[i].get_client_body_buffer_size() << std::endl;
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
		free(_fds);
}
