/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 22:50:25 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/25 23:03:13 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"

ConfigParser::ConfigParser(int argc, const char **argv)
{
	std::string	_err;
	struct stat	_stat;

	// Use default file if none is provided
	if (argc > 2)
		throw std::invalid_argument("too many arguments");
	if (argc < 2)
	{
		_path = DEFAULT_CONFIG_PATH;
		return ;
	}
	if (argv[1] && *argv[1])
	{
		if (stat(argv[1], &_stat) == 0)
		{
			if (S_ISREG(_stat.st_mode))
				_path = argv[1];
			else if (S_ISDIR(_stat.st_mode))
				throw std::invalid_argument("webserv: \e[1m\e[31merror:\e[39m is a directory: '" + _path + "'\e[0m");
		}
		else
			_err.append("webserv: \e[1m\e[31merror:\e[39m file not found: '" + _path + "'\e[0m\n");
	}
	else
		_err.append("webserv: \e[1m\e[31merror:\e[39m no input files\e[0m\n");
	if (!_err.empty())
	{
		_err.erase(_err.size() - 1);
		throw std::invalid_argument(_err);
	}
}

ConfigParser::~ConfigParser()
	{_close_file();}

void
ConfigParser::_open_file(void)
{
	_file.open(_path.c_str(), std::ifstream::out);
	if (!_file.is_open())
		throw std::invalid_argument("webserv: \e[1m\e[31merror:\e[39m could not open file: " + _path + "\e[0m");
}

void
ConfigParser::_close_file(void)
{
	if (_file.is_open())
		_file.close();
}