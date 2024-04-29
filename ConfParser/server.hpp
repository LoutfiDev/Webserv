/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soulang <soulang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 10:17:16 by soulang           #+#    #+#             */
/*   Updated: 2024/04/29 10:07:21 by soulang          ###   ########.fr       */
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
#include <map>
#include <vector>

#include "location.hpp"

class Location;

class Server
{
	private:
		std::string host;
		int port;
		std::vector<std::string>server_names;
		std::map<unsigned int, std::string> error_pages;
		unsigned int max_body_size;
		std::string root;
		std::map<std::string, Location> locations;
		
	public:
		Server();
		Server(std::stringstream& ss);
		Server(const Server& copy);
		Server& operator=(const Server& src);
		~Server();
		

		void set_listen(std::stringstream& ss);
		void set_server_names(std::stringstream& ss);
		void set_error_pages(std::stringstream& ss);
		void set_max_body_size(std::stringstream& ss);
		void set_root(std::stringstream& ss);
		void set_locations(std::stringstream& ss);
		
		std::string get_host( void ) const;
		unsigned int get_port( void ) const;
		std::string get_server_names(const std::string& server_name);
		std::string get_error_pages(const unsigned int error_code);
		unsigned int get_max_body_size( void ) const;
		std::string get_root( void ) const;
		Location *get_locations(const std::string& path);

		
		void pick_directive(std::string key, std::stringstream& ss);
		std::string parse_host(std::string host);
};

#endif