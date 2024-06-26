/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 10:46:44 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/31 17:24:36 by ugdaniel         ###   ########.fr       */
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
	token_newline,
	token_close_brace, // order is important! should be 2
	token_semicolon,
	token_open_brace, // order is important! should be 4
	token_word
}t_token;

class Token
{
protected:
	t_token		_type;
	std::string _word;

public:
	Token(): _type(token_none), _word("") {}
	Token(t_token t, const std::string& w): _type(t), _word(w) {}
	virtual ~Token() {}

	const t_token		&type(void) const {return _type;}
	const std::string	&word(void) const {return _word;}
};

class ConfigToken
	: public Token
{
private:
	unsigned int	_row;
	unsigned int	_col;

public:
	ConfigToken();
	ConfigToken(t_token, const std::string&, unsigned int, unsigned int);
	ConfigToken(const ConfigToken&);
	const ConfigToken	&operator=(const ConfigToken&);
	~ConfigToken();

	unsigned int		row(void) const;
	unsigned int		col(void) const;
};

std::ostream      &operator<<(std::ostream&, const ConfigToken&);

static inline
const std::string
_get_error_string(const std::string &path, const ConfigToken &t, const std::string &msg)
{
	char	col[5];
	char	row[5];

	std::snprintf(col, 5, "%u", t.col());
	std::snprintf(row, 5, "%u", t.row());
	return (std::string("\033[1m" + path + ":"
	         + row + ":" + col
	         + ": \033[31merror:\033[39m "
			 + msg
			 + "\033[0m"));
}

static inline
const std::string
_get_warning_string(const std::string &path, const ConfigToken &t, const std::string &msg)
{
	char	col[5];
	char	row[5];

	std::snprintf(col, 5, "%u", t.col());
	std::snprintf(row, 5, "%u", t.row());
	return (std::string("\033[1m" + path + ":"
	         + row + ":" + col
	         + ": \033[95mwarning:\033[39m "
			 + msg
			 + "\033[0m"));
}

static inline void show_repeat_warning(const std::string& path, const ConfigToken& t)
	{std::cerr << _get_warning_string(path, t, "repeated directive ('" + t.word() + "')\n");}
static inline const std::string& get_word(const std::list<ConfigToken>::const_iterator& x)
	{return ((*x).word());}
static inline const t_token& get_type(const std::list<ConfigToken>::const_iterator& x)
	{return ((*x).type());}
static inline const std::string& get_word(const std::list<Token>::const_iterator& x)
	{return ((*x).word());}
static inline const t_token& get_type(const std::list<Token>::const_iterator& x)
	{return ((*x).type());}

#endif // CONFIG_TOKEN_HPP
