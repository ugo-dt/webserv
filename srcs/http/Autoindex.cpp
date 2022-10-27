/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Autoindex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 12:13:58 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/27 12:29:30 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Autoindex.hpp"

Autoindex::Autoindex()
{
}

Autoindex::~Autoindex()
{
}

std::string
get_index(const std::string& dir_path)
{
	(void)dir_path;
	return (std::string("index"));
}
