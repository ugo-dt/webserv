/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 20:49:06 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/27 17:36:57 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

Webserv	webserv;

int	test_request()
{
	Request	*req;
	const char* buffer = "GET /watch?v=gei-A8K-YPY&list=RDMM&index=3&ab_channel=TPKSongs\r\n"
						"Host: webserv\n"
						"content-field: webserv\n"
						"Test: webserv\n"
						"Field: webserv\n"
						"Header: webserv\n"
						"\n"
						"<html>\n"
						"</html>\n"
	;
	req = new Request(buffer);

	std::cout << "----------" << std::endl;
	std::cout << "method: " << req->get_method() << std::endl;
	std::cout << "uri: " << req->get_uri() << std::endl;
	std::cout << "protocol: " << req->get_http_version() << std::endl;
	std::cout << "query string: " << req->get_query_string() << std::endl;
	std::cout << "Header fields: " << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = req->get_header_fields().begin(); it != req->get_header_fields().end(); it++)
		std::cout << "\t" << (*it).first << ": " << (*it).second << std::endl;
	std::cout << "body: " << std::endl << req->get_body() << std::endl;

	delete (req);
	return (0);
}

void	sig_handler(int signum)
{
	(void)signum;
	std::cout << std::endl << "webserv: caught signal interrupt, stopping." << std::endl;
	webserv.clean();
	exit(EXIT_SUCCESS);
}

int	main(int argc, const char **argv)
{
	try
	{
		// return (test_request());
		signal(SIGINT, sig_handler);
		webserv.init(argc, argv);
		webserv.run();
		webserv.clean();
	}
	catch (std::exception& e)
	{
		std::cout << "webserv: " << e.what() << "\e[0m" << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
