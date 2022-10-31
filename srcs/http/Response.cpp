/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 13:29:07 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/31 18:43:13 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(const Request& request)
	: _request(request),
	  _header(),
	  _body(),
	  _location(0)
{
	_uri = _request.get_uri();
	if (is_directory("./" + _uri) && _uri[_uri.length() - 1] != '/')
		_uri.append(1, '/');
}

Response::~Response()
{
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
}

void
Response::_set_content_type()
{
	std::string	_ext;

	if (_uri.find('.') == std::string::npos)
	{
		_header.set_content_type(MIME_PLAIN_TEXT);
		return ;
	}
	_ext = _uri.substr(_uri.find_last_of("."));
	if (_ext == EXTENSION_BMP)
		_header.set_content_type(MIME_BMP);
	else if (_ext == EXTENSION_CSS)
		_header.set_content_type(MIME_CSS);
	else if (_ext == EXTENSION_GIF)
		_header.set_content_type(MIME_GIF);
	else if (_ext == EXTENSION_HTM)
		_header.set_content_type(MIME_HTM);
	else if (_ext == EXTENSION_HTML)
		_header.set_content_type(MIME_HTML);
	else if (_ext == EXTENSION_ICO)
		_header.set_content_type(MIME_ICO);
	else if (_ext == EXTENSION_JPG)
		_header.set_content_type(MIME_JPG);
	else if (_ext == EXTENSION_JPEG)
		_header.set_content_type(MIME_JPEG);
	else if (_ext == EXTENSION_JS)
		_header.set_content_type(MIME_JS);
	else if (_ext == EXTENSION_PNG)
		_header.set_content_type(MIME_PNG);
	else if (_ext == EXTENSION_PDF)
		_header.set_content_type(MIME_PDF);
	else if (_ext == EXTENSION_SH)
		_header.set_content_type(MIME_SH);
	else
		_header.set_content_type(MIME_PLAIN_TEXT);
}

void
Response::_get_body_from_uri(void)
{
	std::ifstream	f;
	std::string		line;

	f.open(_uri.c_str());
	if (!f.is_open())
	{
		_header.set_status(STATUS_INTERNAL_SERVER_ERROR);
		_body = "501 Internal server error";
		return ;
	}
	_header.set_status(STATUS_OK);
	_set_content_type();
	_body = get_body_from_uri(_uri);
}

void
Response::_get_body(const std::map<u_int16_t, std::string>& error_pages, const t_listen& listen)
{
	struct stat		_stat;
	Autoindex		_ai;

	if (stat(_uri.c_str(), &_stat) == 0)
	{
		if (S_ISREG(_stat.st_mode))
			_get_body_from_uri();
		else if (S_ISDIR(_stat.st_mode))
		{
			if (_location)
			{
				if (_location->is_autoindex_on())
				{
					_body = _ai.get_index(_uri, listen);
					_header.set_content_type(MIME_HTML);
				}
				else if (_location->get_default_file().size())
				{
					_header.set_status(STATUS_FOUND);
					_header.set_location(_location->get_default_file());
					_header.set_content_length("0");
					_body.clear();
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
Response::_parse_post_body()
{
	std::stringstream	sstream(_body);
	std::string			line;
	std::string			file_path;
	std::string			_dir;

	return ;
	while (std::getline(sstream, line))
	{
		std::cout << "line: " << line << std::endl;
	}
	std::cout << "post dir:" << _dir << std::endl;
	std::cout << "post file path:" << file_path << std::endl;
}

void
Response::_handle_post(const std::map<u_int16_t, std::string>& error_pages, const t_listen& listen)
{
	std::ofstream	file;
	std::string		file_path;
	std::string		_dir;

	(void)error_pages;
	(void)listen;
	std::cout << _request.get_body() << std::endl;
}

void
Response::generate(const std::map<u_int16_t, std::string>& error_pages,
                   const std::set<Location>& locations,
				   const t_listen& listen)
{
	const Location	*_old_loc;

	if (!_request.is_valid())
	{
		_header.set_status(STATUS_BAD_REQUEST);
		_uri = error_pages.at(STATUS_BAD_REQUEST);
		_get_body(error_pages, listen);
		return ;
	}
	for (int i = 0; i < 10; i++)
	{
		_old_loc = _location;
		_check_uri(locations);
		if (!_location)
		{
			_location = _old_loc;
			break ;
		}
		if (_location->get_root().empty())
			break ;
		if (_location)
		{
			if (!_location->get_root().empty())
				_uri.insert(0, _location->get_root());
		}
		WS_INFO_LOG("Root is '" + _location->get_root() + "'");
		WS_INFO_LOG("Searching for new location...");
	}
	// for (std::map<std::string, std::string>::const_iterator it = _request.get_header_fields().begin(); it != _request.get_header_fields().end(); it++)
	// 	std::cout << (*it).first << ", " << (*it).second << std::endl;
	_uri.insert(0, 1, '.');
	if (_request.get_method() == METHOD_GET)
		_get_body(error_pages, listen);
	else if (_request.get_method() == METHOD_POST)
	{
		_handle_post(error_pages, listen);
	}
}

const std::string
Response::str()
{
	std::string		str;
	unsigned int	_status;

	_status = atoi(_header.get_status().c_str());
	str = "HTTP/1.1 " + _header.get_status() + " " + get_status_string(_status) + CRLF;

	// 3xx status codes are redirections
	if (_status / 100 == 3 || _status == STATUS_SEE_OTHER)
	{
		str += "Location: " + _header.get_location() + CRLF;
		str += "Content-Length: 0" CRLF;
	}
	else if (_status == STATUS_OK)
	{
		str += "Content-Length: " + _header.get_content_length() + CRLF;
		str += "Content-Type: " + _header.get_content_type() + CRLF;
	}
	str += "Date: " + _header.get_date() + CRLF;
	str += "Server: " + _header.get_server() + CRLF;
	str += CRLF;
	str += _body;
	// std::cout << str << std::endl;
	return (str);
}
