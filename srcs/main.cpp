/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 20:49:06 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/28 16:40:44 by ugdaniel         ###   ########.fr       */
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
	WS_VALUE_LOG("Valid", req->is_valid());
	WS_VALUE_LOG("Method", req->get_method());
	WS_VALUE_LOG("URI", req->get_uri());
	WS_VALUE_LOG("Protocol", req->get_http_version());
	WS_VALUE_LOG("Query string", req->get_query_string());
	WS_VALUE_LOG("Header fields", "");
	for (std::map<std::string, std::string>::const_iterator it = req->get_header_fields().begin(); it != req->get_header_fields().end(); it++)
		WS_VALUE_LOG("\t" + (*it).first, (*it).second);
	WS_VALUE_LOG("Body", "\n" + req->get_body());

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

int	test_response()
{
	Response	*resp;
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
	// const char* bad = "GET ";
	resp = new Response(buffer);

	resp->generate();

	std::cout << "[Response]" << std::endl;
	std::cout << resp->str().c_str() << std::endl;
	
	delete (resp);
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
	return (test_response());
	signal(SIGINT, sig_handler);
	if (webserv.init(argc, argv) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	webserv.run();
	webserv.clean();
	return (EXIT_SUCCESS);
}
