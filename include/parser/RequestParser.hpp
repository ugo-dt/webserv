/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 22:49:07 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/26 16:48:45 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_PARSER_HPP
# define REQUEST_PARSER_HPP

# include "parser.hpp"
# include <list>
# include <string>

class RequestParser
{
private:
	std::list<size_t>	_token_list;
	std::string			_request;

	// get request as string
	// _tokenize (from string)
	// _parse

public:
	RequestParser(const std::string& request);
	~RequestParser();
};

#endif // REQUEST_PARSER_HPP
