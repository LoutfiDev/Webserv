/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soulang <soulang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 10:17:16 by soulang           #+#    #+#             */
/*   Updated: 2024/04/26 11:45:02 by soulang          ###   ########.fr       */
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
		unsigned int port;
		std::vector<std::string>server_names;
		std::map<unsigned int, std::string> error_pages;
		unsigned int max_body_size;
		std::string root;
		std::map<std::string, Location> locations;
		
	public:
		Server();
		Server(std::ifstream& inputFile, std::string rest);
		Server(const Server& copy);
		Server& operator=(const Server& src);
		~Server();
		
		void set_host(const std::string& host);
		void set_port(const unsigned int port);
		void set_server_names(const std::string& server_name);
		void set_error_pages(const unsigned int error_code, std::string& path);
		void set_max_body_size(const unsigned int size);
		void set_root(const std::string& path);
		void set_locations(const std::string& path, const Location location);
		
		std::string get_host( void ) const ;
		unsigned int get_port( void ) const ;
		std::string get_server_names(const std::string& server_name) const ;
		std::string get_error_pages(const unsigned int error_code) const ;
		unsigned int get_max_body_size( void ) const ;
		std::string get_root( void ) const ;
		Location get_locations(const std::string& path) const ;
};

#endif