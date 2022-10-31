/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raw.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 11:56:25 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/31 17:21:29 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raw.hpp"

const std::string	get_body_from_uri(const std::string& uri)
{
	std::string		line;
	std::string		result;
	std::ifstream	f;

	f.open(uri.c_str());
	if (!f.is_open())
		return ("");
	while (std::getline(f, line))
		result.append(line + "\n");
	return (result);
}

const std::string	get_raw_page(unsigned int code, const char *msg)
{
	std::string	s;

	s = "<!DOCTYPE html>\n"
			"<html>\n"
				"\t<title>";
	s.append(to_string(code));
	s.append(" ");
	s.append(msg);
	s.append("</title>\n"
				"\t<body>\n"
					"\t\t<div>\n"
						"\t\t\t<H1>");
	s.append(to_string(code));
	s.append(" ");
	s.append(msg);
	s.append("</H1>\n"
					"\t\t</div>\n"
				"\t</body>\n"
			"</html>\n");
	return s;
}

const std::string	get_status_string(unsigned int code)
{
	if (code == STATUS_CREATED)
		return ("Created");
	if (code == STATUS_ACCEPTED)
		return ("Accepted");
	if (code == STATUS_NO_CONTENT)
		return ("No Content");
	if (code == STATUS_MOVED_PERMANENTLY)
		return ("Moved Permanently");
	if (code == STATUS_FOUND)
		return ("Found");
	if (code == STATUS_FOUND)
		return ("See Other");
	if (code == STATUS_BAD_REQUEST)
		return ("Bad Request");
	if (code == STATUS_FORBIDDEN)
		return ("Forbidden");
	if (code == STATUS_NOT_FOUND)
		return ("Not Found");
	if (code == STATUS_METHOD_NOT_ALLOWED)
		return ("Method Not Allowed");
	if (code == STATUS_PAYLOAD_TOO_LARGE)
		return ("Paylod Too Large");
	if (code == STATUS_INTERNAL_SERVER_ERROR)
		return ("Internal Server Error");
	if (code == STATUS_NOT_IMPLEMENTED)
		return ("Not Implemented");
	if (code == STATUS_SERVICE_UNAVAILABLE)
		return ("Service Unavailable");
	if (code == STATUS_HTTP_VERSION_NOT_SUPPORTED)
		return ("HTTP Version Not Supported");
	// or code == STATUS_OK
	return ("OK");
}

const std::string	get_raw_page(unsigned int code)
{
	return (get_raw_page(code, get_status_string(code).c_str()));
}
