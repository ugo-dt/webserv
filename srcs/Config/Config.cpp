/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 15:50:20 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/06 14:53:06 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

Config::Config(const char *filename)
	: _config_path(filename)
{
}

Config::~Config()
{
}

const std::string&
Config::config_path(void) const
{
	return _config_path;
}
