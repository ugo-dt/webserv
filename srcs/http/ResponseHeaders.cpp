/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHeaders.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 11:14:10 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/30 15:14:34 by ugdaniel         ###   ########.fr       */
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
