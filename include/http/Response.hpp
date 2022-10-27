/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 22:39:09 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/27 16:57:14 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "Request.hpp"
# include "http.hpp"
# include "log.hpp"
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
	Request						*_request;
	size_t						_status;
	std::vector<std::string>	_header_fields;
	std::string					_body;
	std::string					_response;

public:
	Response(const char *request_buffer);
	~Response(void);

	const char	*str();
	size_t		length();
};

static inline
const std::string
get_status_string(size_t status)
{
	if (status == 301)
		return ("Moved Permanently");
	if (status == 201)
		return ("Created");
	if (status == 202)
		return ("Accepted");
	if (status == 204)
		return ("No content");
	if (status == 400)
		return ("Bad Request");
	if (status == 403)
		return ("Forbidden");
	if (status == 404)
		return ("Not found");
	if (status == 405)
		return ("Method not allowed");
	if (status == 413)
		return ("Paylod too large");
	if (status == 500)
		return ("Internal server error");
	if (status == 501)
		return ("Not implemented");
	if (status == 505)
		return ("HTTP version not supported");
	// if (status == 200)
	return ("OK");
}

#endif // RESPONSE_HPP
