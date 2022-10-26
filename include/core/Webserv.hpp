/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madaniel <madaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 12:37:41 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/26 14:59:46 by madaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include "core.hpp"
# include "http.hpp"
# include "Parser.hpp"
# include "Server.hpp"
# include <cstdlib>
# include <map>
# include <poll.h>
# include <sys/stat.h>
# include <vector>

class Webserv
{
private:
	std::vector<Server>				_servers;
	struct pollfd					*_fds; // This is an array.
	bool							_running;
	std::map<uint32_t, std::string>	_default_error_pages;

public:
	Webserv();
	~Webserv();

	void	init(int argc, const char **argv);
	void	run();
	void	clean();
};

#endif // WEBSERV_HPP
