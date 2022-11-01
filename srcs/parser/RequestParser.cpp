/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 22:50:14 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/11/01 18:21:29 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestParser.hpp"
#include <iostream>

RequestParser::RequestParser(Request& request, const char *buffer)
	: _request(request),
	  _buffer(buffer)
{	
}

RequestParser::~RequestParser(void)
{	
}

void
RequestParser::_parse_first_line()
{
	size_t		i;
	size_t		j;
	std::string	line;
	std::string	tmp;

	i = _buffer.find_first_of('\n');
	line = _buffer.substr(0, i);
	i = line.find_first_of(' ');

	if (i == std::string::npos)
	{
		_status = STATUS_BAD_REQUEST;
		return ;
	}

	tmp = std::string(line, 0, i);
	if (tmp == "GET")
		_request.set_method(METHOD_GET);
	else if (tmp == "HEAD")
		_request.set_method(METHOD_HEAD);
	else if (tmp == "POST")
		_request.set_method(METHOD_POST);
	else if (tmp == "DELETE")
		_request.set_method(METHOD_DELETE);
	else
		_request.set_method(METHOD_UNKNOWN);
	if ((j = line.find_first_not_of(' ', i)) == std::string::npos)
		_status = STATUS_BAD_REQUEST;
	if ((i = line.find_first_of(' ', j)) == std::string::npos)
		_status = STATUS_BAD_REQUEST;
	if (_status == STATUS_BAD_REQUEST)
		return ;
	tmp.assign(line, j, i - j);
	if (i > j)
		_request.set_uri(std::string(tmp, 0, tmp.find_first_of('?'))); // remove query string
	else
		_request.set_uri("");
	j = tmp.find_first_of('?');
	if (j != std::string::npos && j + 1 < line.length())
		_request.set_query_string(std::string(tmp, tmp.find_first_of('?') + 1, tmp.length()));
	else
		_request.set_query_string("");
	if ((i = line.find_first_not_of(' ', i)) == std::string::npos)
		_status = STATUS_BAD_REQUEST;

	if (line[i] == 'H' && line[i + 1] == 'T' && line[i + 2] == 'T' && line[i + 3] == 'P' && line[i + 4] == '/')
		_request.set_http_version(std::string(line, i + 5, 3));
	else
	{
		_status = STATUS_BAD_REQUEST;
		return ;
	}
	if (_request.get_http_version() != "1.0" && _request.get_http_version() != "1.1")
		_status = STATUS_HTTP_VERSION_NOT_SUPPORTED;
}

const std::string
RequestParser::_get_next_line(size_t &i)
{
	std::string		ret;
	size_t			j;

	if (i == std::string::npos)
		return "";
	j = _buffer.find_first_of('\n', i);
	ret = _buffer.substr(i, j - i);
	if (ret[ret.size() - 1] == '\r')
	{
		if (!ret.empty())
			ret.resize(ret.size() - 1);
	}
	i = (j == std::string::npos ? j : j + 1);
	return ret;
}

void
RequestParser::run()
{
	size_t		i;
	std::string	field;
	std::string	value;
	std::string line;

	_request.set_invalid();
	_parse_first_line();
	if (_status == STATUS_BAD_REQUEST)
		return ;
	i = 0;
	while (i < _buffer.length() && _buffer[i] != CR && _buffer[i] != LF)
		i++;
	while (i < _buffer.length() && (_buffer[i] == CR || _buffer[i] == LF))
		i++;
	while ((line = _get_next_line(i)) != "\r" && line != "" && line != "\r\n")
	{
		field = get_header_field(line);
		value = get_header_value(line);
		if (field.length() && value.length())
			_request.add_header_field(field, value);
	}
	if (i < _buffer.length())
		_request.set_body(std::string(_buffer, i, _buffer.length()));
	else
		_request.set_body("");
	_request.set_valid();
}
