/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 22:38:24 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/24 22:52:40 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

Config::Config(int argc, const char **argv)
	: _path(),
	  _token_list()
{
}

Config::~Config()
{
	if (_path)
		free(_path);
}
