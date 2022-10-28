/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 13:29:07 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/28 17:11:14 by ugdaniel         ###   ########.fr       */
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
	}
}

Response::~Response()
{
	if (_request)
		delete (_request);
}

void
Response::_check_for_location()
{
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
}

void
Response::generate()
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

	_check_for_location();
	_check_
	_body = "this is the body\n";

	// _response = "HTTP/1.1 " + _header.get_status() + " " + _header.get_status_string() + CRLF;
	// _response += "Content-Length: " + to_string(_body.length()) + CRLF;
	// _response += CRLF; // separates header fields and body
	// _response += _body + CRLF;
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
