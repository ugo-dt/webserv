/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 22:27:48 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/24 22:52:13 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <string>
# include <vector>

typedef enum e_token
{
	token_none,
	token_word,
	token_brace_open,
	token_brace_close,
	token_newline,
	token_semicolon
}Token;

class Config
{
private:
	std::string        *_path;
	std::vector<Token> _token_list;

public:
	Config(int argc, const char **argv);
	~Config(void);
};

#endif // CONFIG_HPP
