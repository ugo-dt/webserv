/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 15:07:16 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/11/03 11:57:14 by ugdaniel         ###   ########.fr       */
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
CGI::_clean()
{
	if (_args)
	{
		for (size_t i = 0; i < ARGS_VARIABLES; i++)
		{
			if (_args[i])
			{
				free(_args[i]);
				_args[i] = NULL;
			}
		}
		free(_args);
		_args = NULL;
	}
	if (_env)
	{
		for (size_t i = 0; i < ENV_VARIABLES; i++)
		{
			if (_env[i])
			{
				free(_env[i]);
				_env[i] = NULL;
			}
		}
		free(_env);
		_env = NULL;
	}
}

void
CGI::_add_arg(const std::string& arg)
{
	for (size_t i = 0; i < ARGS_VARIABLES; i++)
	{
		if (_args[i] == 0)
		{
			_args[i] = strdup(arg.c_str());
			if (!_args[i])
			{
				_clean();
				_throw_errno("strdup");
			}
			return ;
		}
	}
}

void
CGI::_add_env(const std::string& name, const std::string& value)
{
	for (size_t i = 0; i < ENV_VARIABLES; i++)
	{
		if (_env[i] == 0)
		{
			_env[i] = strdup(std::string(name + "=" + value).c_str());
			if (!_env[i])
			{
				_clean();
				_throw_errno("strdup");
			}
			return ;
		}
	}
}

void
CGI::init(void)
{
	std::string	str;
	char		*pwd;

	_args = (char **)malloc(sizeof(char *) * (ARGS_VARIABLES + 1));
	if (!_args)
		_throw_errno("malloc");
	for (size_t i = 0; i < ARGS_VARIABLES + 1; i++)
		_args[i] = 0;
	_add_arg(_executable);
	_add_arg("." + _req.get_uri());

	_env = (char **)malloc(sizeof(char *) * (ENV_VARIABLES + 1));
	if (!_env)
		_throw_errno("init: malloc");
	for (size_t i = 0; i < ENV_VARIABLES + 1; i++)
		_env[i] = 0;

	if ((pwd = getcwd(NULL, 0)) == NULL)
		_throw_errno("getcwd");
	_add_env("PATH_INFO", pwd + _req.get_uri());
	_add_env("SERVER_PROTOCOL", HTTP_PROTOCOL);
	_add_env("REDIRECT_STATUS", "200");
	_add_env("REQUEST_METHOD", "GET");
	_add_env("QUERY_STRING", _req.get_query_string());
	free(pwd);
#ifdef WS_LOG
	WS_INFO_LOG("Environment variables: ");
	for (size_t i = 0; _env[i]; i++)
		WS_INFO_LOG(_env[i]);
	WS_INFO_LOG("Arguments: ");
	for (size_t i = 0; _args[i]; i++)
		WS_INFO_LOG(_args[i]);
#endif
}

void
CGI::run(void)
{
	int fdOut[2];
	int fdIN[2];
	
	if (pipe(fdOut) < 0 || pipe(fdIN) < 0)
		_throw_errno("pipe");

	pid_t pid = fork();
	if (pid == -1)
		_throw_errno("fork");
	if (!pid)
	{
		// stdout is now a copy of fdOut[1] and in case post method, stdin is a copy of fdIn[0]
		dup2(fdOut[1], STDOUT_FILENO);
		close(fdOut[0]);
		close(fdOut[1]);

		dup2(fdIN[0], STDIN_FILENO);
		close(fdIN[0]);
		close(fdIN[1]);

		// change the repo into where the program is
		chdir(_env[0]);
	
		if (execve(_args[0], _args, _env) < 0)
		{
			_clean();
			exit(255);
		}
	}
	else if (pid > 0)
	{
		close(fdOut[1]);
		close(fdIN[1]);
		close(fdIN[0]);

        // Checking if execve correctly worked
        int status = 0;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		{
			_clean();
			close(fdOut[0]);
			throw std::runtime_error("execve failed, make sure file exists and is accessible");
		}
        
		std::string msgbody;
		char buf[2046 + 1];
		memset(buf, 0, 2046 + 1);
		while (read(fdOut[0], buf, 2046) > 0)
		{
			msgbody += buf;
			memset(buf, 0, 2046 + 1);
		}
		msgbody += buf;
		close(fdOut[0]);
		_body = msgbody;
		_clean();
	}
}

CGI::~CGI()
{
	_clean();
}

const std::string&
CGI::get_body()
{
	return _body;
}

size_t
CGI::get_body_size()
{
	return _body_size;
}
