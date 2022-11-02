/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 13:29:07 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/11/02 16:59:20 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(const Request& request)
	: _request(request),
	  _header(),
	  _uri(""),
	  _body(),
	  _location(0),
	  _data("")
{
	_uri = _request.get_uri();
	if (is_directory("./" + _uri) && _uri[_uri.length() - 1] != '/')
		_uri.append(1, '/');
}

Response::~Response()
{
}

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
				WS_INFO_LOG("Found matching location for file '" << (*l).get_uri() << "'");
				_location = &(*l);
				break ;
			}
		}
	}
	// ... or check if route exists for the parent directory ...
	while (!_location && !_dir.empty())
	{
		for (std::set<Location>::const_iterator l = locations.begin(); l != locations.end(); l++)
		{
			if (_dir == (*l).get_uri())
			{
				WS_INFO_LOG("Found matching location for directory '" << (*l).get_uri() << "'");
				_location = &(*l);
				break ;
			}
		}
		// ... and it's parents as well
		_dir.assign(_dir, 0, _dir.length() - 1);
		_dir.assign(_dir, 0, _dir.find_last_of('/') + 1);
	}
}

int
Response::_check_redirections(void)
{
	if (!_location)
		return (0);
	if (_location->get_redirections().count(_uri))
	{
		WS_INFO_LOG("Found redirection: " << _location->get_redirections().at(_uri));
		_header.set_status(STATUS_FOUND);
		_header.set_location(_location->get_redirections().at(_uri));
		_header.set_content_length("0");
		_body.clear();
		return (1);
	}
	return (0);
}

void
Response::_set_content_type(void)
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
Response::_get_body_from_uri(const std::map<u_int16_t, std::string>& error_pages)
{
	std::ifstream	f;
	std::string		line;

	f.open(_uri.c_str(), std::ifstream::in | std::ifstream::binary);
	if (!f.is_open())
	{
		_header.set_status(STATUS_NOT_FOUND);
		_body = get_body_from_uri(error_pages.at(STATUS_NOT_FOUND));
		return ;
	}
	_body = get_file_contents(f);
	_header.set_status(STATUS_OK);
	_set_content_type();
}

void
Response::_get_body(const std::map<u_int16_t, std::string>& error_pages, const t_listen& listen)
{
	struct stat		_stat;
	Autoindex		_ai;

	if (stat(_uri.c_str(), &_stat) == 0)
	{
		if (S_ISREG(_stat.st_mode))
			_get_body_from_uri(error_pages);
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
					WS_INFO_LOG(_uri << ": default_file found (302), redirecting to '" << _location->get_default_file() << "'");
					_header.set_status(STATUS_FOUND);
					_header.set_location(_location->get_default_file());
					_header.set_content_length("0");
					_body.clear();
				}
				else
				{
					WS_WARN_LOG(_uri << ": forbidden (403): autoindex off + no default file");
					_header.set_status(STATUS_FORBIDDEN);
					_uri = error_pages.at(STATUS_FORBIDDEN);
					_get_body_from_uri(error_pages);
				}
			}
			else
			{
				WS_WARN_LOG(_uri << ": forbidden (403): no location for this path");
				_header.set_status(STATUS_FORBIDDEN);
				_uri = error_pages.at(STATUS_FORBIDDEN);
				_get_body_from_uri(error_pages);
			}
		}
	}
	else
	{
		WS_WARN_LOG(_uri << ": no such file or directory (404)");
		_header.set_status(STATUS_NOT_FOUND);
		_uri = error_pages.at(STATUS_NOT_FOUND);
		_get_body_from_uri(error_pages);
	}
	_header.set_content_length(_body.length());
}

void
Response::_parse_post_body(const std::map<u_int16_t, std::string>& error_pages, const std::string& _dir)
{
	std::ofstream						file;
	std::istringstream					_iss(_request.get_body());
	std::string							_line;
	std::string							file_path;
	const std::string					_boundary_end = "--" + _request.get_post_boundary() + "--\r\n";
	std::map<std::string, std::string>	_headers;
	std::vector<std::string>			_file_content;
	std::string							field;
	std::string							value;

	std::getline(_iss, _line); // first boundary
	while (std::getline(_iss, _line) && _line != "" && _line != "\r" && _line != "\n" && _line != CRLF)
	{
		field = get_header_field(_line);
		value = get_header_value(_line);
		_headers.insert(std::make_pair(field, value));
	}
	while (std::getline(_iss, _line))
	{
		_line.append(1, '\n');
		if (_line == _boundary_end)
			break ;
		_file_content.push_back(_line);
	}
	if (!_file_content.empty())
		_file_content.pop_back();
	file_path = get_post_header_value(_headers.at("Content-Disposition"), " filename=");
	file_path.assign(file_path, 1, file_path.find_last_of('"') - 1);

	if (mkdir_p(_dir.c_str()) == EXIT_FAILURE)
	{
		_header.set_status(STATUS_FORBIDDEN);
		_uri = error_pages.at(STATUS_FORBIDDEN);
		_get_body_from_uri(error_pages);
		return ;
	}
	file_path.insert(0, _dir);
	file.open(file_path.c_str(), std::ifstream::out | std::ofstream::binary);
	for (std::vector<std::string>::const_iterator it = _file_content.begin(); it != _file_content.end(); it++)
		file << (*it);
	_header.set_status(STATUS_CREATED);
	_header.set_location(file_path);
	_header.set_content_length(0);
}

void
Response::_handle_post(const std::map<u_int16_t, std::string>& error_pages)
{
	std::string	_dir;

	if (_request.get_post_boundary().empty())
	{
		_header.set_status(STATUS_BAD_REQUEST);
		_uri = error_pages.at(STATUS_BAD_REQUEST);
		_get_body_from_uri(error_pages);
		return ;
	}
	if (_location)
	{
		if (!_location->get_upload_path().empty())
			_dir = _location->get_upload_path();
	}
	if (_dir.empty())
		_dir = _request.get_uri();
	if (_dir[0] == '/')
		_dir.insert(0, 1, '.');
	_parse_post_body(error_pages, _dir);
	_header.set_status(STATUS_FOUND);
	_header.set_location("/");
	_header.set_content_length("0");
	_body.clear();
}

void
Response::_handle_delete(const std::map<u_int16_t, std::string>& error_pages, const t_listen& listen)
{
	if (std::remove(_uri.c_str()) != 0)
	{
		if (errno == EACCES)
		{
			_header.set_status(STATUS_FORBIDDEN);
			_uri = error_pages.at(STATUS_FORBIDDEN);
			_get_body(error_pages, listen);
			return ;
		}
		if (errno == ENOENT)
		{
			_header.set_status(STATUS_NO_CONTENT);
			_header.set_content_length("0");
			_body.clear();
			return ;
		}
	}
	_header.set_status(STATUS_OK);
	_header.set_content_length("0");
	_body.clear();
}

bool
Response::_is_cgi(void)
{
	std::string	_ext;

	if (!_location || _uri.find('.') == std::string::npos)
		return (false);
	_ext = _uri.substr(_uri.find_last_of("."));
	if (_ext.empty())
		return (false);
	return (_location->get_cgi_extensions().count(_ext));
}

int
Response::_run_cgi_script(const std::string& ext)
{
	CGI	cgi(_request, ext, _location->get_cgi_extensions().at(ext));

	try
	{
		cgi.init();
		_body = cgi.run();
	}
	catch (std::exception& e)
	{
		WS_ERROR_LOG("could not run cgi: " << e.what());
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
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
		WS_INFO_LOG("Root is '" << _location->get_root() << "'");
		WS_INFO_LOG("Searching for new location...");
	}
	if (_location)
	{
		if (!(_request.get_method() & _location->get_methods()))
		{
			_header.set_status(STATUS_METHOD_NOT_ALLOWED);
			_uri = error_pages.at(STATUS_METHOD_NOT_ALLOWED);
			_get_body(error_pages, listen);
			return ;
		}
		if (_location->get_redirections().size() && _check_redirections() == 1)
			return ;
		if (_is_cgi())
		{
			if (_run_cgi_script(_uri.substr(_uri.find_last_of("."))) != EXIT_SUCCESS)
			{
				_header.set_status(STATUS_INTERNAL_SERVER_ERROR);
				_uri = error_pages.at(STATUS_INTERNAL_SERVER_ERROR);
				_get_body_from_uri(error_pages);
			}
		}
	}
	_uri.insert(0, 1, '.');
	if (_request.get_method() & (METHOD_GET | METHOD_HEAD))
		_get_body(error_pages, listen);
	else if (_request.get_method() & METHOD_POST)
		_handle_post(error_pages);
	else if (_request.get_method() & METHOD_DELETE)
		_handle_delete(error_pages, listen);
}

const std::string
Response::str(void)
{
	_data = "HTTP/1.1 " + to_string(_header.get_status()) + " " + get_status_string(_header.get_status()) + CRLF;
	// 3xx status codes are redirections
	if (_header.get_status() / 100 == 3 || _header.get_status() == STATUS_CREATED)
	{
		_data += "Location: " + _header.get_location() + CRLF;
		_data += "Content-Length: 0" CRLF;
	}
	else if (_request.get_method() & METHOD_HEAD)
	{
		_data += "Content-Length: 0" CRLF;
	}
	else if (_header.get_status() == STATUS_OK)
	{
		if (!_header.get_content_length().empty())
			_data += "Content-Length: " + _header.get_content_length() + CRLF;
		_data += "Content-Type: " + _header.get_content_type() + CRLF;
	}
	_data += "Date: " + _header.get_date() + CRLF;
	_data += "Server: " + _header.get_server() + CRLF;
	_data += CRLF;
	if (!(_request.get_method() & METHOD_HEAD))
		_data += _body;
	return (_data);
}

size_t
Response::length(void)
{
	return (_data.length());
}
