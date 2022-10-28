/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 13:29:07 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/28 11:48:18 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(const char *request_buffer)
	: _header(),
	  _request(0),
	  _body(),
	  _response("")
{
	if (!request_buffer)
	{
		WS_INFO_LOG("Bad request.");
		_response = "HTTP/1.1 400 Bad Request\r\n"
					"content-length: 17\r\n"
					"\r\n"
					"400 Bad request\r\n";
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
Response::generate()
{
	if (!_request->is_valid())
	{
		WS_INFO_LOG("Bad request.");
		_response = "HTTP/1.1 400 Bad Request\r\n"
				"content-length: 17\r\n"
				"\r\n"
				"400 Bad request\r\n";
		return ;
	}
	WS_INFO_LOG("Valid request.");
	WS_INFO_LOG("Creating response.");

	// Path
	// location
	// Get body
	_body = "this is the body\n";

	_response = "HTTP/1.1 " + _header.get_status() + " " + _header.get_status_string() + CRLF;
	_response += "Content-Length: " + to_string(_body.length()) + CRLF;
	_response += CRLF; // separates header fields and body
	_response += _body + CRLF;
}

const char *
Response::str()
{
	return (_response.c_str());
}

size_t
Response::length()
{
	return (_response.length());
}
