/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raw.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/30 15:13:23 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/11/02 10:46:36 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAW_HPP
# define RAW_HPP

# include "http.hpp"
# include "string_utils.hpp"

# include <string>
# include <fstream>

const std::string	get_file_contents(std::ifstream& f);
const std::string	get_body_from_uri(const std::string& uri);
const std::string	get_status_string(unsigned int code);
const std::string	get_raw_page(unsigned int code, const char *msg);
const std::string	get_raw_page(unsigned int code);

#endif // RAW_HPP
