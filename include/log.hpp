/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 10:41:17 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/31 17:24:59 by ugdaniel         ###   ########.fr       */
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
#  define WS_INFO_LOG(msg)			print_log("\033[96m[INFO] ", msg)
#  define WS_WARN_LOG(msg)			print_log("\033[93m[WARNING] ", msg)
#  define WS_ERROR_LOG(msg)			print_log("\033[91m[ERROR] ", msg)
#  define WS_VALUE_LOG(msg, val)	print_value(msg, val)
# else
#  define WS_INFO_LOG(msg)
#  define WS_WARN_LOG(msg)
#  define WS_ERROR_LOG(msg)
#  define WS_VALUE_LOG(msg, val)
# endif

static inline void print_log(const char *type, const char *msg)
	{std::cerr << type << msg << "\033[0m" << std::endl;}
static inline void print_log(const char *type, const std::string& msg)
	{std::cerr << type << msg << "\033[0m" << std::endl;}
static inline void print_value(const std::string& name, const std::string& s)
	{std::cerr << "\033[95m" << name << ": " << s << "\033[0m" << std::endl;}
static inline void print_value(const std::string& name, size_t val)
	{std::cerr << "\033[95m" << name << ": " << val << "\033[0m" << std::endl;}

#endif // LOG_HPP
