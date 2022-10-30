/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 20:49:06 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/30 15:08:45 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

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
	std::string	path = "./www";
	std::string	index;

	index = ai.get_index(path, listen);
	std::cout << index << std::endl;
	return (0);
}
int	test_response()
{
	const Server&	s = webserv.get_servers()[0];
	Response	*resp;
	const char* buffer = "POST /www/upload/ HTTP/1.1\r\n"
						"Host: localhost:8080\r\n"
						"Connection: keep-alive\r\n"
						"Content-Length: 215\r\n"
						"Content-Type: multipart/form-data; boundary=----WebKitFormBoundaryXm1GxPkiB4DspXig\r\n"
						"Accept-Encoding: gzip, deflate, br\r\n"
						"Accept-Language: en-US,en;q=0.9,fr-FR;q=0.8,fr;q=0.7,ja-JP;q=0.6,ja;q=0.5\r\n"
						"\r\n"
						"------WebKitFormBoundaryXm1GxPkiB4DspXig\r\n"
						"Content-Disposition: form-data; name=\"myFile\"; filename=\"postfile\"\r\n"
						"Content-Type: application/octet-stream\r\n"
						"\r\n"
						"this is the file\r\n"
						"\r\n"
						"------WebKitFormBoundaryXm1GxPkiB4DspXig--\r\n"
	;
	resp = new Response(buffer);
	t_listen	l = {s.get_host(), s.get_port()};

	resp->generate(s.get_error_pages(), s.get_locations(), l);

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
	signal(SIGINT, sig_handler);
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
