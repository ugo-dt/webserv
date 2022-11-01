/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 21:08:46 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/11/01 14:43:22 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

Webserv::Webserv(void)
	: _servers(),
	  _clients(),
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

int
Webserv::init(int argc, const char **argv)
{
	ConfigParser	parser;
	size_t			i;
	size_t			j;

	try
	{
		parser.init(argc, argv);
		parser.run(_servers);
	}
	catch (std::invalid_argument& e)
	{
		std::cout << e.what() << "\033[0m" << std::endl;
		return (EXIT_FAILURE);
	}
	if (!_servers.size())
	{
		std::cout << "webserv: no servers found." << std::endl;
		exit(EXIT_SUCCESS);
	}
	try
	{
		for (i = 0; i < _servers.size(); i++)
		{
			if (_host_port_already_used(i, _servers[i].get_listen()))
				continue ;
			_nsockets++;
		}
		_sockets = (int *)malloc(sizeof(int) * _nsockets);
		if (!_sockets)
			_throw_errno("init: malloc");
		i = 0;
		j = 0;
		while (i < _nsockets && j < _servers.size())
		{
			if (_host_port_already_used(i, _servers[j].get_listen()))
			{
				j++;
				continue ;
			}
			std::cout << _servers[j]; // debug
			_servers[j].setup();
			_sockets[i] = _servers[j].get_socket();
			i++;
			j++;
		}
	}
	catch (std::runtime_error& e)
	{
		std::cerr << "webserv: error: " << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

Server*
Webserv::_find_matching_server(const std::string& _host, const std::string& _port, t_client& client)
{
	Server	*_server;

	WS_VALUE_LOG("Client address", inet_ntoa(client.sockaddr.sin_addr));
	WS_VALUE_LOG("Client port", htons(client.sockaddr.sin_port));
	WS_VALUE_LOG("Request host", _host);
	WS_VALUE_LOG("Request port", _port);
	_server = NULL;
	for (size_t i = 0; i < _servers.size(); i++)
	{
		WS_VALUE_LOG("Names", i);
		for (std::set<std::string>::const_iterator it = _servers[i].get_server_names().begin(); it != _servers[i].get_server_names().end(); it++)
			WS_INFO_LOG("\t" + (*it));
		if (!_host.empty())
		{
			if (!_port.empty())
			{
				if (_port == to_string(_servers[i].get_port())
					&& (_host == _servers[i].get_host() || _servers[i].get_server_names().count(_host)))
				{
					_server = &_servers[i];
					break ;
				}
			}
			else if (_servers[i].get_server_names().count(_host))
			{
				_server = &_servers[i];
				break ;
			}
		}
		else if (client.sockaddr.sin_port == _servers[i].get_sockaddr().sin_port)
		{
			_server = &_servers[i];
			break ;
		}
	}
	return (_server);
}

int
Webserv::_unchunked_the_request(Request& _req, t_client& client, const char *_buffer)
{
	static bool		_post = false;
	static size_t	_bytes = 0;
	static size_t	_bytes_max = 0;

	if (!_post)
	{
		_req.parse(_buffer);
		if (!_req.is_valid())
		{
			std::cout << "NOT VALID" << std::endl;
			_send_bad_request(client.fd);
			return (1);
		}
		if (_req.get_method() == METHOD_POST)
		{
			if (_req.get_header_fields().count("Content-Length") == 0)
			{
				std::cout << "NO CONTENT LENGTH" << std::endl;
				_send_bad_request(client.fd);
				return (1);
			}
			_bytes_max = atoi(_req.get_header_fields().at("Content-Length").c_str());
			_post = true;
			return (1);
		}
	}
	else
	{
		_bytes += strlen(_buffer);
		_req.append_to_body(_buffer);
		if (_bytes < _bytes_max)
			return (1);
		_post = false;
		_req.set_post_boundary(get_post_header_value(_req.get_header_fields().at("Content-Type"), " boundary="));
		if (_req.get_post_boundary().empty())
			_req.set_post_boundary(get_post_header_value(_req.get_header_fields().at("Content-Type"), "boundary="));
	}
	return (0);
}

void
Webserv::_route_request_to_server(t_client& client, const char *_buffer)
{
	std::string		_host = "";
	std::string		_port = "";
	Server			*_server = NULL;
	static Request	_req;

	std::cout << "\033[95mreading" << std::endl;
	std::cout << _buffer << "\033[0m" << std::endl;
	if (_unchunked_the_request(_req, client, _buffer) == 1)
		return ;
	std::cout << "\033[31munchunked\033[0m" << std::endl;
	if (_req.get_header_fields().count("Host"))
	{
		_host.assign(_req.get_header_fields().at("Host"), 0, _req.get_header_fields().at("Host").find_first_of(':'));
		if (_req.get_header_fields().at("Host").find(':') != std::string::npos)
			_port.assign(_req.get_header_fields().at("Host"), _req.get_header_fields().at("Host").find_last_of(':') + 1);
	}
	_server = _find_matching_server(_host, _port, client);
	if (_server)
		_server->generate_response(client.fd, _req);
	else
	{
		std::cout << "NO SERVER" << std::endl;
		_send_bad_request(client.fd);
	}
}

void
Webserv::_handle_request(t_client& client)
{
	ssize_t		_bytes;
	char		_buffer[BUFFER_SIZE + 1];

	// We're checking if we can actually read from the file descriptor
	// This will read data without removing it from the queue
	_bytes = recv(client.fd, _buffer, 1, MSG_PEEK);
	if (_bytes < 0)
	{
		// This should usually not happen, as we're polling through the file descriptors
		// Playing it safe, though
		std::cout << "Connection closed (" << client.fd << ")" << std::endl;
		close_fd(client.fd);
		return ;
	}

	// Read the data from the client.
	memset(_buffer, 0, BUFFER_SIZE + 1);
	_bytes = recv(client.fd, _buffer, BUFFER_SIZE, 0);
	if (_bytes <= 0)
	{
		// =0: connection was closed by client
		// <0: some error happened
		std::cout << "Connection closed by client (" << client.fd << ")" << std::endl;
		close_fd(client.fd);
		return ;
	}
	_route_request_to_server(client, _buffer);
	//send(client.fd, "Response\n", 10, 0);
}

void
Webserv::_accept_connection(int& sock_fd, const nfds_t &_nfds)
{
	size_t			i;
	t_client		client;
	std::string		_page;
	std::string		_str;

	memset((void *)&client.sockaddr, 0, sizeof(sockaddr));
	client.sockaddr_len = sizeof(client.sockaddr);
	client.fd = accept(sock_fd, (struct sockaddr *)&client.sockaddr, (socklen_t *)&client.sockaddr_len);
	if (client.fd < 0)
	{
		std::cerr << "Refused new connection: " << std::strerror(errno) << std::endl;
		return ;
	}
	if (getsockname(sock_fd, (struct sockaddr *)&client.sockaddr, (socklen_t *)&client.sockaddr_len))
	{
		std::cerr << "Refused new connection: " << std::strerror(errno) << std::endl;
		close(client.fd);
		return ;
	}
	i = _nsockets;
	while (i < _nfds)
	{
		if (_poll_fds[i].fd < 0)
		{
			_poll_fds[i].fd = client.fd;
			break ;
		}
		i++;
	}
	if (i == _nfds)
	{
		std::cout << "Refused new connection: " << "too many clients" << std::endl;
			if (_default_error_pages.count(STATUS_SERVICE_UNAVAILABLE))
				_page = get_body_from_uri(_default_error_pages.at(STATUS_SERVICE_UNAVAILABLE));
			else
				_page = get_raw_page(STATUS_SERVICE_UNAVAILABLE);
			_str = "HTTP/1.1 503 Service Unavailable" CRLF;
			_str += "Server: webserv" CRLF;
			_str += "Content-Length: " + to_string(_page.length()) + CRLF;
			_str += "Content-Type: text/html" CRLF;
			_str += CRLF;
			_str += _page + CRLF;
			send(client.fd, _str.c_str(), _str.length(), 0);
			close(client.fd);
	}
	else
	{
		std::cout << "Accepted new connection (" << client.fd << ")" << std::endl;
		_clients.push_back(client);
	}
}

void
Webserv::run(void)
{
	t_client	*client;
	int			_poll_ret;
	nfds_t		_nfds;

	_nfds = _nsockets + MAX_CONNECTIONS;
	_poll_fds = (struct pollfd *)std::malloc(sizeof(struct pollfd) * _nfds);
	if (_poll_fds == NULL)
		_throw_errno("init: malloc");
	for (size_t i = 0; i < _nsockets; i++)
	{
		_poll_fds[i].fd = _sockets[i];
		_poll_fds[i].events = POLLIN;
	}
	for (size_t i = _nsockets; i < _nfds; i++)
	{
		_poll_fds[i].fd = -1;
		_poll_fds[i].events = POLLIN;
	}
	client = NULL;
	while (_running)
	{
		_poll_ret = poll(_poll_fds, _nfds, 0);
		if (_poll_ret > 0)
		{
			for (size_t i = 0; i < _nsockets; i++)
				if (_poll_fds[i].revents & POLLIN)
					_accept_connection(_poll_fds[i].fd, _nfds);
			for (size_t i = _nsockets; i < _nfds; i++)
			{
				if (_poll_fds[i].fd < 0)
					continue ;
				if (_poll_fds[i].revents & POLLNVAL)
				{
					close_fd(_poll_fds[i].fd);
					_poll_fds[i].revents = 0;
					continue ;
				}
				if (_poll_fds[i].revents & (POLLIN | POLLERR))
				{
					client = _get_client_with_fd(_poll_fds[i].fd);
					if (client)
						_handle_request(*client);
					client = NULL;
				}
			}
		}
	}
}

void
Webserv::_send_bad_request(int& fd)
{
	std::string	_page;
	std::string	_str;
	
	_page = get_raw_page(400);
	_str = "HTTP/1.1 400 Bad Request" CRLF;
	_str += "Server: webserv" CRLF;
	_str += "Content-Length: " + to_string(_page.length()) + CRLF;
	_str += CRLF;
	_str += _page + CRLF;
	send(fd, _str.c_str(), _str.length(), 0);
}

bool
Webserv::_host_port_already_used(size_t max, const t_listen& l)
{
	for (size_t i = 0; i < max && i < _servers.size(); i++)
		if (_servers[i].get_listen() == l)
			return (true);
	return (false);
}

t_client*
Webserv::_get_client_with_fd(const int& fd)
{
	for (std::vector<t_client>::iterator it = _clients.begin(); it != _clients.end(); it++)
		if ((*it).fd == fd)
			return (&*it);
	return (NULL);
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
