/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 12:37:41 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/30 11:58:08 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include "core.hpp"
# include "http.hpp"

# include "ConfigParser.hpp"
# include "RequestParser.hpp"
# include "Server.hpp"

# include <csignal>
# include <cstdlib>
# include <map>
# include <poll.h>
# include <sys/stat.h>
# include <vector>

class Webserv
{
private:
	std::vector<Server>				_servers;
	int								*_sockets; // This is an array.
	size_t							_nsockets;
	struct pollfd					*_poll_fds; // This is an array with all connections to each server, in a row
	bool							_running;
	std::map<uint32_t, std::string>	_default_error_pages;

public:
	Webserv();
	~Webserv();

	int		init(int argc, const char **argv);
	void	run();
	void	clean();

	const std::vector<Server>&	get_servers() {return _servers;}
};

#endif // WEBSERV_HPP
