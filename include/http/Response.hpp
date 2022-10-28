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

# include "Autoindex.hpp"
# include "core.hpp"
# include "http.hpp"
# include "log.hpp"
# include "Location.hpp"
# include "Request.hpp"
# include "ResponseHeaders.hpp"
# include "string_utils.hpp"
# include <string>
# include <vector>

// HTTP/1.1 CODE
// Headers
// empty line
// Body
class Response
{
private:
	Request			*_request;
	ResponseHeaders	_header;
	std::string		_body;
	size_t			_response_length;
	const Location	*_location;

public:
	Response(const char *request_buffer);
	~Response(void);

	void				generate();
	const std::string	str();
	size_t				length();
};

#endif // RESPONSE_HPP
