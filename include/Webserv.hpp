/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 22:23:26 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/24 22:47:56 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include "Config.hpp"

class Webserv
{
private:
	Config	_config;

public:
	Webserv(int argc, const char **argv);
	~Webserv(void);
};

#endif // WEBSERV_HPP
