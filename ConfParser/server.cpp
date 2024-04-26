/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soulang <soulang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 10:19:40 by soulang           #+#    #+#             */
/*   Updated: 2024/04/26 11:49:55 by soulang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

Server::Server()
{
}

Server::Server(std::ifstream& inputFile, std::string rest)
{
	std::string tmp;
	std::string key;
	std::stringstream ss(rest);
	while (std::getline(ss, key, ' '))
	{
		
	}
	while (std::getline(inputFile, tmp))
	{
		if (tmp == "" && line++)
			continue;
		
		line++;
	}
}

Server::Server(const Server& copy)
{
	(void)copy;
}

Server& Server::operator=(const Server& src)
{
	(void)src;
	return (*this);
}

Server::~Server()
{
}

// Server setters
void Server::set_host(const std::string& _host) { host = _host; }

void Server::set_port(const unsigned int _port) { port = _port; }

void Server::set_server_names(const std::string& _server_name) { server_names.push_back(_server_name); }

void Server::set_error_pages(const unsigned int error_code, std::string& path) { server_names[error_code] = path; }

void Server::set_max_body_size(const unsigned int size) { max_body_size = size; }

void Server::set_root(const std::string& path) { root = path; }

void Server::set_locations(const std::string& path, const Location location) { locations[path] = location; }

// Server getters
std::string Server::get_host( void ) const { return host; }

unsigned int Server::get_port( void ) const {return port; }

std::string Server::get_server_names(const std::string& server_name) const {
	std::vector<std::string>::const_iterator it = server_names.begin();
	for(; it != server_names.end() && *it != server_name ; it++)
		;
	if (it == server_names.end())
	    return ("");
	return (*it);
}

std::string Server::get_error_pages(const unsigned int error_code) const {}
unsigned int Server::get_max_body_size( void ) const { return max_body_size; }
std::string Server::get_root( void ) const { return root;}
Location Server::get_locations(const std::string& path) const {}