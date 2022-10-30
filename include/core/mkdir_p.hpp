/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mkdir_p.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/30 15:00:22 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/30 15:01:27 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MKDIR_P_HPP
# define MKDIR_P_HPP

# include "core.hpp"

# include <cstdlib>
# include <sys/stat.h>

/// @return 0 on success, 1 on failure
int mkdir_p(const char *path);

#endif // MKDIR_P_HPP
