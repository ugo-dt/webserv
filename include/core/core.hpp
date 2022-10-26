/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 22:02:45 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/25 23:42:33 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CORE_HPP
# define CORE_HPP

# include "http.hpp"
# include "parser.hpp"

# include <iostream>
# include <stdexcept>
# include <string>

// throws an exception with the error string correspondig to errno
inline
void _throw_errno(const std::string& msg)
{
	throw std::runtime_error(std::string(msg + ": " + strerror(errno)));
}

#endif // CORE_HPP
