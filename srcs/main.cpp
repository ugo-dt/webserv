/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/03 19:43:26 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/09/03 20:09:52 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

int	main(int argc, char **argv)
{
	if (argc > 2)
	{
		std::cerr << "webserv: too many arguments" << std::endl;
		return (1);
	}
	if (argc == 1)
		std::cout << "default" << std::endl; // default path for config file
	else
	{
		std::cout << argv[1] << std::endl; // given path for config file
	}

	// config
	// run
	// clear
	return (0);
}
