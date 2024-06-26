/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 12:08:06 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/11/02 12:21:45 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "http.hpp"

# include <string>
# include <map>

class Location
{
private:
	std::string							_uri;
	bool								_is_directory;
	unsigned int						_methods;
	std::string							_root;
	std::map<std::string, std::string>	_redirections;
	bool								_autoindex;
	std::string							_default_file;
	std::map<std::string, std::string>	_cgi_extensions;
	std::string							_upload_path;

	unsigned int						_state;

public:
	Location();
	Location(const Location& x);
	const Location&	operator=(const Location& x);
	~Location();

	const std::string&							get_uri() const;
	bool										is_directory() const;
	const unsigned int&							get_methods() const;
	const std::string&							get_root() const;
	const std::map<std::string, std::string>&	get_redirections() const;
	bool										is_autoindex_on() const;
	const std::string&							get_default_file() const;
	const std::map<std::string, std::string>&	get_cgi_extensions() const;
	const std::string&							get_upload_path() const;

	void	set_uri(const std::string&);
	void	set_as_directory(bool d);
	void	set_methods(const unsigned int&);
	void	set_root(const std::string&);
	void	set_redirections(const std::map<std::string, std::string>&);
	void	set_autoindex(bool);
	void	set_default_file(const std::string&);
	void	set_cgi_extensions(const std::map<std::string, std::string>&);
	void	set_upload_path(const std::string&);

	void	add_method(const unsigned int& method);
	void	add_redirection(const std::string& from, const std::string& to);
	void	add_cgi_extension(const std::string& ext, const std::string& bin);

	void		 set_state(unsigned int x);
	unsigned int get_state(void) const;
};

inline bool operator<(const Location& x, const Location& y)
	{return (x.get_uri() < y.get_uri());}

inline bool operator==(const Location& x, const Location& y)
	{return (x.get_uri() == y.get_uri());}

#endif // LOCATION_HPP
