/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHeaders.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 11:14:10 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/28 16:44:10 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseHeaders.hpp"

ResponseHeaders::ResponseHeaders()
	: _status(200),
	  _content_type(MIME_PLAIN_TEXT),
	  _content_length("0"),
	  _server("webserv")
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

const std::string	ResponseHeaders::get_status() const
	{return to_string(_status);}
const std::string&	ResponseHeaders::get_content_type() const
	{return _content_type;}
const std::string&	ResponseHeaders::get_content_length() const
	{return _content_length;}
const std::string&	ResponseHeaders::get_server() const
	{return _server;}

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
	if (_status == 301)
		return ("Moved Permanently");
	if (_status == 201)
		return ("Created");
	if (_status == 202)
		return ("Accepted");
	if (_status == 204)
		return ("No content");
	if (_status == 400)
		return ("Bad Request");
	if (_status == 403)
		return ("Forbidden");
	if (_status == 404)
		return ("Not found");
	if (_status == 405)
		return ("Method not allowed");
	if (_status == 413)
		return ("Paylod too large");
	if (_status == 500)
		return ("Internal server error");
	if (_status == 501)
		return ("Not implemented");
	if (_status == 505)
		return ("HTTP version not supported");
	// if (_status == 200)
	return ("OK");
}