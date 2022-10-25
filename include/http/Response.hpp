/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 22:39:09 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/25 20:59:34 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "http.hpp"
# include <string>
# include <vector>

// HTTP/1.1 CODE
// Headers
// empty line
// Body
class Response
{
private:
	size_t						_status;
	std::vector<std::string>	_header_fields;
	std::string					_body;

public:
	Response(void);
	~Response(void);
};

#endif // RESPONSE_HPP
