/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 22:48:48 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/29 16:23:10 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_PARSER_HPP
# define CONFIG_PARSER_HPP

# include "Location.hpp"
# include "Server.hpp"
# include "Token.hpp"
# include <fstream>
# include <list>
# include <sys/stat.h>
# include <vector>

# define DEFAULT_CONFIG_PATH				"config/default"

# define MAX_ERRORS							20

# define DIRECTIVE_LISTEN					"listen"
# define DIRECTIVE_SERVER_NAME				"server_name"
# define DIRECTIVE_ERROR_PAGE				"error_page"
# define DIRECTIVE_CLIENT_BODY_BUFFER_SIZE	"client_body_buffer_size"
# define DIRECTIVE_LOCATION					"location"
# define DIRECTIVE_LIMIT_EXCEPT				"limit_except"
# define DIRECTIVE_REWRITE					"rewrite"
# define DIRECTIVE_ROOT						"root"
# define DIRECTIVE_AUTOINDEX				"autoindex"
# define DIRECTIVE_DEFAULT_FILE				"default_file"
# define DIRECTIVE_CGI_EXTENSION			"cgi_extension"
# define DIRECTIVE_UPLOAD_PATH				"upload_path"

typedef enum e_state
{
	state_http_hethods            = 0x01,
	state_http_redirection        = 0x02,
	state_root                    = 0x04,
	state_autoindex               = 0x08,
	state_error_pages             = 0x10,
	state_listen                  = 0x20,
	state_server_name             = 0x40,
	state_client_body_buffer_size = 0x80,
	state_default_file            = 0x100,
	state_cgi_extension           = 0x200,
	state_upload_path             = 0x400,
	state_limit_except            = 0x800
}t_state;

class ConfigParser
{
private:
	typedef enum e_context {http, server, location} Context;

private:
	std::list<ConfigToken>					_token_list;
	std::list<ConfigToken>::const_iterator	_end;
	std::string								_config_path;
	std::ifstream							_file;
	size_t									_errors;

	void			_open_file();
	void			_close_file();
	void			_tokenize();
	void			_check_order();
	void			_parse();
	void			_print_error(const std::string& path, const ConfigToken& t, const std::string& msg);
	void			_throw_token_error(const std::string& path, const ConfigToken& t, const std::string& msg);

	const Server	_parse_server_block(std::list<ConfigToken>::const_iterator& cur);
	void			_parse_directive_listen(std::list<ConfigToken>::const_iterator& cur, Server& s);
	void			_parse_directive_server_name(std::list<ConfigToken>::const_iterator&, Server&);
	void			_parse_directive_error_page(std::list<ConfigToken>::const_iterator& cur, Server& s);
	void			_parse_directive_client_body_buffer_size(std::list<ConfigToken>::const_iterator& cur, Server& x);

	void			_parse_location_block(std::list<ConfigToken>::const_iterator& cur, Server& s);
	void 			_parse_directive_limit_except(std::list<ConfigToken>::const_iterator& cur, Location& l);
	void 			_parse_directive_rewrite(std::list<ConfigToken>::const_iterator& cur, Location& l);
	void 			_parse_directive_root(std::list<ConfigToken>::const_iterator& cur, Location& l);
	void 			_parse_directive_autoindex(std::list<ConfigToken>::const_iterator& cur, Location& l);
	void			_parse_directive_default_file(std::list<ConfigToken>::const_iterator& cur, Location& l);
	void			_parse_directive_cgi_extension(std::list<ConfigToken>::const_iterator& cur, Location& l);
	void			_parse_directive_upload_path(std::list<ConfigToken>::const_iterator& cur, Location& l);

public:
	ConfigParser();
	~ConfigParser();

	void	init(int argc, const char **argv);
	void	run(std::vector<Server>& servers);
};

inline bool	operator==(const std::list<ConfigToken>::const_iterator& it, const t_token& t)
	{return (get_type(it) == t);}
inline bool	operator!=(const std::list<ConfigToken>::const_iterator& it, const t_token& t)
	{return (get_type(it) != t);}
inline bool	operator==(const std::list<ConfigToken>::const_iterator& it, const std::string& s)
	{return (get_word(it) == s);}
inline bool	operator!=(const std::list<ConfigToken>::const_iterator& it, const std::string& s)
	{return (get_word(it) != s);}

void print_server_info_debug(const Server &s);

static inline
bool
str_is_numeric(const char *s)
{
	if (!s)
		return (true);
	for (; *s; s++)
		if (std::isdigit(*s) == 0)
			return (false);
	return (true);
}

static inline bool	str_is_numeric(const std::string& s)
	{return (str_is_numeric(s.c_str()));}

static inline bool	is_line_break(const t_token& t)
	{return (t == token_newline || t == token_close_brace || t == token_semicolon);}
static inline bool	is_line_break(const ConfigToken& t)
	{return (is_line_break(t.type()));}
static inline bool	is_line_break(const std::list<ConfigToken>::const_iterator& it)
	{return (is_line_break(*it));}
static inline bool	is_separator(const char& c)
	{return (c == '{' || c == '}' || c == ';' || c == '#');}

static inline
const std::string
get_n_spaces(size_t n)
{
	std::string	s;
	for (; n > 0; n--)
		s.append(" ");
	return (s);
}

#endif // CONFIG_PARSER_HPP
