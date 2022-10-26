/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 20:08:13 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/26 15:40:29 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "core.hpp"
# include "http.hpp"
# include "Location.hpp"
# include <arpa/inet.h>
# include <unistd.h>
# include <sys/socket.h>
# include <unistd.h>
# include <fcntl.h>

# include <map>
# include <set>
# include <string>
# include <vector>

# define MAX_PENDING_CONNECTIONS	32 // no idea for now, this is totally random need to check later

typedef struct s_listen
{
	std::string	host;
	int			port;
}t_listen;

inline bool operator==(const t_listen& x, const t_listen& y)
	{return (x.host == y.host && x.port == y.port);}

// server configuration
class Server
{
private:
	int								_socket; // previously _serverFd
	t_listen						_listen; // host:port
	struct sockaddr_in				_sockaddr;
	std::set<std::string>			_server_names;
	std::map<uint16_t, std::string>	_error_pages;
	size_t							_client_body_buffer_size;
	std::set<Location>				_locations;

	unsigned int					_state;

public:
	Server();
	~Server();

	void									setup();
	void									clean();

	const int&								get_socket() const;
	const std::string&						get_host() const;
	const int&								get_port() const;
	const std::set<std::string>&			get_server_names() const;
	const std::map<uint16_t, std::string>&	get_error_pages() const;
	const size_t&							get_client_body_buffer_size() const;
	const std::set<Location>&				get_locations() const;

	void									set_socket(const int& socket);
	void									set_host(const std::string& host);
	void									set_port(const int& port);
	void									set_server_names(const std::set<std::string> server_names);
	void									set_error_pages(const std::map<uint16_t, std::string>& error_pages);
	void									set_client_body_buffer_size(const size_t& size);
	void									set_locations(const std::set<Location>& locations);

	void									add_server_name(const std::string& name);
	void									set_error_page(uint16_t code, const std::string& path);
	void									add_location(const Location& l);

	// parser
	void			set_state(unsigned int x);
	unsigned int	get_state() const;
};

std::ostream&	operator<<(std::ostream &o, const Server& s);

#endif // SERVER_HPP
