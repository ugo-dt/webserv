/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 10:46:44 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/26 10:54:00 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_TOKEN_HPP
# define CONFIG_TOKEN_HPP

# include <cstdio>
# include <iostream>
# include <list>
# include <string>

typedef enum e_token
{
	token_none,
	token_word,

	// config file
	token_close_brace, // order is important! should be 2
	token_newline,
	token_open_brace, // order is important! should be 4
	token_semicolon,

	// requests
	token_colon
}t_token;

class Token
{
private:
	t_token			_type;
	std::string 	_word;
	unsigned int	_row;
	unsigned int	_col;

public:
	Token();
	Token(t_token, const std::string&, unsigned int, unsigned int);
	Token(const Token&);
	const Token	&operator=(const Token&);
	~Token();

	const t_token		&type(void) const;
	const std::string	&word(void) const;
	unsigned int		row(void) const;
	unsigned int		col(void) const;
};

std::ostream      &operator<<(std::ostream&, const Token&);

static inline
const std::string
_get_error_string(const std::string &path, const Token &t, const std::string &msg)
{
	char	col[5];
	char	row[5];

	std::snprintf(col, 5, "%u", t.col());
	std::snprintf(row, 5, "%u", t.row());
	return (std::string("webserv: \e[1m" + path + ":"
	         + row + ":" + col
	         + ": \e[31merror:\e[39m "
			 + msg
			 + "\e[0m"));
}

static inline
const std::string
_get_warning_string(const std::string &path, const Token &t, const std::string &msg)
{
	char	col[5];
	char	row[5];

	std::snprintf(col, 5, "%u", t.col());
	std::snprintf(row, 5, "%u", t.row());
	return (std::string("webserv: \e[1m" + path + ":"
	         + row + ":" + col
	         + ": \e[95mwarning:\e[39m "
			 + msg
			 + "\e[0m"));
}

static inline void show_repeat_warning(const std::string& path, const Token& t)
	{std::cerr << _get_warning_string(path, t, "repeated directive ('" + t.word() + "')") << std::endl;}
static inline void show_token_warning(const std::string& path, const Token& t, const std::string& msg)
	{std::cerr << _get_warning_string(path, t, msg) << std::endl;}
static inline void show_token_error(const std::string& path, const Token& t, const std::string& msg)
	{std::cerr << _get_error_string(path, t, msg) << std::endl;}
static inline void throw_token_error(const std::string& path, const Token& t, const std::string& msg)
	{throw std::logic_error(_get_error_string(path, t, msg));}
static inline const std::string& get_word(const std::list<Token>::const_iterator& x)
	{return ((*x).word());}
static inline const t_token& get_type(const std::list<Token>::const_iterator& x)
	{return ((*x).type());}

#endif // CONFIG_TOKEN_HPP
