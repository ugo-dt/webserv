/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 13:29:07 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/28 20:30:58 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(const char *request_buffer)
	: _request(0),
	  _header(),
	  _body(),
	  _location(0)
{
	if (!request_buffer)
	{
		_header.set_status(STATUS_BAD_REQUEST);
		_body = "400 Bad request";
	}
	else
	{
		_request = new Request(request_buffer);
		if (!_request)
			_throw_errno("Fatal error");
		_uri = _request->get_uri();
	}
}

Response::~Response()
{
	if (_request)
		delete (_request);
}

/*
	file or directory?
	if directory found then
		if path matches location
			check method
			if default file then
				redirect to default file
			else if autoindex
				get_index() as body
	else if file found then
		get directory path
		if directory matches location
			check method
	else
		not found

	path matches?
	or directory matches?

	method ok?
	root
	redirections
	autoindex if directory?
*/
void
Response::_check_uri(const std::set<Location>& locations)
{
	std::string	_dir;
	size_t		_slash;

	_slash = _uri.find_last_of('/');
	if (_slash != std::string::npos)
		_dir.assign(_uri, 0, _uri.find_last_of('/') + 1);
	else
		_dir = "/";
	WS_VALUE_LOG("Path", _uri);
	WS_VALUE_LOG("Dir ", _dir);

	// Check if route exists for this specific file ...
	for (std::set<Location>::const_iterator l = locations.begin(); l != locations.end(); l++)
	{
		if (_uri == (*l).get_uri())
		{
			WS_VALUE_LOG("Found matching location (file): ",  (*l).get_uri());
			_location = &(*l);
			break ;
		}

	}
	// ... or check if route exists for the directory
	if (!_location)
	{
		for (std::set<Location>::const_iterator l = locations.begin(); l != locations.end(); l++)
		{
			if (_dir == (*l).get_uri())
			{
				WS_VALUE_LOG("Found matching location (directory): ", (*l).get_uri());
				_location = &(*l);
				break ;
			}
		}
	}
	if (_location)
	{
		WS_VALUE_LOG("Location root", _location->get_root());
		if (_location->get_root().size())
			_uri.insert(0, _location->get_root());
		else
			_uri.insert(0, ".");
	}
	else
		_uri.insert(0, ".");
}

void
Response::_get_body(const t_listen& listen)
{
	struct stat		_stat;
	std::string		line;
	std::ifstream	f;
	Autoindex		_ai;

	std::cout << "body: " << _uri << std::endl;
	if (stat(_uri.c_str(), &_stat) == 0)
	{
		if (S_ISREG(_stat.st_mode))
		{
			WS_VALUE_LOG("File found at", _uri);
			f.open(_uri, std::ifstream::out);
			if (!f.is_open())
			{
				_header.set_status(STATUS_INTERNAL_SERVER_ERROR);
				this->_body = "Internal server error";
				return ;
			}
			_header.set_status(STATUS_OK);
			_header.set_content_type(MIME_HTML);
			while (std::getline(f, line))
				this->_body.append(line + "\n");
			f.close();
		}
		else if (S_ISDIR(_stat.st_mode))
		{
			WS_VALUE_LOG("Directory found at", _uri);
			if (_location->is_autoindex_on())
			{
				this->_body = _ai.get_index(_uri, listen);
			}
		}
	}
	else
	{
		WS_VALUE_LOG("No such file or directory", _uri);
		_header.set_status(STATUS_NOT_FOUND);
		this->_body = "404 Not found";
	}
}

void
Response::generate(const std::map<uint16_t, std::string>& error_pages,
                   const std::set<Location>& locations,
				   const t_listen& listen)
{
	if (!_request->is_valid())
	{
		WS_INFO_LOG("Invalid request.");
		_header.set_status(STATUS_BAD_REQUEST);
		_body = "400 Bad request";
		return ;
	}
	WS_INFO_LOG("Valid request.");
	WS_INFO_LOG("Creating response.");

	_check_uri(locations);
	_get_body(listen);
	(void)error_pages;
}

const std::string
Response::str()
{
	std::string	str;

	_header.set_content_length(_body.length());
	str = "HTTP/1.1 " + _header.get_status() + " " + _header.get_status_string() + CRLF;
	str += "Content-Length: " + _header.get_content_length() + CRLF;
	str += "Content-Type: " + _header.get_content_type() + CRLF;
	str += "Date: " + _header.get_date() + CRLF;
	str += "Server: " + _header.get_server() + CRLF;
	str += CRLF;
	str += _body;
	_response_length = str.length();
	return (str);
}

size_t
Response::length()
{
	return (_response_length);
}
