/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 22:50:25 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/26 15:33:03 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"

ConfigParser::ConfigParser() {}

void
ConfigParser::init(int argc, const char **argv)
{
	std::string	_err;
	struct stat	_stat;

	// Use default file if none is provided
	if (argc > 2)
		throw std::invalid_argument("\e[1m\e[31merror:\e[39m too many arguments");
	if (argc < 2)
	{
		_path = DEFAULT_CONFIG_PATH;
	}
	else
	{
		if (argv[1] && *argv[1])
		{
			_path = argv[1];
			if (stat(_path.c_str(), &_stat) == 0)
			{
				if (S_ISDIR(_stat.st_mode))
					throw std::invalid_argument("\e[1m\e[31merror:\e[39m is a directory: '" + _path + "'");
			}
			else
				_err.append("\e[1m\e[31merror:\e[39m file not found: '" + _path + "'\n");
		}
		else
			_err.append("\e[1m\e[31merror:\e[39m no input files\n");
		if (!_err.empty())
		{
			_err.erase(_err.size() - 1);
			throw std::invalid_argument(_err);
		}
	}
	_open_file();
}

ConfigParser::~ConfigParser()
	{_close_file();}

void
ConfigParser::_open_file(void)
{
	_file.open(_path.c_str(), std::ifstream::out);
	if (!_file.is_open())
		throw std::invalid_argument("webserv: \e[1m\e[31merror:\e[39m could not open file: " + _path + "");
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
		throw_token_error(_path, (*cur), "listen: too few parameters");
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
		if (!str_is_numeric(word))
			throw_token_error(_path, *cur, "listen: port should only contain numbers ('" + word + "')");
		s.set_port(atoi(word.c_str()));
	}
	cur++;
	if (!is_line_break(get_type(cur)))
		throw_token_error(_path, (*cur), "listen: unexpected parameter ('" + get_word(cur) + "')");
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
ConfigParser::_parse_directive_error_page(std::list<Token>::const_iterator& cur, Server& s)
{
	unsigned int	_ec;

	// error code
	cur++;
	if (get_type(cur) != token_word)
		throw_token_error(_path, *cur, "error_page: expected error code");
	if (!str_is_numeric(get_word(cur).c_str()))
		throw_token_error(_path, *cur, "error_page: error code should only contain numbers");
	_ec = atoi(get_word(cur).c_str());
	cur++;
	// uri
	if (get_type(cur) != token_word)
		throw_token_error(_path, *cur, "error_page: expected uri");
	s.set_error_page(_ec, get_word(cur));
	cur++;
	if (!is_line_break(get_type(cur)))
		throw_token_error(_path, (*cur), "error_page: unexpected parameter: '" + get_word(cur) + "'");
	s.set_state(state_error_pages);
}

void
ConfigParser::_parse_directive_client_body_buffer_size(std::list<Token>::const_iterator& cur, Server& s)
{
	if (s.get_state() & state_client_body_buffer_size)
		show_repeat_warning(_path, (*cur));
	cur++;
	if (!str_is_numeric(get_word(cur).c_str()))
		throw_token_error(_path, *cur, "client_body_buffer_size: size should only contain numbers");
	s.set_client_body_buffer_size(atoi(get_word(cur).c_str()));
	cur++;
	if (!is_line_break(get_type(cur)))
		throw_token_error(_path, (*cur), "client_body_buffer_size: unexpected parameter: '" + get_word(cur) + "'");
	s.set_state(state_client_body_buffer_size);
}

void
ConfigParser:: _parse_directive_limit_except(std::list<Token>::const_iterator& cur, Location& l)
{
	std::string							_method;
	std::list<Token>::const_iterator	_end;

	if (l.get_state() & state_server_name)
		show_repeat_warning(_path, (*cur));
	cur++;
	if (get_type(cur) == token_newline)
		throw_token_error(_path, (*cur), "limit_except: expected method");
	_end = _token_list.end();
	while (cur != _end && get_type(cur) == token_word)
	{
		_method = get_word(cur);
		if (_method == "GET")
			l.add_method(METHOD_GET);
		else if (_method == "POST")
			l.add_method(METHOD_POST);
		else if (_method == "DELETE")
			l.add_method(METHOD_DELETE);
		else
			throw_token_error(_path, (*cur), "limit_except: unknown method ('" + _method + "')");
		cur++;
	}
	l.set_state(state_server_name);
}

// rewrite path replacement
void
ConfigParser::_parse_directive_rewrite(std::list<Token>::const_iterator& cur, Location& l)
{
	std::string							_path;
	std::list<Token>::const_iterator	_end;

	_end = _token_list.end();
	if (l.get_state() & state_http_redirection)
		show_repeat_warning(_path, (*cur));
	cur++;
	if (get_type(cur) == token_newline)
		throw_token_error(_path, (*cur), "rewrite: too few parameters");
	_path = get_word(cur);
	cur++;
	if (cur == _end || get_type(cur) != token_word)
		throw_token_error(_path, (*cur), "rewrite: expected replacement");
	l.add_redirection(_path, get_word(cur));
	cur++;
	if (get_type(cur) != token_newline && get_type(cur) != token_close_brace)
		throw_token_error(_path, (*cur), "rewrite: unexpected parameter ('" + get_word(cur) + "')");
	l.set_state(state_http_redirection);
}

void
ConfigParser::_parse_directive_root(std::list<Token>::const_iterator& cur, Location& l)
{
	if (l.get_state() & state_root)
		show_repeat_warning(_path, (*cur));
	cur++;
	if (get_type(cur) == token_newline)
		throw_token_error(_path, (*cur), "root: too few parameters");
	l.set_root(get_word(cur));
	cur++;
	if (!is_line_break(get_type(cur)))
		throw_token_error(_path, (*cur), "root: unexpected parameter ('" + get_word(cur) + "')");
	l.set_state(state_root);
}

void
ConfigParser::_parse_directive_autoindex(std::list<Token>::const_iterator& cur, Location& l)
{
	std::string	_ai;

	if (l.get_state() & state_autoindex)
		show_repeat_warning(_path, (*cur));
	cur++;
	if (get_type(cur) == token_newline)
		throw_token_error(_path, (*cur), "autoindex: too few parameters");
	_ai = get_word(cur);
	if (_ai != "on" && _ai != "off")
		throw_token_error(_path, (*cur), "autoindex: wrong parameter ('" + _ai + "')");
	if (_ai == "on")
	{
		// Make sure location is a folder
		if (l.get_uri().at(l.get_uri().length() - 1) != '/')
			throw_token_error(_path, (*cur), "autoindex: not a folder ('" + l.get_uri() + "')");
		l.set_autoindex(true);
	}
	else
		l.set_autoindex(false);
	cur++;
	if (!is_line_break(get_type(cur)))
		throw_token_error(_path, (*cur), "autoindex: unexpected parameter ('" + get_word(cur) + "')");
	l.set_state(state_autoindex);
}

void
ConfigParser::_parse_directive_default_file(std::list<Token>::const_iterator& cur, Location& l)
{
	if (l.get_state() & state_default_file)
		show_repeat_warning(_path, (*cur));
	cur++;
	if (get_type(cur) == token_newline)
		throw_token_error(_path, (*cur), "default_file: too few parameters");
	l.set_default_file(get_word(cur));
	cur++;
	if (!is_line_break(get_type(cur)))
		throw_token_error(_path, (*cur), "default_file: unexpected parameter ('" + get_word(cur) + "')");
	l.set_state(state_default_file);
}

void
ConfigParser::_parse_directive_cgi_extension(std::list<Token>::const_iterator& cur, Location& l)
{
	if (l.get_state() & state_cgi_extension)
		show_repeat_warning(_path, (*cur));
	cur++;
	if (get_type(cur) == token_newline)
		throw_token_error(_path, (*cur), "cgi_extension: too few parameters");
	l.set_cgi_extension(get_word(cur));
	cur++;
	if (!is_line_break(get_type(cur)))
		throw_token_error(_path, (*cur), "cgi_extension: unexpected parameter ('" + get_word(cur) + "')");
	l.set_state(state_cgi_extension);
}

void
ConfigParser::_parse_directive_upload_path(std::list<Token>::const_iterator& cur, Location& l)
{
	if (l.get_state() & state_upload_path)
		show_repeat_warning(_path, (*cur));
	cur++;
	if (get_type(cur) == token_newline)
		throw_token_error(_path, (*cur), "upload_path: too few parameters");
	l.set_upload_path(get_word(cur));
	cur++;
	if (!is_line_break(get_type(cur)))
		throw_token_error(_path, (*cur), "upload_path: unexpected parameter ('" + get_word(cur) + "')");
	l.set_state(state_upload_path);
}

void
ConfigParser::_parse_location_block(std::list<Token>::const_iterator& cur, Server& s)
{
	Location							l;
	int									_braces;
	std::list<Token>::const_iterator	_end;

	_end = _token_list.end();
	cur++;

	// uri
	if (cur == _end || get_type(cur) != token_word)
		throw_token_error(_path, (*cur), "location: expected uri");
	l.set_uri(get_word(cur));
	cur++;

	// block
	if (cur == _end || get_type(cur) != token_open_brace)
		throw_token_error(_path, (*cur), "expected '{'");
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
			if (get_word(cur) == DIRECTIVE_LIMIT_EXCEPT)
				_parse_directive_limit_except(cur, l);
			else if (get_word(cur) == DIRECTIVE_REWRITE)
				_parse_directive_rewrite(cur, l);
			else if (get_word(cur) == DIRECTIVE_ROOT)
				_parse_directive_root(cur, l);
			else if (get_word(cur) == DIRECTIVE_AUTOINDEX)
				_parse_directive_autoindex(cur, l);
			else if (get_word(cur) == DIRECTIVE_DEFAULT_FILE)
				_parse_directive_default_file(cur, l);
			else if (get_word(cur) == DIRECTIVE_CGI_EXTENSION)
				_parse_directive_cgi_extension(cur, l);
			else if (get_word(cur) == DIRECTIVE_UPLOAD_PATH)
				_parse_directive_upload_path(cur, l);
			else if (get_word(cur) != "newline")
				throw_token_error(_path, (*cur), "unknown location directive '" + get_word(cur) + "'");
		}
		cur++;
	}
	for (std::set<Location>::const_iterator it = s.get_locations().begin(); it != s.get_locations().end(); it++)
		if ((*it).get_uri() == l.get_uri())
			std::cerr << _get_warning_string(_path, (*cur), "location: repeated uri is unused ('" + l.get_uri() + "')") << std::endl;
	s.add_location(l);
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
			// else if (get_word(cur) == DIRECTIVE_ROOT)
			// 	_parse_directive_root(cur, s);
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
