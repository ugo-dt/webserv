/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/03 19:43:26 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/09/22 15:47:11 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int	main(int argc, char **argv)
{
	if (argc > 2)
	{
		std::cerr << "webserv: too many arguments" << std::endl;
		return (1);
	}

	// config
	if (argv[1])
		Config	config(argv[1]);
	else
		Config	config(DEFAULT_CONFIG_PATH);

	// parse
	// run
	// clear
	return (0);
}
