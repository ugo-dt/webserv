/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 22:39:16 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/26 12:40:10 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "http.hpp"
# include <string>
# include <vector>

class Request
{
private:
	unsigned int				_method;
	std::string					_uri;
	std::string					_http_protocol;
	std::string					_query_string;
	std::vector<std::string>	_header_fields;
	std::string					_body;

public:
	Request(void);
	~Request(void);

	// getters
	// setters
};

#endif // REQUEST_HPP
