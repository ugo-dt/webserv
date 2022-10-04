/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 15:50:20 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/04 14:57:42 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

Config::Config(const char *path)
{
	if (path)
		_config_path = path;
	else
		_config_path = DEFAULT_CONFIG_PATH;
}

Config::~Config()
{
}

const std::string&
Config::config_path(void) const
{
	return _config_path;
}
