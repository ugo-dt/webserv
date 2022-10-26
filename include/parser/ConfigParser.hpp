/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 22:48:48 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/26 15:48:49 by ugdaniel         ###   ########.fr       */
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
	state_upload_path            = 0x400
}t_state;

class ConfigParser
{
private:
	typedef enum e_context {http, server, location} Context;

private:
	std::list<Token>	_token_list;
	std::string			_path;
	std::ifstream		_file;

	void			_open_file();
	void			_close_file();
	void			_tokenize();
	void			_check_order();
	void			_parse();

	const Server	_parse_server_block(std::list<Token>::const_iterator& cur);
	void			_parse_directive_listen(std::list<Token>::const_iterator& cur, Server& s);
	void			_parse_directive_server_name(std::list<Token>::const_iterator&, Server&);
	void			_parse_directive_error_page(std::list<Token>::const_iterator& cur, Server& s);
	void			_parse_directive_client_body_buffer_size(std::list<Token>::const_iterator& cur, Server& x);

	void			_parse_location_block(std::list<Token>::const_iterator& cur, Server& s);
	void 			_parse_directive_limit_except(std::list<Token>::const_iterator& cur, Location& l);
	void 			_parse_directive_rewrite(std::list<Token>::const_iterator& cur, Location& l);
	void 			_parse_directive_root(std::list<Token>::const_iterator& cur, Location& l);
	void 			_parse_directive_autoindex(std::list<Token>::const_iterator& cur, Location& l);
	void			_parse_directive_default_file(std::list<Token>::const_iterator& cur, Location& l);
	void			_parse_directive_cgi_extension(std::list<Token>::const_iterator& cur, Location& l);
	void			_parse_directive_upload_path(std::list<Token>::const_iterator& cur, Location& l);

public:
	ConfigParser();
	~ConfigParser();

	void	init(int argc, const char **argv);
	void	run(std::vector<Server>& servers);
};

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

static inline
bool
str_is_numeric(const std::string& s)
{
	return (str_is_numeric(s.c_str()));
}

static inline
bool
is_line_break(const t_token& t)
{
	return (t == token_newline || t == token_close_brace || t == token_semicolon);
}

#endif // CONFIG_PARSER_HPP
