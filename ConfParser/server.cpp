/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soulang <soulang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 10:19:40 by soulang           #+#    #+#             */
/*   Updated: 2024/04/29 11:30:17 by soulang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

Server::Server()
{
}

Server::Server(std::stringstream& ss) : port (-1)
{
	int open_brace = 0, close_brace = 0;
	std::string tmp;
	while (std::getline(ss, tmp, ' '))
	{
		if (tmp.empty())
			continue;
		if (tmp == "{" && !open_brace && !open_brace++)
			continue;
		else if (tmp == "}" && !close_brace && !close_brace++)
			break;
		else if (open_brace)
			pick_directive(tmp, ss);
		else
			throw 2;
	}
	if (!close_brace)
		throw 4;
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
void Server::set_listen(std::stringstream& ss) {
	std::string value;
	std::string tmp;
	char *rest;
	int semi_colomn = 0;
	
	while (!semi_colomn && std::getline(ss, value, ' '))
	{
		if (value.empty())
			continue;
		if (value == ";" && !host.empty() && port > 1)
			return;
		std::stringstream s(value);
		while (std::getline(s, tmp, ':'))
		{
			if (host.empty())
				host = parse_host(tmp);
			else if (port == -1)
			{
				port = strtod(tmp.c_str(), &rest);
				if ((!rest[0] && (port > 0 && port < 10000)) || \
				    ((rest[0] && !strncmp(rest, ";", strlen(rest)) && !semi_colomn && !semi_colomn++) \
				    && (port > 0 && port < 10000)))
		            break;
				else
					throw 7;
			}
			else
			    throw 8;
		}
	}
}

void Server::set_server_names(std::stringstream& ss) { (void)ss; }

void Server::set_error_pages(std::stringstream& ss) { (void)ss; }

void Server::set_max_body_size(std::stringstream& ss) { (void)ss; }

void Server::set_root(std::stringstream& ss) { (void)ss; } 

void Server::set_locations(std::stringstream& ss) { (void)ss; }

// Server getters
std::string Server::get_host( void ) const { return host; }

unsigned int Server::get_port( void ) const {return port; }

std::string Server::get_server_names(const std::string& server_name) {
	std::vector<std::string>::iterator it = server_names.begin();
	for(; it != server_names.end() && *it != server_name ; it++)
		;
	if (it == server_names.end())
	    return ("");
	return (*it);
}

std::string Server::get_error_pages(const unsigned int error_code) {
	std::string path = "";
	if (error_pages.find(error_code) != error_pages.end())
		path = error_pages[error_code];
	return (path);
}

unsigned int Server::get_max_body_size( void ) const { return max_body_size; }

std::string Server::get_root( void ) const { return root;}

Location *Server::get_locations(const std::string& path) {
	Location *tmp = NULL;
	if (locations.find(path) != locations.end())
		*tmp = locations[path];
	return (tmp);
}

//Additional memberFunc

void Server::pick_directive(std::string key, std::stringstream& ss)
{
	std::string str[6] = {"listen", "server_name", "error_page", "client_max_body_size", "root", "location"};
	void (Server::* ptr[6]) (std::stringstream &) = {&Server::set_listen, &Server::set_server_names, &Server::set_error_pages \
		, &Server::set_max_body_size, &Server::set_root, &Server::set_locations};
	for (int i = 0; i < 6; i++)
	{
		if (str[i] == key)
			return ((this->*(ptr[i]))(ss));
	}
	throw 3;
}

std::string Server::parse_host(std::string host) {
	
	std::string tmp;
	std::stringstream ss(host);
	int nbr;
	char *rest;
	int i = 1;
	
	while (std::getline(ss, tmp, '.'))
	{
		nbr = strtod(tmp.c_str(), &rest);
		if (rest[0] || !(nbr >= 0 && nbr < 256) || i > 4)
			throw 9;
		i++;
	}
	
	return host;
}