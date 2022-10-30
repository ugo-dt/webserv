/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 16:56:39 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/30 14:59:49 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRING_UTILS_HPP
# define STRING_UTILS_HPP

# include <algorithm>
# include <string>
# include <sstream>

std::string		str_to_upper(std::string str);
std::string		str_to_lower(std::string str);
std::string&	capitalize(std::string& str);
std::string&	strip(std::string& str, char c);
std::string		to_string(ssize_t n);

#endif // STRING_UTILS_HPP
