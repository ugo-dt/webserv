/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 15:07:16 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/11/02 17:00:09 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"

CGI::CGI(const Request& req, const std::string& ext, const std::string& exec)
	: _req(req),
	  _extension(ext),
	  _executable(exec),
	  _args(NULL),
	  _env(NULL)
{
}

void
CGI::init(void)
{
	std::string	str;
	char		*pwd;
	size_t		i;

	//Getting absolute path
	if ((pwd = getcwd(NULL, 0)) == NULL)
		_throw_errno("getcwd");
	str = pwd + _req.get_uri();

	_env = (char **)malloc(sizeof(char *) * 6 + 1);
	if (!_env)
		_throw_errno("init: malloc");
	str.insert(0, "PATH_INFO=");
	i = 0;
	_env[i++] = strdup(str.c_str());
	_env[i++] = strdup("SERVER_PROTOCOL=HTTP/1.1");
	_env[i++] = strdup("REDIRECT_STATUS=200");
	if (_executable.compare("php-cgi") == 0)
        _env[i++] = strdup("REQUEST_METHOD=GET");
	str = "QUERY_STRING=" + _req.get_query_string();
	_env[i++] = strdup(str.c_str());
	_env[i++] = NULL;
}

const std::string
CGI::run(void)
{
	return ("yeee cgi no problem");
}

// CGI::CGI(const Request& req)
// {
// 	char		*pwd;
// 	std::string	absolute_path;

// 	//Getting absolute path
// 	if ((pwd = getcwd(NULL, 0)) == NULL)
// 		_throw_errno("getcwd");
// 	absolute_path = pwd + req.get_uri();

// 	//Getting extension
// 	_extension = absolute_path.substr(absolute_path.find_last_of('.'), std::string::npos);

// 	// Choosing which cgi bin to execve
// 	// _executable = _get_cgi_name();

// 	if ((_env = new char*[7]) == NULL)
// 		throw std::runtime_error("webserv: env new failed.");
	
// 	int i = 0;
// 	_env[i++] = strdup(("PATH_INFO=" + absolute_path).c_str());
// 	_env[i++] = strdup("SERVER_PROTOCOL=HTTP/1.1");
// 	_env[i++] = strdup("REDIRECT_STATUS=200");
	
// 	if (req.get_method() == METHOD_GET)
// 	{
// 		if (!_executable.compare("php-cgi"))
//             _env[i++] = strdup("REQUEST_METHOD=GET");
		
// 		_env[i++] = strdup(("QUERY_STRING=" + req.get_query_string()).c_str());
// 	}
// 	_env[i++] = NULL;

// 	if ((_args = new char*[3]) == NULL)
// 		throw std::runtime_error("webserv: args new failed");

// 	_args[0] = strdup(_executable.c_str());
// 	_args[1] = (!_executable.compare(".cgi")) ? NULL : (char*)req.get_uri().c_str();
// 	_args[2] = NULL;

// 	for (int i = 0; _env[i] != NULL; i++)
// 	{
// 		std::cout << "_env: " << _env[i] << std::endl;			//CRAPPY DEBUG LINE HERE
// 	}
// }

CGI::~CGI()
{
	//for (int i = 0; _env[i] != NULL; i++)
	//{
	//	free(_env[i]);
	//	_env[i] = NULL;
	//}
	//delete [] _env;
	//for (int i = 0; _args[i] != NULL; i++)
	//{
	//	free(_args[i]);
	//	_args[i] = NULL;
	//}
	//delete [] _env;
}

// void			CGI::run()
// {
// 	int		fd_out[2];
// 	int		fd_in[2];

// 	if (pipe(fd_out) < 0 || pipe(fd_in) < 0)
// 		throw std::runtime_error("webserv: pipe failed");

// 	pid_t pid = fork();
// 	if (pid == 0)
// 	{
// 		dup2(fd_out[1], STDOUT_FILENO);
// 		close(fd_out[0]);
// 		close(fd_out[1]);

// 		dup2(fd_in[0], STDIN_FILENO);
// 		close(fd_in[0]);
// 		close(fd_in[1]);
// 		//chdir(_path_info.first.c_str());
// 		if (execve(_args[0], _args, _env) < 0)
// 		{
// 			throw std::runtime_error("webserv: execve failed.");
// 		}
// 	}
// 	else if (pid > 0)
// 	{
// 		close(fd_out[1]);
// 		if (_req.get_method() == METHOD_POST)
// 		{
// 			if (write(fd_in[1], _req.get_body().c_str(), _req.get_body().size()) < 0)
// 				throw std::runtime_error("webserv: write failed.");
// 		}
// 	}
// }
