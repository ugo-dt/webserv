/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 22:48:48 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/25 23:27:47 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_PARSER_HPP
# define CONFIG_PARSER_HPP

# include <fstream>
# include <sys/stat.h>
# include <vector>

# define DEFAULT_CONFIG_PATH	"config/default"

class ConfigParser
{
private:
	std::vector<size_t>	_token_list;
	std::string			_path;
	std::fstream		_file;

	void _open_file();
	void _close_file();
	void _tokenize();
	void _parse();

public:
	ConfigParser(int argc, const char **argv);
	~ConfigParser();
};

#endif // CONFIG_PARSER_HPP
