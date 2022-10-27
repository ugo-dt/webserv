/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 13:29:07 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/27 17:46:45 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(const char *request_buffer)
	: _request(0),
	  _status(200),
	  _header_fields(),
	  _body(),
	  _response()
{
	if (!request_buffer)
	{
		// send bad request
		WS_INFO_LOG("bad request.");
		_response = "Bad request.\n";
	}
	else
	{
		_request = new Request(request_buffer);
		
		if (!_request->is_valid())
		{
			WS_INFO_LOG("bad request.");
			_response = "Bad request.\n";
			return ;
		}

		WS_INFO_LOG("valid request.");
		_response.append(HTTP_PROTOCOL).append(" ").append(to_string(_status)).append(get_status_string(_status));
		_response = "This is the response.\n";
	}
}

Response::~Response()
{
	if (_request)
		delete (_request);
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
