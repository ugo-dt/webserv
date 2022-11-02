/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 10:41:17 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/11/02 09:57:46 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOG_HPP
# define LOG_HPP

# include <iostream>

# ifdef DEBUG
#  define WS_LOG 1
# endif

/* webserv log */
# if defined(WS_LOG)
#  define WS_INFO_LOG(msg)			std::cout << "\033[96m[INFO] " << msg << "\033[0m" << std::endl
#  define WS_WARN_LOG(msg)			std::cout << "\033[93m[WARNING] " << msg << "\033[0m" << std::endl
#  define WS_ERROR_LOG(msg)			std::cout << "\033[91m[ERROR] " << msg << "\033[0m" << std::endl
# else
#  define WS_INFO_LOG(msg)
#  define WS_WARN_LOG(msg)
#  define WS_ERROR_LOG(msg)
# endif

#endif // LOG_HPP
