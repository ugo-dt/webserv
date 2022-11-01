/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 16:56:39 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/11/01 14:10:24 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRING_UTILS_HPP
# define STRING_UTILS_HPP

# include <algorithm>
# include <string>
# include <sstream>

const std::string	get_post_header_value(const std::string& header, const std::string& arg);
const std::string	get_header_field(const std::string& line);
const std::string	get_header_value(const std::string& line);
std::string			str_to_upper(std::string str);
std::string			str_to_lower(std::string str);
std::string&		capitalize(std::string& str);
std::string&		strip(std::string& str, char c);
std::string			to_string(ssize_t n);

#endif // STRING_UTILS_HPP
