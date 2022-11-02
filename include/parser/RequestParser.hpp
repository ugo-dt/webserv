/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 22:49:07 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/11/01 14:09:33 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_PARSER_HPP
# define REQUEST_PARSER_HPP

# include "http.hpp"
# include "Request.hpp"
# include "string_utils.hpp"
# include <string>

class Request;

class RequestParser
{
private:
	Request&	_request; // request to fill
	std::string	_buffer;
	size_t		_status;

	void				_parse();
	void				_parse_first_line();
	const std::string	_get_next_line(size_t &i);

public:
	RequestParser(Request& request, const char *buffer);
	~RequestParser();

	void	run();
};

#endif // REQUEST_PARSER_HPP
