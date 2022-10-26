/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 21:49:36 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/26 14:48:59 by ugdaniel         ###   ########.fr       */
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
	_listen.port = 80;
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
Server::init_connection(void)
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
	std::cout << "host: " << _listen.host << std::endl;
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
	
	//	add to fd set(server_fd)
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