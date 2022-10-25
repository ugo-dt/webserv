/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 11:37:31 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/25 23:28:45 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

# include "ConfigParser.hpp"
# include "RequestParser.hpp"
# include <string>
# include <vector>

typedef enum e_token
{
	token_none,
	token_word,

	// config file
	token_open_brace,
	token_close_brace,
	token_newline,
	token_semicolon,

	// requests
	token_colon
}t_token;

#endif // PARSER_HPP
