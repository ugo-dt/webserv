/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 16:56:39 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/28 14:46:55 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRING_UTILS_HPP
# define STRING_UTILS_HPP

# include <algorithm>
# include <string>
# include <sstream>

static inline std::string	str_to_upper(std::string& str)
{
	std::transform(str.begin(), str.end(),str.begin(), ::toupper);
	return str;
}

static inline std::string	str_to_lower(std::string str)
{
	std::transform(str.begin(), str.end(),str.begin(), ::tolower);
	return str;
}

static inline std::string&	capitalize(std::string& str)
{
	size_t	i = 0;

	str_to_lower(str);
	str[i] = std::toupper(str[i]);
	while((i = str.find_first_of('-', i + 1)) != std::string::npos)
	{
		if (i + 1 < str.size())
		str[i + 1] = std::toupper(str[i + 1]);
	}
	return str;
}

static inline std::string&	strip(std::string& str, char c)
{
	size_t	i;

	if (!str.size())
		return str;
	i = str.size();
	while (i && str[i - 1] == c)
		i--;
	str.resize(i);
	for (i = 0; str[i] == c; i++);
	str = str.substr(i, std::string::npos);
	return str;
}

static inline std::string	to_string(ssize_t n)
{
	std::stringstream tmp;
	tmp << n;
	return tmp.str();
}

#endif // STRING_UTILS_HPP
