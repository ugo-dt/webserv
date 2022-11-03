/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 21:49:36 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/11/03 10:54:36 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server()
	: _socket(-1),
	  _listen(),
	  _sockaddr(),
	  _sockaddr_len(0),
	  _server_names(),
	  _error_pages(),
	  _client_body_buffer_size(8192),
	  _locations(),
	  _state(0)
{
	_listen.host = "127.0.0.1";
	_listen.port = DEFAULT_PORT;
	_error_pages[STATUS_BAD_REQUEST] = "www/400.html";
	_error_pages[STATUS_FORBIDDEN] = "www/403.html";
	_error_pages[STATUS_NOT_FOUND] = "www/404.html";
	_error_pages[STATUS_METHOD_NOT_ALLOWED] = "www/405.html";
	_error_pages[STATUS_PAYLOAD_TOO_LARGE] = "www/413.html";
	_error_pages[STATUS_INTERNAL_SERVER_ERROR] = "www/500.html";
	_error_pages[STATUS_NOT_IMPLEMENTED] = "www/501.html";
	_error_pages[STATUS_SERVICE_UNAVAILABLE] = "www/503.html";
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
	if (_socket == -1)
		_throw_errno("socket");
	WS_INFO_LOG("Created new socket for " + _listen.host + ":" + to_string(_listen.port) + " (" + to_string(_socket) + ")");
	option = 1;
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) == -1)
	{
		close(_socket);
		_throw_errno("setsockopt");
	}
	
	_sockaddr_len = sizeof(_sockaddr);
	memset((void *)&_sockaddr, 0, _sockaddr_len); 
	_sockaddr.sin_family = AF_INET; 
	if (_listen.host == "localhost" || _listen.host == "0.0.0.0")
		_sockaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	else
		_sockaddr.sin_addr.s_addr = inet_addr(_listen.host.c_str());
	_sockaddr.sin_port = htons(_listen.port);

	if (bind(_socket, (struct sockaddr *)&_sockaddr, _sockaddr_len) == -1)
	{
		close(_socket);
		_throw_errno("bind");
	}

	fcntl(_socket, F_SETFL, O_NONBLOCK);

	if (listen(_socket, MAX_CONNECTIONS) == -1)
	{
		close(_socket);
		_throw_errno("listen");
	}
}

int
Server::generate_response(int& _fd, const Request& _req)
{
	// Create and send response based on request
	ssize_t		_bytes;
	Response	_resp(_req, _listen);
	std::string	_str;

	_resp.generate(_error_pages, _locations, _client_body_buffer_size);
	WS_INFO_LOG("Sending response.");
	_str = _resp.str();
	_bytes = send(_fd, _str.c_str(), _str.length(), 0);
	if (_bytes < 0)
	{
		WS_ERROR_LOG("Could not send response to client (" + to_string(_fd) + "): " + std::strerror(errno));
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
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
const t_listen&	Server::get_listen() const
	{return _listen;}
const std::string& Server::get_host() const
	{return _listen.host;}
const int& Server::get_port() const
	{return _listen.port;}
struct sockaddr_in& Server::get_sockaddr()
	{return _sockaddr;}
const std::set<std::string>& Server::get_server_names() const
	{return _server_names;}
const std::map<u_int16_t, std::string>& Server::get_error_pages() const
	{return _error_pages;}
const size_t& Server::get_client_body_buffer_size() const
	{return _client_body_buffer_size;}
const std::set<Location>& Server::get_locations() const
	{return _locations;}

void Server::set_socket(const int& socket)
	{_socket = socket;}
void Server::set_host(const std::string& host)
{
	if (host == "localhost" || host == "0.0.0.0")
		_listen.host = "127.0.0.1"; 
	else
		_listen.host = host;
}
void Server::set_port(const int& port)
	{_listen.port = port;}
void Server::set_server_names(const std::set<std::string> server_names)
	{_server_names = server_names;}
void Server::set_error_pages(const std::map<u_int16_t, std::string>& error_pages)
	{_error_pages = error_pages;}
void Server::set_client_body_buffer_size(const size_t& size)
	{_client_body_buffer_size = size;}
void Server::set_locations(const std::set<Location>& locations)
	{_locations = locations;}
void Server::add_server_name(const std::string& name)
	{_server_names.insert(name);}
void Server::set_error_page(u_int16_t code, const std::string& path)
	{_error_pages[code] = path;}
void Server::add_location(const Location& l)
	{_locations.insert(l);}

void Server::set_state(unsigned int x)
	{_state |= x;}
unsigned int Server::get_state(void) const
	{return _state;}

std::ostream&	operator<<(std::ostream &o, const Server& s)
{
#ifdef WS_LOG
	o << "\033[93m[Server]" << std::endl << "Host: " << s.get_host() << std::endl
			<< "Port: " << s.get_port() << std::endl << "Names: " << std::endl;
		for (std::set<std::string>::const_iterator i = s.get_server_names().begin(); i != s.get_server_names().end(); i++)
			o << "\t" << (*i) << std::endl;
		o << "Error pages: " << std::endl;
		for (std::map<u_int16_t, std::string>::const_iterator i = s.get_error_pages().begin(); i != s.get_error_pages().end(); i++)
			o << "\t" << (*i).first << ":" << (*i).second << std::endl;
		o << "buffer size: " << s.get_client_body_buffer_size() << std::endl;
		for (std::set<Location>::const_iterator it = s.get_locations().begin(); it != s.get_locations().end(); it++)
		{
			o << "\033[91m[Location]: " << (*it).get_uri() << std::endl
				<< "\033[93mAllowed methods: ";
			if ((*it).get_methods() & METHOD_GET)
				o << "GET ";
			if ((*it).get_methods() & METHOD_HEAD)
				o << "HEAD ";
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
				<< "CGI extensions: " << std::endl;
			for (std::map<std::string, std::string>::const_iterator i = (*it).get_cgi_extensions().begin(); i != (*it).get_cgi_extensions().end(); i++)
				o << "\t" << (*i).first << ":" << (*i).second << std::endl;
			o << "Upload path: " << (*it).get_upload_path() << std::endl;
		}
		o << "--------\033[0m" << std::endl;
#else
	(void)s;
#endif
	return (o);
}
