/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 22:39:09 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/28 14:57:06 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "Autoindex.hpp"
# include "core.hpp"
# include "http.hpp"
# include "log.hpp"
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
	ResponseHeaders	_header;
	Request			*_request;
	std::string		_body;
	std::string		_response;
	std::string		_content_type;

public:
	Response(const char *request_buffer);
	~Response(void);

	void		generate();
	const char	*str();
	size_t		length();
};

#endif // RESPONSE_HPP
