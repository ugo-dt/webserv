/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 21:49:36 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/26 18:40:27 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server()
	: _socket(-1),
	  _fds(),
	  _nfds(1),
	  _listen(),
	  _sockaddr(),
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
	for (size_t i = 0; i < MAX_CONNECTIONS; i++)
	{
		bzero(&_fds[i], sizeof(struct pollfd));
		_fds[i].fd = -1;
	}
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

	bzero((char *)&_sockaddr, sizeof(_sockaddr)); 
	_sockaddr.sin_family = AF_INET; 
	if (_listen.host == "localhost")
		_sockaddr.sin_addr.s_addr = inet_addr("0.0.0.0");
	else
		_sockaddr.sin_addr.s_addr = inet_addr(_listen.host.c_str());
	_sockaddr.sin_port = htons(_listen.port);

	if (bind(_socket, (struct sockaddr *)&_sockaddr, sizeof(_sockaddr)) < 0)
		_throw_errno("bind");

	fcntl(_socket, F_SETFL, O_NONBLOCK);

	if (listen(_socket, MAX_CONNECTIONS) < 0)
		_throw_errno("listen");
}

ssize_t write_n(int fd, char *ptr, size_t n)
{
        ssize_t nleft = n, nwritten;

        while (nleft > 0) {
                if ((nwritten = write(fd, ptr, nleft)) <= 0 ) {
                        if (nwritten < 0 && errno == EINTR) {
                                nwritten = 0;   /* and call write() again */
                        } else {
                                return -1;
                        }
                }

                nleft -= nwritten;
                ptr += nwritten;
        }

        return n;
}

void
Server::wait_connections(void)
{
	int			fd;
	int			_poll_ret;
	size_t		i;
	ssize_t		n;
	socklen_t	addrlen = (socklen_t)sizeof(_sockaddr);

	_fds[0].fd = _socket;
	_fds[0].events = POLLIN;
	_poll_ret = poll(_fds, _nfds, -1);
	if (_poll_ret <= 0)
		return ;

	// Check new connection
	if (_fds[0].revents & POLLIN)
	{
		if ((fd = accept(_socket, (struct sockaddr *)&_sockaddr, (socklen_t *)&addrlen)) < 0)
		{
			std::cerr << "could not accept: " << std::strerror(errno) << std::endl;
			return ;
		}

		// Save client socket into _fds array
		i = 0;
		while (i < MAX_CONNECTIONS)
		{
			if (_fds[i].fd < 0)
			{
				_fds[i].fd = fd;
				break;
			}
			i++;
		}
		if (i == MAX_CONNECTIONS)
		{
			std::cerr << "Error: too many _fds\n" << std::endl;
			close(fd);
		}
		if (i > _nfds)
			_nfds = i;
		_fds[i].events = POLLIN;

		// No more readable file descriptors
		if (--_poll_ret <= 0) {
			return ;
		}
	} 

	// Check all clients to read data
	for (i = 1; i <= _nfds; i++)
	{
		fd = _fds[i].fd;
		if (fd < 0)
			continue;

		// If the client is readable or errors occur
		if (_fds[i].revents & (POLLIN | POLLERR))
		{
			n = read(fd, _buffer, BUFFER_SIZE);
			if (n < 0)
			{
				fprintf(stderr, "Error: read from socket %d\n", fd);
				close(fd);
				_fds[i].fd = -1;
			}
			else if (n == 0)
			{    // connection closed by client
				printf("Close socket %d\n", fd);
				close(fd);
				_fds[i].fd = -1;                                        
			}
			else
			{
				printf("Read %zu bytes from socket %d\n", n, fd);
				write_n(fd, _buffer, n);
			}

			// No more readable file descriptors
			if (--_poll_ret <= 0) {
					break;
			}
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
