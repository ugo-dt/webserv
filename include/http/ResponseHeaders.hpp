/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHeaders.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 11:14:45 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/28 11:38:38 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HEADERS_HPP
# define RESPONSE_HEADERS_HPP

/*
 * Response header fields
 */

# include "http.hpp"
# include "string_utils.hpp"
# include <string>
# include <sys/time.h>

class ResponseHeaders
{
private:
	unsigned int	_status;
	std::string		_content_type;
	std::string		_content_length;
	std::string		_server;

public:
	ResponseHeaders();
	~ResponseHeaders();

	void	set_status(unsigned int status);
	void	set_status(const std::string& status);
	void	set_content_type(const std::string&);
	void	set_content_length(const std::string&);
	void	set_server(const std::string&);

	const std::string	get_status() const;
	const std::string	get_date() const;
	const std::string&	get_content_type() const;
	const std::string&	get_content_length() const;
	const std::string&	get_server() const;
	const std::string	get_status_string() const;
};

#endif // RESPONSE_HEADERS_HPP
