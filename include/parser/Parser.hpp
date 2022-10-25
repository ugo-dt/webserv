/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 11:37:31 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/25 11:45:14 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

# include <string>
# include <vector>

template <class Token>
class Parser
{
protected:
	std::vector<Token> _token_list;

public:
	Parser();
	virtual	~Parser();
};

class ConfigParser
	: public Parser<int>
{
private:
	std::string	_path;

public:
	ConfigParser(int argc, char **argv);
	~ConfigParser();
};

#endif // PARSER_HPP
