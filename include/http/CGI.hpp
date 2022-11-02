/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 15:05:38 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/11/02 17:00:12 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "core.hpp"

#include "Location.hpp"
#include "Request.hpp"
#include "string_utils.hpp"

#include <sys/wait.h>
#include <unistd.h>
#include <string>

class CGI
{
private:
	Request			_req;
	std::string		_extension;
	std::string		_executable;
	char			**_args;
	char			**_env;

	std::string		_get_cgi_name();

public:
	CGI(const Request& req, const std::string& ext, const std::string& exec);
	~CGI();

	void				init();
	const std::string	run();
};
