/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 10:41:17 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/26 15:08:15 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef LOG_H
# define LOG_H

# include <iostream>

# ifdef DEBUG
#  define WS_LOG 1
# endif

/* webserv log */
# if defined(WS_LOG)
#  define WS_INFO_LOG(msg)			print_log("\e[96m[INFO] ", msg)
#  define WS_WARN_LOG(msg)			print_log("\e[93m[WARNING] ", msg)
#  define WS_ERROR_LOG(msg)			print_log("\e[91m[ERROR] ", msg)
#  define WS_VALUE_LOG(msg, val)	print_value(msg, val)
# else
#  define WS_INFO_LOG(msg)
#  define WS_WARN_LOG(msg)
#  define WS_ERROR_LOG(msg)
#  define WS_VALUE_LOG(msg, val)
# endif

static inline void print_log(const char *type, const char *msg)
	{std::cerr << type << msg << "\e[0m" << std::endl;}
static inline void print_log(const char *type, const std::string& msg)
	{std::cerr << type << msg << "\e[0m" << std::endl;}
static inline void print_value(const std::string& name, const std::string& s)
	{std::cerr << "\e[95m" << name << ": " << s << "\e[0m" << std::endl;}
static inline void print_value(const std::string& name, size_t val)
	{std::cerr << "\e[95m" << name << ": " << val << "\e[0m" << std::endl;}

#endif // LOG_H
