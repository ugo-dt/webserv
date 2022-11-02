/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Autoindex.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 12:11:55 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/28 18:20:00 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUTOINDEX_HPP
# define AUTOINDEX_HPP

# include "http.hpp"
# include "log.hpp"
# include "string_utils.hpp"

# include "dirent.h"
# include <cstring>
# include <string>
# include <sstream>
# include <map>

class Autoindex
{
private:
	std::string							_result;
	std::map<std::string, std::string>	_links; // link name, link

	void	_create_link_and_sort(const t_listen& host_port, struct dirent *entity, std::string path);
	
public:
	Autoindex();
	~Autoindex();

	const std::string	get_index(std::string dir_path, const t_listen& host_port);
};

#endif // AUTOINDEX_HPP
