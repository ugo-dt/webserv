/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 22:02:45 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/27 12:30:35 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CORE_HPP
# define CORE_HPP

# include "log.hpp"
# include <iostream>
# include <stdexcept>
# include <string>
# include <sys/types.h>

// throws an exception with msg followed by the error string correspondig to errno
static inline void _throw_errno(const std::string& msg)
	{throw std::runtime_error(msg + ": " + strerror(errno));}

#endif // CORE_HPP
