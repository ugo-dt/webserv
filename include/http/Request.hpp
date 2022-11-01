/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 22:39:16 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/11/01 14:10:15 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "http.hpp"
# include "string_utils.hpp"

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
	std::string							_post_boundary;

public:
	Request();
	Request(const char *_buffer);
	~Request(void);
	
	void				parse(const char *_buffer);
	const std::string	get_post_header_value(const std::string& header, const std::string& arg) const;

	const unsigned int&							get_method() const;
	const std::string&							get_uri() const;
	const std::string&							get_http_version() const;
	const std::string&							get_query_string() const;
	const std::map<std::string, std::string>&	get_header_fields() const;
	const std::string&							get_body() const;
	bool										is_valid() const;
	const std::string&							get_post_boundary() const;

	void	add_header_field(const std::string& field, const std::string& value);
	void	append_to_body(const std::string&);

	void	set_method(const unsigned int&);
	void	set_uri(const std::string&);
	void	set_http_version(const std::string&);
	void	set_query_string(const std::string&);
	void	set_header_fields(const std::map<std::string, std::string>&);
	void	set_body(const std::string&);
	void	set_valid();
	void	set_invalid();
	void	set_post_boundary(const std::string& boundary);

	// setters
};

#endif // REQUEST_HPP
