/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 20:49:06 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/28 15:43:04 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

#include "Autoindex.hpp"

Webserv	webserv;

int	test_request()
{
	Request	*req;
	const char* buffer = "GET /index.html?arg1=value1&arg2=value2 HTTP/1.1\r\n"
						"Host: webserv\r\n"
						"content-field: webserv\r\n"
						"Test: webserv\r\n"
						"Field: webserv\r\n"
						"Header: webserv\r\n"
						"\r\n"
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
	std::cout << "valid: " << std::endl << req->is_valid() << std::endl;

	delete (req);
	return (0);
}
int	test_autoindex()
{
	// root / www
	// / -> /www/
	// index.html -> www/index.html
	//
	// index given path is localhost/www/index.html
	// so it will create links such as localhost/
	Autoindex	ai;
	t_listen	listen = {"localhost", 7777};
	std::string	path = "/mnt/nfs/homes/ugdaniel/sgoinfre/goinfre/ugdaniel/webserv/www";
	std::string	index;

	index = ai.get_index(path, listen);
	std::cout << index << std::endl;
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
	// return (test_request());
	signal(SIGINT, sig_handler);
	if (webserv.init(argc, argv) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	webserv.run();
	webserv.clean();
	return (EXIT_SUCCESS);
}
