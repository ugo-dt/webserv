/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 20:49:06 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/11/02 10:03:27 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

Webserv	webserv;

void	sig_handler(int signum)
{
	(void)signum;
	std::cout << std::endl << "webserv: caught signal interrupt, stopping." << std::endl;
	webserv.clean();
	exit(EXIT_SUCCESS);
}

void	define_log(void);

int	main(int argc, const char **argv)
{
	signal(SIGINT, sig_handler);
	signal(SIGPIPE, SIG_IGN);
	if (webserv.init(argc, argv) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	try
	{
		webserv.run();
		webserv.clean();
	}
	catch (std::runtime_error& e)
	{
		std::cerr << "webserv: error: " << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
