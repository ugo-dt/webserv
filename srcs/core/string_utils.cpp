/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/30 14:56:32 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/11/02 10:23:47 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "string_utils.hpp"

const std::string	get_post_header_value(const std::string& header, const std::string& arg)
{
	std::string			result;
	std::istringstream	_iss(header);
	std::string			_token;

	while (std::getline(_iss, _token, ';'))
	{
		if (_token.compare(0, arg.length(), arg) == 0)
		{
			result.assign(_token, arg.length(), _token.length() - arg.length());
			break ;
		}
	}
	return (result);
}

const std::string	get_header_field(const std::string& line)
{
	std::string	ret;

	size_t	i = line.find_first_of(':');
	if (i != std::string::npos)
		ret.append(line, 0 , i);
	else
		return ("");
	capitalize(ret);
	return (strip(ret, ' '));
}

const std::string	get_header_value(const std::string& line)
{
	size_t i;
	std::string	ret;

	i = line.find_first_of(':');
	i = line.find_first_not_of(' ', i + 1);
	if (i != std::string::npos)
		ret.append(line, i, std::string::npos);
	return (strip(ret, ' '));
}

std::string	str_to_upper(std::string str)
{
	std::transform(str.begin(), str.end(),str.begin(), ::toupper);
	return str;
}

std::string	str_to_lower(std::string str)
{
	std::transform(str.begin(), str.end(),str.begin(), ::tolower);
	return str;
}

std::string&	capitalize(std::string& str)
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

std::string&	strip(std::string& str, char c)
{
	size_t	i;

	if (str.empty())
		return str;
	i = str.size();
	while (i && str[i - 1] == c)
		i--;
	str.resize(i);
	for (i = 0; str[i] == c; i++);
	str = str.substr(i, std::string::npos);
	return str;
}

std::string	to_string(ssize_t n)
{
	std::stringstream tmp;
	tmp << n;
	return tmp.str();
}
