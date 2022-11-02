/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 13:29:00 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/11/02 12:05:13 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "RequestParser.hpp"
#include "log.hpp"

Request::Request()
	: _method(0),
	  _uri(""),
	  _http_version(""),
	  _query_string(""),
	  _header_fields(),
	  _body(),
	  _valid(false),
	  _post_boundary("")
{
}

Request::Request(const char *_buffer)
	: _method(0),
	  _uri(""),
	  _http_version(""),
	  _query_string(""),
	  _header_fields(),
	  _body(),
	  _valid(false),
	  _post_boundary("")
{
	parse(_buffer);
}

void
Request::parse(const char *_buffer)
{
	RequestParser	_parser(*this, _buffer);

	_parser.run();
#ifdef WS_LOG
	std::cout << "[\033[92m" << _buffer << "\033[0m]" << std::endl;
#endif
}

void
Request::clear(void)
{
	_method = 0;
	_uri.clear();
	_http_version.clear();
	_query_string.clear();
	_header_fields.clear();
	_body.clear();
	_valid = false;
	_post_boundary.clear();
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
const std::string&	Request::get_post_boundary() const
	{return _post_boundary;}

void	Request::add_header_field(const std::string& field, const std::string& value)
	{_header_fields.count(field) == 0 ? _header_fields[field] = value : _header_fields[field].append(", ").append(value);}
void	Request::append_to_body(const std::string& body)
	{_body.append(body);}

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
void	Request::set_post_boundary(const std::string& boundary)
	{_post_boundary = boundary;}
