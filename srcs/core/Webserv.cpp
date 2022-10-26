/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madaniel <madaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 21:08:46 by ugdaniel          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2022/10/26 14:53:45 by madaniel         ###   ########.fr       */
=======
/*   Updated: 2022/10/26 14:43:36 by ugdaniel         ###   ########.fr       */
>>>>>>> aac0f07a580712943afbaf1e21270a6e8842bb5e
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

void print_server_info_debug(const Server &s)
{
	std::cout << "\e[93m[Server]" << std::endl << "Host: " << s.get_host() << std::endl
		<< "Port: " << s.get_port() << std::endl << "Names: " << std::endl;
	for (std::set<std::string>::const_iterator i = s.get_server_names().begin(); i != s.get_server_names().end(); i++)
		std::cout << "\t" << (*i) << std::endl;
	std::cout << "Error pages: " << std::endl;
	for (std::map<uint16_t, std::string>::const_iterator i = s.get_error_pages().begin(); i != s.get_error_pages().end(); i++)
		std::cout << "\t" << (*i).first << ":" << (*i).second << std::endl;
	std::cout << "buffer size: " << s.get_client_body_buffer_size() << std::endl;
	for (std::set<Location>::const_iterator it = s.get_locations().begin(); it != s.get_locations().end(); it++)
	{
		std::cout << "\e[91m[Location]: " << (*it).get_uri() << std::endl
			<< "\e[93mAllowed methods: ";
		if ((*it).get_methods() & METHOD_GET)
			std::cout << "GET ";
		if ((*it).get_methods() & METHOD_POST)
			std::cout << "POST ";
		if ((*it).get_methods() & METHOD_DELETE)
			std::cout << "DELETE";
		std::cout << std::endl << "Redirections: " << std::endl;
		for (std::map<std::string, std::string>::const_iterator i = (*it).get_redirections().begin(); i != (*it).get_redirections().end(); i++)
			std::cout << "\t" << (*i).first << ":" << (*i).second << std::endl;
		std::cout << "Root: " << (*it).get_root() << std::endl
			<< "Autoindex: " << (*it).is_autoindex_on() << std::endl
			<< "Default file: " << (*it).get_default_file() << std::endl
			<< "CGI extension: " << (*it).get_cgi_extension() << std::endl
			<< "Upload path: " << (*it).get_upload_path() << std::endl;
	}
	std::cout << "--------\e[0m" << std::endl;
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
		print_server_info_debug(_servers.at(i));
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
