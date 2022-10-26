/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 22:50:25 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/26 11:41:24 by ugdaniel         ###   ########.fr       */
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

void
ConfigParser::_tokenize(void)
{
	size_t		_col;
	size_t		_row;
	size_t		j;
	std::string	line;
	t_token		t;

	_row = 1;
	while (std::getline(_file, line))
	{
		_col = 0;
		while (_col < line.length() && isspace(line[_col]))
			_col++;
		// discard comments
		if (line[_col] == '#')
			continue ;
		while (_col < line.length())
		{
			while (_col < line.length() && isspace(line[_col]))
				_col++;
			if (line[_col] == '{' || line[_col] == '}')
			{
				// weird logic to get token, we substract 127 with the character to get the type value and cast it (see enum)
				t = (t_token)(127 - line[_col]);
				_token_list.push_back(Token(t, std::string(1, line[_col]), _row, _col));
				_col++;
			}
			else if (line[_col] == ';')
			{
				// semicolons are treated as newlines, might change later
				_token_list.push_back(Token(token_newline, ";", _row, _col));
				_col++;
			}
			else
			{
				j = _col;
				while (_col < line.length())
				{
					if (isspace(line[_col]) || line[_col] == '{' || line[_col] == '}' || line[_col] == ';')
						break ;
					_col++;
				}
				if (_col - j > 0)
					_token_list.push_back(Token(token_word, std::string(line, j, _col - j), _row, _col));
			}
		}
		_row++;
		_token_list.push_back(Token(token_newline, "newline", _row, _col));
	}
}

void
ConfigParser::_check_order(void)
{
	Context								_context;
	unsigned int						_braces;
	std::list<Token>::const_iterator	cur;
	std::list<Token>::const_iterator	_end;

	_context = http;
	_braces = 0;
	cur = _token_list.begin();
	_end = _token_list.end();
	while (cur != _end)
	{
		while (get_type(cur) == token_newline || get_type(cur) == token_none)
			cur++;
		if (cur == _end)
			break ;
		if (_context == http)
		{
			if (get_word(cur) != "server")
				throw_token_error(_path, (*cur), "expected 'server' block");
			cur++;
			while (cur != _end && get_type(cur) == token_newline)
				cur++;
			if (get_type(cur) != token_open_brace)
				throw_token_error(_path, (*cur), "expected '{'");
			cur++;
			while (cur != _end && get_type(cur) == token_newline)
				cur++;
			if (get_type(cur) == token_open_brace)
				throw_token_error(_path, (*cur), "unexpected '{'");
			_braces++;
			_context = server;
		}
		if (_context == server)
		{
			if (get_type(cur) == token_open_brace)
				_braces++;
			else if (get_type(cur) == token_close_brace)
			{
				if (!_braces)
					throw_token_error(_path, (*cur), "unexpected '}'");
				_braces--;
			}
			if (!_braces)
				_context = http;
		}
		cur++;
	}
	if (_braces)
	{
		if (cur == _end)
			cur--;
		throw_token_error(_path, (*cur), "expected '}'");
	}
}

void
ConfigParser::_parse_directive_listen(std::list<Token>::const_iterator& cur, Server& s)
{
	size_t								n;
	std::string							word;
	std::list<Token>::const_iterator	_end;

	cur++;
	if (get_type(cur) == token_newline || get_type(cur) == token_open_brace || get_type(cur) == token_close_brace)
		throw std::invalid_argument("listen: too few parameters");
	word = get_word(cur);
	n = std::count(word.begin(), word.end(), ':');
	if (n > 1)
		throw_token_error(_path, (*cur), "listen: bad parameter");
	if (n == 0)
	{
		// listen port
		if (str_is_numeric(word))
			s.set_port(atoi(word.c_str()));
		// listen address
		else
			s.set_host(word);
	}
	else
	{
		// listen address:port
		s.set_host(std::string(word.c_str(), word.find_first_of(':')));
		word = std::string(word.c_str() + word.find_first_of(':') + 1);
		if (not str_is_numeric(word))
			throw_token_error(_path, *cur, "port should only contain numbers ('" + word + "')");
		s.set_port(atoi(word.c_str()));
	}
	cur++;
	if (!is_line_break(get_type(cur)))
		throw std::invalid_argument("listen: unexpected parameter ('" + get_word(cur) + "')");
	s.set_state(state_listen);
}

void
ConfigParser::_parse_directive_server_name(std::list<Token>::const_iterator& cur, Server& s)
{
	std::list<Token>::const_iterator	_end;

	cur++;
	_end = _token_list.end();
	while (cur != _end && get_type(cur) == token_word)
	{
		s.add_server_name(get_word(cur));
		cur++;
	}
	s.set_state(state_server_name);
}

void
Parser::_parse_directive_error_page(std::list<Token>::const_iterator& cur, Server& s)
{
	unsigned int	_ec;

	if (!(x.get_state() & state_error_pages))
		x.clear_error_pages();
	// error code
	cur++;
	if (get_type(cur) != token_word)
		throw_token_error(_path, *cur, "error_page: expected error code");
	if (not str_is_numeric(get_word(cur).c_str()))
		throw_token_error(_path, *cur, "error_page: error code should only contain numbers");
	_ec = atoi(get_word(cur).c_str());
	cur++;
	// uri
	if (get_type(cur) != token_word)
		throw_token_error(_path, *cur, "error_page: expected uri");
	x.set_error_page(_ec, get_word(cur));
	cur++;
	if (!is_line_break(get_type(cur)))
	{
		show_token_error(_path, (*cur), "error_page: unexpected parameter: '" + get_word(cur) + "'");
		std::__throw_logic_error(ERROR_PAGE_USAGE);
	}
	x.set_state(state_error_pages);
}

const Server
ConfigParser::_parse_server_block(std::list<Token>::const_iterator& cur)
{
	Server								s;
	int									_braces;
	std::list<Token>::const_iterator	_end;

	_end = _token_list.end();
	while (cur != _end && get_type(cur) != token_word)
		cur++;	
	_braces = 1;
	while (_braces && cur != _end)
	{
		if (get_type(cur) == token_open_brace)
			_braces++;
		else if (get_type(cur) == token_close_brace)
			_braces--;
		else
		{
			if (get_word(cur) == DIRECTIVE_LISTEN)
				_parse_directive_listen(cur, s);
			else if (get_word(cur) == DIRECTIVE_SERVER_NAME)
				_parse_directive_server_name(cur, s);
			else if (get_word(cur) == DIRECTIVE_ERROR_PAGE)
				_parse_directive_error_page(cur, s);
			else if (get_word(cur) == DIRECTIVE_CLIENT_BODY_BUFFER_SIZE)
				_parse_directive_client_body_buffer_size(cur, s);
			else if (get_word(cur) == DIRECTIVE_LOCATION)
				_parse_location_block(cur, s);
			else if (get_word(cur) == DIRECTIVE_ROOT)
				_parse_directive_root(cur, s);
			else if (get_word(cur) != "newline")
				throw_token_error(_path, (*cur), "unknown server directive '" + get_word(cur) + "'");
		}
		cur++;
	}
	return (s);
}

void
ConfigParser::run(std::vector<Server>& servers)
{
	_tokenize();
	_check_order();
	std::list<Token>::const_iterator	cur;
	std::list<Token>::const_iterator	_end;
	std::list<Token>::const_iterator	start;

	cur = _token_list.begin();
	_end = _token_list.end();
	// parse file
	while (cur != _end)
	{
		if ((*cur).word() == "server")
			servers.push_back(_parse_server_block(++cur));
		else
			cur++;
	}
}
