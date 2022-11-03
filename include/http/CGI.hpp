/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 15:05:38 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/11/03 11:53:08 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "core.hpp"
#include "http.hpp"
#include "log.hpp"

#include "Location.hpp"
#include "Request.hpp"
#include "string_utils.hpp"

#include <cstdlib>
#include <cstring>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

#define ENV_VARIABLES	6
#define ARGS_VARIABLES	2

class CGI
{
private:
	Request			_req;
	std::string		_extension;
	std::string		_executable;
	char			**_args;
	char			**_env;
	std::string		_body;
	size_t			_body_size;

	void		_add_arg(const std::string& arg);
	void		_add_env(const std::string& name, const std::string& value);
	void		_clean();

public:
	CGI(const Request& req, const std::string& ext, const std::string& exec);
	~CGI();

	void				init();
	void				run();
	const std::string&	get_body();
	size_t				get_body_size();
};
