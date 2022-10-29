/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 12:09:22 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/29 12:22:29 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location()
	: _uri(""),
	  _is_directory(false),
	  _methods(ALL_METHODS),
	  _root(""),
	  _redirections(),
	  _autoindex(false),
	  _default_file(),
	  _cgi_extension(),
	  _upload_path("/upload"),
	  _state(0)
{
}

Location::Location(const Location& x)
	: _uri(x._uri),
	  _is_directory(x._is_directory),
	  _methods(x._methods),
	  _root(x._root),
	  _redirections(x._redirections),
	  _autoindex(x._autoindex),
	  _default_file(x._default_file),
	  _cgi_extension(x._cgi_extension),
	  _upload_path(x._upload_path),
	  _state(x._state)
{
}

const Location&
Location::operator=(const Location& x)
{
	if (this != &x)
	{
		_uri = x._uri;
		_is_directory = x._is_directory;
		_methods = x._methods;
		_root = x._root;
		_redirections = x._redirections;
		_autoindex = x._autoindex;
		_default_file = x._default_file;
		_cgi_extension = x._cgi_extension;
		_upload_path = x._upload_path;
	}
	return (*this);
}

Location::~Location()
{
}

const std::string&	Location::get_uri() const
	{return _uri;}
bool	Location::is_directory() const
	{return _is_directory;}
const unsigned int&	Location::get_methods() const
	{return _methods;}
const std::string&	Location::get_root() const
	{return _root;}
const std::map<std::string, std::string>& Location::get_redirections() const
	{return _redirections;}
bool	Location::is_autoindex_on() const
	{return _autoindex;}
const std::string&	Location::get_default_file() const
	{return _default_file;}
const std::string&	Location::get_cgi_extension() const
	{return _cgi_extension;}
const std::string&	Location::get_upload_path() const
	{return _upload_path;}

void	Location::set_uri(const std::string& uri)
	{_uri = uri;}
void	Location::set_as_directory(bool d)
	{_is_directory = d;}
void	Location::set_methods(const unsigned int& methods)
	{_methods = methods;}
void	Location::set_root(const std::string& root)
	{_root = root;}
void	Location::set_redirections(const std::map<std::string, std::string>& r)
	{_redirections = r;}
void	Location::set_autoindex(bool a)
	{_autoindex = a;}
void	Location::set_default_file(const std::string& path)
	{_default_file = path;}
void	Location::set_cgi_extension(const std::string& ext)
	{_cgi_extension = ext;}
void	Location::set_upload_path(const std::string& path)
	{_upload_path = path;}

void	Location::add_method(const unsigned int& method)
	{_methods |= method;}
void	Location::add_redirection(const std::string& from, const std::string& to)
	{_redirections.insert(std::make_pair(from, to));}

void Location::set_state(unsigned int x)
	{_state |= x;}
unsigned int Location::get_state(void) const
	{return _state;}
