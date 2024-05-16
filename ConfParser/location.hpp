/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soulang <soulang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 10:17:20 by soulang           #+#    #+#             */
/*   Updated: 2024/05/16 15:44:35 by soulang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <iostream>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <sstream>
#include <cstdlib>

#include "parser.hpp"

class Location
{
	private:
	
		bool autoindex;
		std::vector<std::string>allow_methods;
		std::string root;
		std::vector<std::string>index;
		std::string upload_dir;
		std::map<std::string, std::string>cgi;
		std::map<std::string, std::string>redirection;
		
	public:
	
		Location();
		Location(std::string& rest);
		Location(const Location& copy);
		Location& operator=(const Location& src);
		~Location();
		
		void set_autoindex(std::string& rest);
		void set_root(std::string& rest);
		void set_allow_methods(std::string& rest);
		void set_index(std::string& rest);
		void set_upload_dir(std::string& rest);
		void set_cgi(std::string& rest);
		void set_redirection(std::string& rest);
		

		void pick_directive(std::string& rest);
};

#endif