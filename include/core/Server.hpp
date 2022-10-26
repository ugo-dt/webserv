/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 20:08:13 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/25 23:26:12 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "core.hpp"
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

typedef struct s_location
{
	std::string		name;
	unsigned int	methods;
	std::string		root;
	bool			redirect;
	bool			autoindex;
	std::string		default_file;
	std::string		cgi_extension;
	std::string		upload_files;
}t_location;

typedef struct s_listen
{
	ssize_t		port;	
	std::string	host;
}t_listen;

// server configuration
class Server
{
private:
	int								_socket;
	t_listen						_listen;
	struct sockaddr_in				_sockaddr;
	std::set<std::string>			_server_names;
	std::map<size_t, std::string>	_error_pages;
	size_t							_client_body_buffer_size;
	std::vector<t_location>			_locations;

public:
	Server();
	~Server();

	void	init();
	void	clean();
};

#endif // SERVER_HPP
