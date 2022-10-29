/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHeaders.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 11:14:10 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/29 21:30:37 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseHeaders.hpp"

ResponseHeaders::ResponseHeaders()
	: _status(200),
	  _content_type(MIME_PLAIN_TEXT),
	  _content_length("0"),
	  _server("webserv"),
	  _location("")
{
}

ResponseHeaders::~ResponseHeaders()
{
}

void	ResponseHeaders::set_status(const std::string& status)
	{_status = std::atoi(status.c_str());}
void	ResponseHeaders::set_status(unsigned int status)
	{_status = status;}
void	ResponseHeaders::set_content_type(const std::string& type)
	{_content_type = type;}
void	ResponseHeaders::set_content_length(const std::string& length)
	{_content_length = length;}
void	ResponseHeaders::set_content_length(size_t length)
	{_content_length = to_string(length);}
void	ResponseHeaders::set_server(const std::string& server)
	{_server = server;}
void	ResponseHeaders::set_location(const std::string& location)
	{_location = location;}

const std::string	ResponseHeaders::get_status() const
	{return to_string(_status);}
const std::string&	ResponseHeaders::get_content_type() const
	{return _content_type;}
const std::string&	ResponseHeaders::get_content_length() const
	{return _content_length;}
const std::string&	ResponseHeaders::get_server() const
	{return _server;}
const std::string&	ResponseHeaders::get_location() const
	{return _location;}

const std::string	ResponseHeaders::get_date() const
{
	char			buffer[100];
	struct timeval	tv;
	struct tm		*tm;

	gettimeofday(&tv, NULL);
	tm = gmtime(&tv.tv_sec);
	strftime(buffer, 100, "%a, %d %b %Y %H:%M:%S GMT", tm);
	return (buffer);
}

const std::string
ResponseHeaders::get_status_string(void) const
{
	if (_status == STATUS_CREATED)
		return ("Created");
	if (_status == STATUS_ACCEPTED)
		return ("Accepted");
	if (_status == STATUS_NO_CONTENT)
		return ("No content");
	if (_status == STATUS_MOVED_PERMANENTLY)
		return ("Moved Permanently");
	if (_status == STATUS_FOUND)
		return ("Found");
	if (_status == STATUS_FOUND)
		return ("See other");
	if (_status == STATUS_BAD_REQUEST)
		return ("Bad Request");
	if (_status == STATUS_FORBIDDEN)
		return ("Forbidden");
	if (_status == STATUS_NOT_FOUND)
		return ("Not found");
	if (_status == STATUS_METHOD_NOT_ALLOWED)
		return ("Method not allowed");
	if (_status == STATUS_PAYLOAD_TOO_LARGE)
		return ("Paylod too large");
	if (_status == STATUS_INTERNAL_SERVER_ERROR)
		return ("Internal server error");
	if (_status == STATUS_NOT_IMPLEMENTED)
		return ("Not implemented");
	if (_status == STATUS_HTTP_VERSION_NOT_SUPPORTED)
		return ("HTTP version not supported");
	// or _status == STATUS_OK
	return ("OK");
}
