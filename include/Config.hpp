/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 15:42:33 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/08 12:12:36 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <fstream>
# include <string>

# define DEFAULT_CONFIG_PATH	"config/default"

class Config
{
private:
	std::string	_config_path;

	Config();

public:
	Config(const char *);
	~Config();

	const std::string&	config_path(void) const;
};

#endif // CONFIG_HPP
