	/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 22:39:09 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/28 16:23:53 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "core.hpp"
# include "http.hpp"
# include "log.hpp"
# include "string_utils.hpp"

# include "Autoindex.hpp"
# include "Location.hpp"
# include "Request.hpp"
# include "ResponseHeaders.hpp"

# include <fstream>
# include <set>
# include <string>
# include <sys/stat.h>

// HTTP/1.1 CODE
// Headers
// empty line
// Body
class Response
{
private:
	Request			*_request;
	std::string		_uri;
	ResponseHeaders	_header;
	std::string		_body;
	size_t			_response_length;
	const Location	*_location; // matching location (set to NULL if no match)

	void	_check_uri(const std::set<Location>& server_locations);
	void	_get_body(const t_listen& listen);

public:
	Response(const char *request_buffer);
	~Response(void);

	void				generate(const std::map<uint16_t, std::string>& error_pages, const std::set<Location>& locations, const t_listen& listen);
	const std::string	str();
	size_t				length();
};

#endif // RESPONSE_HPP
