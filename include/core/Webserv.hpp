/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 12:37:41 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/31 11:54:25 by ugdaniel         ###   ########.fr       */
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

typedef struct s_client
{
	int					fd;
	struct sockaddr_in	sockaddr;
	size_t				sockaddr_len;
}t_client;

class Webserv
{
private:
	std::vector<Server>				_servers;
	std::vector<t_client>			_clients;
	int								*_sockets; // This is an array.
	size_t							_nsockets;
	struct pollfd					*_poll_fds; // This is an array with all connections to each server, in a row
	bool							_running;
	std::map<uint32_t, std::string>	_default_error_pages;

	bool		_host_port_already_used(size_t max, const t_listen& l);
	void		_accept_connection(int& sock_fd, struct pollfd *_fds, const size_t &max);
	void		_handle_request(t_client& client);
	t_client*	_get_client_with_fd(const int& fd);
	void		_route_request_to_server(t_client& client, Request *_req);

public:
	Webserv();
	~Webserv();

	int		init(int argc, const char **argv);
	void	run();
	void	clean();

	const std::vector<Server>&	get_servers() {return _servers;}
};

#endif // WEBSERV_HPP
