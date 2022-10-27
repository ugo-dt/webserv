/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 21:49:36 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/27 13:11:20 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server()
	: _socket(-1),
	  _fds(),
	  _nfds(MAX_CONNECTIONS),
	  _buffer(),
	  _listen(),
	  _sockaddr(),
	  _sockaddr_len(0),
	  _server_names(),
	  _error_pages(),
	  _client_body_buffer_size(8192),
	  _locations(),
	  _state(0)
{
	_listen.host = "0.0.0.0";
	_listen.port = DEFAULT_PORT;
	_error_pages[STATUS_BAD_REQUEST] = "www/400.html";
	_error_pages[STATUS_FORBIDDEN] = "www/403.html";
	_error_pages[STATUS_NOT_FOUND] = "www/404.html";
	_error_pages[STATUS_METHOD_NOT_ALLOWED] = "www/405.html";
	_error_pages[STATUS_PAYLOAD_TOO_LARGE] = "www/413.html";
	_error_pages[STATUS_INTERNAL_SERVER_ERROR] = "www/500.html";
	_error_pages[STATUS_NOT_IMPLEMENTED] = "www/501.html";
	_error_pages[STATUS_HTTP_VERSION_NOT_SUPPORTED] = "www/505.html";
}

Server::~Server()
{
	clean();
}

void
Server::setup(void)
{
	int		option;

	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket < 0)
		_throw_errno("socket");
	option = 1;
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)))
		_throw_errno("setsockopt");
	
	_sockaddr_len = sizeof(_sockaddr);
	bzero((char *)&_sockaddr, _sockaddr_len); 
	_sockaddr.sin_family = AF_INET; 
	if (_listen.host == "localhost")
		_sockaddr.sin_addr.s_addr = inet_addr("0.0.0.0");
	else
		_sockaddr.sin_addr.s_addr = inet_addr(_listen.host.c_str());
	_sockaddr.sin_port = htons(_listen.port);

	if (bind(_socket, (struct sockaddr *)&_sockaddr, _sockaddr_len) < 0)
		_throw_errno("bind");

	fcntl(_socket, F_SETFL, O_NONBLOCK);

	if (listen(_socket, MAX_CONNECTIONS) < 0)
		_throw_errno("listen");
	_fds[0].fd = _socket;
	_fds[0].events = POLLIN;
	for (size_t i = 1; i < MAX_CONNECTIONS; i++)
	{
		_fds[i].fd = -1;
		_fds[i].events = POLLIN; 
	}
}

void
Server::_handle_request(int& _fd)
{
	size_t	_bytes;

	// This will read data without removing it from the queue.
	_bytes = recv(_fd, _buffer, 1, MSG_PEEK);
	if (_bytes < 0)
	{
		fprintf(stderr, "Error: read from socket %d\n", _fd);
		std::cout << "Could not read from client: " << std::strerror(errno) << std::endl;
		WS_VALUE_LOG("Socket closed", _fd);
		close(_fd);
		_fd = -1;
		return ;
	}

	// Read the data from the client.
	bzero(_buffer, BUFFER_SIZE);
	_bytes = recv(_fd, _buffer, BUFFER_SIZE, 0);

	if (_bytes == 0)
	{
		// Connection closed by client.
		std::cout << "Connection closed." << std::endl;
		close(_fd);
		WS_VALUE_LOG("Socket closed", _fd);
		_fd = -1;
		return ;
	}
	// Parse request
	// Create response
	// Send response

	printf("Read %zu bytes from socket %d\n", _bytes, _fd);
	send(_fd, "Bien recu\n", 11, 0);
}

void
Server::wait_connections(void)
{
	int			_incoming_fd;
	int			_poll_ret;
	size_t		i;

	_poll_ret = poll(_fds, _nfds, 0);
	if (_poll_ret <= 0)
		return ;

	// Check new connection
	if (_fds[0].revents & POLLIN)
	{
		_incoming_fd = accept(_socket, (struct sockaddr *)&_sockaddr, (socklen_t *)&_sockaddr_len);
		if (_incoming_fd < 0)
		{
			std::cerr << "could not accept: " << std::strerror(errno) << std::endl;
			return ;
		}
		printf("Accept socket %d (%s : %hu)\n", 
				_incoming_fd,
            	inet_ntoa(_sockaddr.sin_addr),
                ntohs(_sockaddr.sin_port));

		// Save client socket into _fds array
		i = 0;
		while (i < MAX_CONNECTIONS)
		{
			if (_fds[i].fd < 0)
			{
				_fds[i].fd = _incoming_fd;
				break;
			}
			i++;
		}
		if (i == MAX_CONNECTIONS)
		{
			std::cout << "Refused new connection: too many clients" << std::endl;
			close(_incoming_fd);
		}
		_fds[i].events = POLLIN;

		// No more readable file descriptors
		if (--_poll_ret <= 0)
			return ;
	}

	// Check all clients to read data
	for (i = 1; i < _nfds; i++)
	{
		if (_fds[i].fd < 0)
			continue;

		// If the client is readable or errors occur
		if (_fds[i].revents & (POLLIN | POLLERR))
		{
			_handle_request(_fds[i].fd);
			if (--_poll_ret <= 0)
				break;
		}
	}
}

void
Server::clean()
{
	// close fds
	if (_socket != -1)
	{
		close(_socket);
		_socket = -1;
	}
}

const int& Server::get_socket() const
	{return _socket;}
const std::string& Server::get_host() const
	{return _listen.host;}
const int& Server::get_port() const
	{return _listen.port;}
struct sockaddr_in& Server::get_sockaddr()
	{return _sockaddr;}
const std::set<std::string>& Server::get_server_names() const
	{return _server_names;}
const std::map<uint16_t, std::string>& Server::get_error_pages() const
	{return _error_pages;}
const size_t& Server::get_client_body_buffer_size() const
	{return _client_body_buffer_size;}
const std::set<Location>& Server::get_locations() const
	{return _locations;}

void Server::set_socket(const int& socket)
	{_socket = socket;}
void Server::set_host(const std::string& host)
	{_listen.host = host;}
void Server::set_port(const int& port)
	{_listen.port = port;}
void Server::set_server_names(const std::set<std::string> server_names)
	{_server_names = server_names;}
void Server::set_error_pages(const std::map<uint16_t, std::string>& error_pages)
	{_error_pages = error_pages;}
void Server::set_client_body_buffer_size(const size_t& size)
	{_client_body_buffer_size = size;}
void Server::set_locations(const std::set<Location>& locations)
	{_locations = locations;}
void Server::add_server_name(const std::string& name)
	{_server_names.insert(name);}
void Server::set_error_page(uint16_t code, const std::string& path)
	{_error_pages[code] = path;}
void Server::add_location(const Location& l)
	{_locations.insert(l);}

void Server::set_state(unsigned int x)
	{_state |= x;}
unsigned int Server::get_state(void) const
	{return _state;}

std::ostream&	operator<<(std::ostream &o, const Server& s)
{
#ifdef DEBUG
	o << "\e[93m[Server]" << std::endl << "Host: " << s.get_host() << std::endl
			<< "Port: " << s.get_port() << std::endl << "Names: " << std::endl;
		for (std::set<std::string>::const_iterator i = s.get_server_names().begin(); i != s.get_server_names().end(); i++)
			o << "\t" << (*i) << std::endl;
		o << "Error pages: " << std::endl;
		for (std::map<uint16_t, std::string>::const_iterator i = s.get_error_pages().begin(); i != s.get_error_pages().end(); i++)
			o << "\t" << (*i).first << ":" << (*i).second << std::endl;
		o << "buffer size: " << s.get_client_body_buffer_size() << std::endl;
		for (std::set<Location>::const_iterator it = s.get_locations().begin(); it != s.get_locations().end(); it++)
		{
			o << "\e[91m[Location]: " << (*it).get_uri() << std::endl
				<< "\e[93mAllowed methods: ";
			if ((*it).get_methods() & METHOD_GET)
				o << "GET ";
			if ((*it).get_methods() & METHOD_POST)
				o << "POST ";
			if ((*it).get_methods() & METHOD_DELETE)
				o << "DELETE";
			o << std::endl << "Redirections: " << std::endl;
			for (std::map<std::string, std::string>::const_iterator i = (*it).get_redirections().begin(); i != (*it).get_redirections().end(); i++)
				o << "\t" << (*i).first << ":" << (*i).second << std::endl;
			o << "Root: " << (*it).get_root() << std::endl
				<< "Autoindex: " << (*it).is_autoindex_on() << std::endl
				<< "Default file: " << (*it).get_default_file() << std::endl
				<< "CGI extension: " << (*it).get_cgi_extension() << std::endl
				<< "Upload path: " << (*it).get_upload_path() << std::endl;
		}
		o << "--------\e[0m" << std::endl;
#else
	(void)s;
#endif
	return (o);
}
