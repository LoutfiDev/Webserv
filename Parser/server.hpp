/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soulang <soulang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 10:17:16 by soulang           #+#    #+#             */
/*   Updated: 2024/05/24 10:28:18 by soulang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <limits>
#include <map>
#include <vector>

#include "location.hpp"

class Location;

class Server
{
	private:
		
	public:
		std::string host;
		std::string port;
		std::vector<std::string>server_names;
		std::map<std::string, std::string> default_error_pages;
		std::map<std::vector<std::string>, std::string> error_pages;
		std::string max_body_size;
		std::string root;
		std::map<std::string, Location*> locations;
		
		Server();
		Server(std::string& rest);
		Server(const Server& copy);
		Server& operator=(const Server& src);
		~Server();
		

		void set_listen(std::string& rest);
		void set_server_names(std::string& rest);
		void set_default_error_pages( void );
		void set_error_pages(std::string& rest);
		void set_max_body_size(std::string& rest);
		void set_root(std::string& rest);
		void set_locations(std::string& rest);
		
		// std::string get_host( void ) const;
		// std::string get_port( void ) const;
		// std::string get_server_names(const std::string& server_name);
		// std::string get_error_pages(const unsigned int error_code);
		// std::string get_max_body_size( void ) const;
		// std::string get_root( void ) const;
		// Location *get_locations(const std::string& path);


		void pick_directive(std::string& key);
};

#endif