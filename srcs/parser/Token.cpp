/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 10:57:55 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/26 10:58:18 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Token.hpp"

Token::Token(void)
	: _type(token_none),
	  _word(""),
	  _row(0),
	  _col(0)
{
}

Token::Token(t_token t, const std::string &s, unsigned int row, unsigned int col)
	: _type(t),
	  _word(s),
	  _row(row),
	  _col(col + 1)
{
}

Token::Token(const Token &x)
	: _type(x._type),
	  _word(x._word),
	  _row(x._row),
	  _col(x._col)
{
}

Token::~Token() {}

const Token	&Token::operator=(const Token &x)
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
operator<<(std::ostream &o, const Token &t)
{
	o << t.word();
	return (o);
}

const std::string & Token::word(void) const
	{return _word;}
const t_token & Token::type(void) const  
	{return _type;}
unsigned int Token::row(void) const
	{return _row;}
unsigned int Token::col(void) const
	{return _col;}
