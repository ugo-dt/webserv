/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 15:50:20 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/09/21 16:40:27 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include <iostream>

Config::Config(std::string path)
	: _config_file(path)
{
	std::cout << "File path: " << _config_file << std::endl;
}



Config::~Config()
{
}
