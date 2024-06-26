/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 10:57:55 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/28 13:57:31 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Token.hpp"

ConfigToken::ConfigToken(void)
	: Token(token_none, ""),
	  _row(0),
	  _col(0)
{
}

ConfigToken::ConfigToken(t_token type, const std::string &word, unsigned int row, unsigned int col)
	: Token(type, word),
	  _row(row),
	  _col(col + 1)
{
}

ConfigToken::ConfigToken(const ConfigToken &x)
	: Token(x._type, x._word),
	  _row(x._row),
	  _col(x._col)
{
	_type = x._type;
	_word = x._word;
}

ConfigToken::~ConfigToken() {}

const ConfigToken	&ConfigToken::operator=(const ConfigToken &x)
{
	if (this != &x)
	{
		_type = x._type;
		_word = x._word;
		_row = x._row;
		_col = x._col;
	}
	return *this;
}

std::ostream&
operator<<(std::ostream &o, const ConfigToken &t)
{
	o << t.word();
	return (o);
}

unsigned int ConfigToken::row(void) const
	{return _row;}
unsigned int ConfigToken::col(void) const
	{return _col;}
