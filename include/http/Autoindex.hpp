/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Autoindex.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 12:11:55 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/27 12:14:11 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUTOINDEX_HPP
# define AUTOINDEX_HPP

# include "http.hpp"
# include <string>

class Autoindex
{
private:
public:
	Autoindex();
	~Autoindex();

	const std::string&	get_index(const std::string& dir_path);
};

#endif // AUTOINDEX_HPP
