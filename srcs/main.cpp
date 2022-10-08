/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/03 19:43:26 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/08 12:12:14 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int	main(int argc, char **argv)
{
	Config	*config;

	if (argc > 2)
	{
		std::cerr << "webserv: too many arguments" << std::endl;
		return (1);
	}
	if (argv[1])
		config = new Config(argv[1]);
	else
		config = new Config(DEFAULT_CONFIG_PATH);
	std::cout << "File path: " << config->config_path() << std::endl;

/* Since MacOS doesn’t implement write() the same way as other Unix OSes, you are allowed to use fcntl().
	You must use file descriptors in non-blocking mode in order to get a behavior similar to the one of other Unix OSes.
	However, you are allowed to use fcntl() only as follows: fcntl(fd, F_SETFL, O_NONBLOCK);
	Any other flag is forbidden. */
# if defined(__linux__)
	std::cout << "Linux" << std::endl;
# else
	std::cout << "macOS" << std::endl;
# endif

	// parse
	// run
	// clear
	return (0);
}
