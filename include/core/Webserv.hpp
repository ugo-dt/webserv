/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 12:37:41 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/11/01 19:24:05 by ugdaniel         ###   ########.fr       */
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

inline bool	operator==(const t_client& x, const t_client& y)
	{return (x.fd == y.fd);}

class Webserv
{
private:
	std::vector<Server>				_servers;
	std::vector<t_client>			_clients;
	int								*_sockets; // This is an array.
	size_t							_nsockets;
	struct pollfd					*_poll_fds; // This is an array with all connections to each server, in a row
	nfds_t							_nfds;
	bool							_running;
	std::map<uint32_t, std::string>	_default_error_pages;

	Server*	_find_matching_server(const std::string& _host, const std::string& _port, t_client& client);
	int		_unchunked_the_request(Request& _req, t_client& client, const char *_buffer);
	void	_route_request_to_server(t_client& client, const char *_buffer);
	void	_handle_request(t_client& client);
	void	_accept_connection(int& sock_fd);

	bool	_host_port_already_used(size_t max, const t_listen& l);
	void	_send_bad_request(int& fd);
	void	_remove_client(t_client& client);
	int		_get_client_with_fd(int fd);

public:
	Webserv();
	~Webserv();

	int		init(int argc, const char **argv);
	void	run();
	void	clean();

	const std::vector<Server>&	get_servers() {return _servers;}
};

static inline
void
close_fd(int& fd)
{
	if (fd == -1)
		return ;
	WS_VALUE_LOG("File descriptor closed", fd);
	close(fd);
	fd = -1;
}

#endif // WEBSERV_HPP
