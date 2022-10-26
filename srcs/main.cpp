/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 20:49:06 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/26 10:36:34 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

Webserv	webserv;

void	sig_handler(int signum)
{
	(void)signum;
	std::cout << std::endl << "webserv: caught signal interrupt" << std::endl;
	webserv.clean();
	exit(EXIT_SUCCESS);
}

int	main(int argc, const char **argv)
{
	try
	{
		signal(SIGINT, sig_handler);
		webserv.init(argc, argv);
		webserv.run();
		webserv.clean();
	}
	catch (std::exception& e)
	{
		std::cout << "webserv: " << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
