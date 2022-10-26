/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 22:48:48 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/26 11:16:23 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_PARSER_HPP
# define CONFIG_PARSER_HPP

# include "Token.hpp"
# include "Server.hpp"
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
# define DIRECTIVE_CGI_EXTENSION			"cgi_extension"
# define DIRECTIVE_UPLOAD_FILES				"upload_files"

class ConfigParser
{
private:
	typedef enum e_context {http, server, location} Context;

private:
	std::list<Token>	_token_list;
	std::string			_path;
	std::fstream		_file;

	void	_open_file();
	void	_close_file();
	void	_tokenize();
	void	_check_order();
	void	_parse();

	void	_parse_directive_listen(std::list<Token>::const_iterator& cur, Server& s);

public:
	ConfigParser(int argc, const char **argv);
	~ConfigParser();

	void	run(std::vector<Server>& servers);
};

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
