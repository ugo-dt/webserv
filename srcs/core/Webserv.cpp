/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 21:08:46 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/31 12:12:31 by ugdaniel         ###   ########.fr       */
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
			std::cout << _servers[i]; // debug
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

void
Webserv::_route_request_to_server(t_client& client, Request *_req)
{
	std::string	_host = "";
	std::string	_port = "";
	Server		*_server = NULL;

	if (_req->get_header_fields().count("Host"))
	{
		_host.assign(_req->get_header_fields().at("Host"), 0, _req->get_header_fields().at("Host").find_first_of(':'));
		if (_req->get_header_fields().at("Host").find(':') != std::string::npos)
			_port.assign(_req->get_header_fields().at("Host"), _req->get_header_fields().at("Host").find_last_of(':') + 1);
	}
	WS_INFO_LOG("client sockaddr: " + inet_ntoa(client.sockaddr.sin_addr) + ", " + htons(client.sockaddr.sin_port));
	WS_INFO_LOG("Request Host: " + _host);
	WS_INFO_LOG("Request Port: " + _port);
	for (size_t i = 0; i < _servers.size(); i++)
	{
		WS_INFO_LOG("Names (" + i + "):");
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
	if (_server)
		_server->generate_response(client.fd, _req);
	else
	{
		std::string	_page = get_raw_page(400);
		_host = "HTTP/1.1 400 Bad Request" CRLF;
		_host += "Server: webserv" CRLF;
		_host += "Content-Length: " + to_string(_page.length()) + CRLF;
		_host += CRLF;
		_host += _page + CRLF;
		send(client.fd, _host.c_str(), _host.length(), 0);
	}
}

void
Webserv::_handle_request(t_client& client)
{
	ssize_t		_bytes;
	char		_buffer[BUFFER_SIZE];
	Request		*_req;

	// We're checking if we can actually read from the file descriptor
	// This will read data without removing it from the queue
	_bytes = recv(client.fd, _buffer, 1, MSG_PEEK);
	if (_bytes <= 0)
	{
		// This should usually not happen, as we're polling through the file descriptors
		// Playing it safe, though
		std::cout << "Connection closed (" << client.fd << ")" << std::endl;
		close_fd(client.fd);
		return ;
	}

	// Read the data from the client.
	memset(_buffer, 0, BUFFER_SIZE);
	_bytes = recv(client.fd, _buffer, BUFFER_SIZE, 0);
	if (_bytes <= 0)
	{
		// =0: connection was closed by client
		// <0: some error happened
		std::cout << "Connection closed (" << client.fd << ")" << std::endl;
		close_fd(client.fd);
		return ;
	}
	_req = new Request(_buffer);
	if (!_req)
		std::cerr << "Could not allocate memory" << std::endl;
	if (!_req->is_valid())
	{
		send(client.fd, "Invalid request\n", 17, 0);
		return ;
	}
	_route_request_to_server(client, _req);
	delete (_req);
	//send(client.fd, "Response\n", 10, 0);
}

void
Webserv::_accept_connection(int& sock_fd, struct pollfd *_fds, const size_t &max)
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
	i = 0;
	while (i < max)
	{
		if (_fds[i].fd < 0)
		{
			_fds[i].fd = client.fd;
			break ;
		}
		i++;
	}
	if (i == max)
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
	client = NULL;
	while (_running)
	{
		_poll_ret = poll(_poll_fds, _nfds, 0);
		if (_poll_ret > 0)
		{
			for (size_t i = 0; i < _nsockets; i++)
			{
				if (_poll_fds[i * MAX_CONNECTIONS].revents & POLLIN)
				{
					_accept_connection(_poll_fds[i * MAX_CONNECTIONS].fd, &_poll_fds[i * MAX_CONNECTIONS], _nfds);
				}
				for (size_t j = 1; j < MAX_CONNECTIONS; j++)
				{
					if (_poll_fds[i * MAX_CONNECTIONS + j].fd < 0)
						continue ;
					if (_poll_fds[i * MAX_CONNECTIONS + j].revents & (POLLIN | POLLERR))
					{
						client = _get_client_with_fd(_poll_fds[i * MAX_CONNECTIONS + j].fd);
						if (client)
							_handle_request(*client);
						client = NULL;
					}
				}
			}
		}
	}
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
