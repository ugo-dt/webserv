/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 21:49:36 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/26 15:40:09 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server()
	: _socket(-1),
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

	memset((char *)&_sockaddr, 0, sizeof(_sockaddr)); 
	_sockaddr.sin_family = AF_INET; 
	if (_listen.host == "localhost")
		_sockaddr.sin_addr.s_addr = inet_addr("0.0.0.0");
	else
		_sockaddr.sin_addr.s_addr = inet_addr(_listen.host.c_str());
	_sockaddr.sin_port = htons(_listen.port);

	if (bind(_socket, (struct sockaddr *)&_sockaddr, sizeof(_sockaddr)) < 0)
		_throw_errno("bind");

	fcntl(_socket, F_SETFL, O_NONBLOCK);

	if (listen(_socket, MAX_PENDING_CONNECTIONS) < 0)
		_throw_errno("listen");
}

void
Server::clean()
{
	if (_socket != -1)
		close(_socket);
}

const int& Server::get_socket() const
	{return _socket;}
const std::string& Server::get_host() const
	{return _listen.host;}
const int& Server::get_port() const
	{return _listen.port;}
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
