/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 13:29:00 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/29 18:45:12 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "RequestParser.hpp"
#include "log.hpp"

Request::Request(const char *_buffer)
	: _method(0),
	  _uri(""),
	  _http_version(""),
	  _query_string(""),
	  _header_fields(),
	  _body(),
	  _valid(false)
{
	RequestParser	_parser(*this, _buffer);

	WS_INFO_LOG("Parsing request.");
	std::cout << "this is the request: " << _buffer << std::endl;
	_parser.run();
}

Request::~Request()
{}

const unsigned int&	Request::get_method() const
	{return _method;}
const std::string&	Request::get_uri() const
	{return _uri;}
const std::string&	Request::get_http_version() const
	{return _http_version;}
const std::string&	Request::get_query_string() const
	{return _query_string;}
const std::string&	Request::get_body() const
	{return _body;}
const std::map<std::string, std::string>&	Request::get_header_fields() const
	{return _header_fields;}
bool	Request::is_valid() const
	{return _valid;}

void	Request::add_header_field(const std::string& field, const std::string& value)
	{_header_fields.count(field) == 0 ? _header_fields[field] = value : _header_fields[field].append(", ").append(value);}

void	Request::set_method(const unsigned int& m)
	{_method = m;}
void	Request::set_uri(const std::string& uri)
	{_uri = uri;}
void	Request::set_http_version(const std::string& p)
	{_http_version = p;}
void	Request::set_query_string(const std::string& qs)
	{_query_string = qs;}
void	Request::set_header_fields(const std::map<std::string, std::string>& hf)
	{_header_fields = hf;}
void	Request::set_body(const std::string& body)
	{_body = body;}
void	Request::set_valid()
	{_valid = true;}
void	Request::set_invalid()
	{_valid = false;}
