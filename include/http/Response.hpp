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
# include "mkdir_p.hpp"
# include "raw.hpp"
# include "string_utils.hpp"

# include "Autoindex.hpp"
# include "CGI.hpp"
# include "Location.hpp"
# include "Request.hpp"
# include "ResponseHeaders.hpp"

# include <cstdio>
# include <fstream>
# include <map>
# include <sstream>
# include <set>
# include <string>
# include <sys/stat.h>
# include <vector>

// HTTP/1.1 CODE
// Headers
// empty line
// Body
class Response
{
private:
	const Request&	_request;
	ResponseHeaders	_header;
	std::string		_uri;
	std::string		_body;
	const Location*	_location; // matching location (NULL if no match)
	std::string		_data;

	void				_check_uri(const std::set<Location>& server_locations);
	int					_check_redirections(void);
	void				_get_body(const std::map<u_int16_t, std::string>& error_pages, const t_listen& listen);
	void				_set_content_type();
	void				_get_body_from_uri(const std::map<u_int16_t, std::string>& error_pages);
	void				_handle_post(const std::map<u_int16_t, std::string>& error_pages);
	void				_handle_delete(const std::map<u_int16_t, std::string>& error_pages, const t_listen& listen);
	void				_parse_post_body(const std::map<u_int16_t, std::string>& error_pages, const std::string& _dir);
	bool				_is_cgi(void);
	int					_run_cgi_script(const std::string&);

public:
	Response(const Request& request);
	~Response(void);

	void				generate(const std::map<u_int16_t, std::string>& error_pages, const std::set<Location>& locations, const t_listen& listen);
	const std::string	str();
	size_t				length();
};

static inline
bool
is_directory(const std::string& path)
{
	struct stat	_stat;
	return (stat(path.c_str(), &_stat) == 0 && S_ISDIR(_stat.st_mode));
}

#endif // RESPONSE_HPP
