/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 21:08:46 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/11/01 20:15:55 by ugdaniel         ###   ########.fr       */
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
		_nfds = MAX_CONNECTIONS * _nsockets + _nsockets;
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
			_send_bad_request(client.fd);
			return (1);
		}
		if (_req.get_method() == METHOD_POST)
		{
			if (_req.get_header_fields().count("Content-Length") == 0)
			{
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

	// std::cout << "\033[95mreading" << std::endl;
	// std::cout << _buffer << "\033[0m" << std::endl;
	if (_unchunked_the_request(_req, client, _buffer) == 1)
		return ;
	// std::cout << "\033[31munchunked\033[0m" << std::endl;
	if (_req.get_header_fields().count("Host"))
	{
		_host.assign(_req.get_header_fields().at("Host"), 0, _req.get_header_fields().at("Host").find_first_of(':'));
		if (_req.get_header_fields().at("Host").find(':') != std::string::npos)
			_port.assign(_req.get_header_fields().at("Host"), _req.get_header_fields().at("Host").find_last_of(':') + 1);
	}
	if (_host == "localhost" || _host == "0.0.0.0")
		_host = "127.0.0.1";
	_server = _find_matching_server(_host, _port, client);
	if (_server)
		_server->generate_response(client.fd, _req);
	else
	{
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
		_remove_client(client);
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
		_remove_client(client);
		return ;
	}
	_route_request_to_server(client, _buffer);
}

void
Webserv::_accept_connection(int& sock_fd)
{
	size_t			i;
	t_client		client;
	std::string		_page;
	std::string		_str;

	memset((void *)&client.sockaddr, 0, sizeof(sockaddr));
	client.sockaddr_len = sizeof(client.sockaddr);
	client.fd = accept(sock_fd, (struct sockaddr *)&client.sockaddr, (socklen_t *)&client.sockaddr_len);
	if (client.fd == -1)
	{
		std::cerr << "Refused new connection: " << std::strerror(errno) << std::endl;
		return ;
	}
	WS_VALUE_LOG("client.fd", client.fd);
	if (getsockname(sock_fd, (struct sockaddr *)&client.sockaddr, (socklen_t *)&client.sockaddr_len) == -1)
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
	if (i == _nfds || _clients.size() == _nfds)
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
		return ;
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
	int	client;
	int	_poll_ret;

	_clients.reserve(_nfds);
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
	while (_running)
	{
		client = 0;
		_poll_ret = poll(_poll_fds, _nfds, 0);
		if (_poll_ret == -1)
		{
			std::cout << "poll error: " << strerror(errno) << std::endl;
			_running = false;
		}
		else if (_poll_ret > 0)
		{
			for (size_t i = 0; i < _nsockets; i++)
				if (_poll_fds[i].revents & POLLIN)
					_accept_connection(_poll_fds[i].fd);
			for (size_t i = _nsockets; i < _nfds; i++)
			{
				if (_poll_fds[i].fd == -1)
					continue ;
				client = _get_client_with_fd(_poll_fds[i].fd);
				if (_poll_fds[i].revents & (POLLNVAL | POLLHUP))
				{
					std::cout << "Connection closed (" << _poll_fds[i].fd << ")" << std::endl;
					_remove_client(_clients[client]);
					_poll_fds[i].revents = 0;
					continue ;
				}
				else if (_poll_fds[i].revents & (POLLIN | POLLERR))
				{
					if (client != -1)
						_handle_request(_clients[client]);
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

int
Webserv::_get_client_with_fd(int fd)
{
	if (fd == -1)
		return (-1);
	for (size_t i = 0; i < _clients.size(); i++)
		if (_clients[i].fd == fd)
			return (i);
	return (-1);
}

void
Webserv::_remove_client(t_client& client)
{
	for (size_t i = 0; i < _nfds; i++)
	{
		if (_poll_fds[i].fd == client.fd)
		{
			_poll_fds[i].fd = -1;
			break ;
		}
	}
	for (std::vector<t_client>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if ((*it) == client)
		{
			close(client.fd);
			_clients.erase(it);
			break ;
		}
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
