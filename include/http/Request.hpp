/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 22:39:16 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/27 17:46:00 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "http.hpp"
# include <map>
# include <string>

class Request
{
private:
	unsigned int						_method;
	std::string							_uri;
	std::string							_http_version;
	std::string							_query_string;
	std::map<std::string, std::string>	_header_fields;
	std::string							_body;
	bool								_valid;

public:
	Request(const char *_buffer);
	~Request(void);

	const unsigned int&							get_method() const;
	const std::string&							get_uri() const;
	const std::string&							get_http_version() const;
	const std::string&							get_query_string() const;
	const std::map<std::string, std::string>&	get_header_fields() const;
	const std::string&							get_body() const;
	bool										is_valid() const;

	void	add_header_field(const std::string& field, const std::string& value);

	void	set_method(const unsigned int&);
	void	set_uri(const std::string&);
	void	set_http_version(const std::string&);
	void	set_query_string(const std::string&);
	void	set_header_fields(const std::map<std::string, std::string>&);
	void	set_body(const std::string&);
	void	set_valid();
	void	set_invalid();

	// setters
};

#endif // REQUEST_HPP
