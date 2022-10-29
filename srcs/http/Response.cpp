/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 13:29:07 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/29 12:19:32 by ugdaniel         ###   ########.fr       */
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

	if (_uri[0] != '/')
		_uri.insert(0, "/");
	_slash = _uri.find_last_of('/');
	if (_slash != std::string::npos)
		_dir.assign(_uri, 0, _uri.find_last_of('/') + 1);
	else
		_dir = "/";
	WS_VALUE_LOG("Path", _uri);
	WS_VALUE_LOG("Dir ", _dir);

	_location = NULL;
	// Check if route exists for this specific file ...
	if (_uri != "/" && _uri[_uri.length() - 1] != '/')
	{
		for (std::set<Location>::const_iterator l = locations.begin(); l != locations.end(); l++)
		{
			if (_uri == (*l).get_uri())
			{
				WS_VALUE_LOG("Found matching location (file): ",  (*l).get_uri());
				_location = &(*l);
				break ;
			}
		}
	}
	if (!_location)
	{
		// ... or check if route exists for the directory
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
	// if (_request->get_method() & _location->get_methods())
}

void
Response::_set_content_type()
{
	std::string	_ext;

	_ext = _uri.substr(_uri.find_last_of(".") + 1);
	if (_ext == "bmp")
		_header.set_content_type(MIME_BMP);
	else if (_ext == "css")
		_header.set_content_type(MIME_CSS);
	else if (_ext == "gif")
		_header.set_content_type(MIME_GIF);
	else if (_ext == "htm")
		_header.set_content_type(MIME_HTM);
	else if (_ext == "html")
		_header.set_content_type(MIME_HTML);
	else if (_ext == "ico")
		_header.set_content_type(MIME_ICO);
	else if (_ext == "jpg")
		_header.set_content_type(MIME_JPG);
	else if (_ext == "jpeg")
		_header.set_content_type(MIME_JPEG);
	else if (_ext == "js")
		_header.set_content_type(MIME_JS);
	else if (_ext == "png")
		_header.set_content_type(MIME_PNG);
	else if (_ext == "pdf")
		_header.set_content_type(MIME_PDF);
	else if (_ext == "sh")
		_header.set_content_type(MIME_SH);
	else
		_header.set_content_type(MIME_PLAIN_TEXT);
}

void
Response::_get_body_from_uri(void)
{
	std::ifstream	f;
	std::string		line;

	f.open(_uri, std::ifstream::out);
	if (!f.is_open())
	{
		_header.set_status(STATUS_INTERNAL_SERVER_ERROR);
		_body = "501 Internal server error";
		return ;
	}
	_header.set_status(STATUS_OK);
	_set_content_type();
	while (std::getline(f, line))
		_body.append(line + "\n");
	f.close();
}

void
Response::_get_body(const std::map<uint16_t, std::string>& error_pages, const t_listen& listen)
{
	struct stat		_stat;
	Autoindex		_ai;

	if (stat(_uri.c_str(), &_stat) == 0)
	{
		if (S_ISREG(_stat.st_mode))
		{
			WS_VALUE_LOG("File found at", _uri);
			_get_body_from_uri();
		}
		else if (S_ISDIR(_stat.st_mode))
		{
			WS_VALUE_LOG("Directory found at", _uri);
			if (_location)
			{
				WS_VALUE_LOG("Location autoindex", _location->is_autoindex_on());
				WS_VALUE_LOG("Location default file", _location->get_default_file());
				if (_location->is_autoindex_on())
				{
					_body = _ai.get_index(_uri, listen);
					_header.set_content_type(MIME_HTML);
				}
				else if (_location->get_default_file().size())
				{
					_header.set_status(STATUS_FOUND);
					_header.set_location(_location->get_default_file());
				}
				else
				{
					_header.set_status(STATUS_FORBIDDEN);
					_uri = error_pages.at(STATUS_FORBIDDEN);
					_get_body_from_uri();
				}
				// redirections
				// for (std::map<std::string, std::string>::const_iterator it = _location->get_redirections().begin(); it != _location->get_redirections().end(); it++)
				// {
				// 	if (_uri == (*it).second)
				// 	{
				// 		return ;
				// 	}
				// }
			}
			else
			{
				_header.set_status(STATUS_FORBIDDEN);
				_uri = error_pages.at(STATUS_FORBIDDEN);
				_get_body_from_uri();
			}
		}
	}
	else
	{
		WS_VALUE_LOG("No such file or directory", _uri);
		_header.set_status(STATUS_NOT_FOUND);
		_uri = error_pages.at(STATUS_NOT_FOUND);
		_get_body_from_uri();
	}
	_header.set_content_length(_body.length());
}

void
Response::generate(const std::map<uint16_t, std::string>& error_pages,
                   const std::set<Location>& locations,
				   const t_listen& listen)
{
	const Location	*_old_loc;

	if (!_request->is_valid())
	{
		WS_INFO_LOG("Invalid request.");
		_header.set_status(STATUS_BAD_REQUEST);
		_uri = error_pages.at(STATUS_BAD_REQUEST);
		_get_body(error_pages, listen);
		return ;
	}
	WS_INFO_LOG("Valid request.");
	WS_INFO_LOG("Creating response.");

	for (int i = 0; i < 10; i++)
	{
		_old_loc = _location;
		_check_uri(locations);
		if (!_location)
		{
			_location = _old_loc;
			break ;
		}
		if (!_location->get_root().size())
			break ;
		if (_location)
		{
			WS_VALUE_LOG("Location root", _location->get_root());
			if (_location->get_root().size())
				_uri.insert(0, _location->get_root());
		}
	}
	_uri.insert(0, ".");
	_get_body(error_pages, listen);
}

const std::string
Response::str()
{
	std::string		str;
	unsigned int	_status;

	_status = atoi(_header.get_status().c_str());
	str = "HTTP/1.1 " + _header.get_status() + " " + _header.get_status_string() + CRLF;

	// 3xx status codes are redirections
	if (_status / 100 == 3)
		str += "Location: " + _header.get_location() + CRLF;
	else if (_status == STATUS_OK)
	{
		str += "Content-Length: " + _header.get_content_length() + CRLF;
		str += "Content-Type: " + _header.get_content_type() + CRLF;
	}
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
