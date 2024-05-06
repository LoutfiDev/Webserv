/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soulang <soulang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 10:19:40 by soulang           #+#    #+#             */
/*   Updated: 2024/05/06 15:54:21 by soulang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

Server::Server() {}

Server::Server(std::string& rest)
{
	int open_brace = 0, close_brace = 0;
	
	while (!rest.empty())
	{
	    while (rest[0] == ' ')
			rest.erase(0,1);
		if (rest[0] == '{' && !open_brace && !open_brace++)
			rest.erase(0, 1);
		else if (rest[0] == '}' && open_brace && !close_brace)
		{
			rest.erase(0, 1);
			return ;
		}
		else if (open_brace)
		    pick_directive(rest);
		else
			throw 2;
	}
}

Server::Server(const Server& copy) { (void)copy; }

Server& Server::operator=(const Server& src)
{
	(void)src;
	return (*this);
}

Server::~Server() {}

// Server setters
std::string parse_host(std::string host) {
	
	std::string tmp;
	std::stringstream ss(host);
	int nbr;
	char *rest;
	int i = 1;
	
	while (std::getline(ss, tmp, '.'))
	{
		nbr = strtod(tmp.c_str(), &rest);
		if (rest[0] || !(nbr >= 0 && nbr < 256) || i > 4)
			return "";
		i++;
	}
	
	return host;
}

std::string parse_port(std::string port) {
	
	char *rest;
	int nbr;
	
	nbr = strtod(port.c_str(), &rest);
	if (rest[0] || !(nbr > 0 && nbr < 10000))
		return "";

	return port;
}

void Server::set_listen(std::string& rest) {
// 	std::string value;
// 	std::string tmp;
// 	char *rest;

// 	while (!semi_colon && std::getline(ss, value, ' '))
// 	{
// 		if (value.empty())
// 			continue;
// 		else if (value == ";" && !host.empty() && port > 1)
// 			return;
// 		std::stringstream s(value);
// 		while (std::getline(s, tmp, ':'))
// 		{
// 			if (host.empty())
// 				host = parse_host(tmp);
// 			else if (port == -1)
// 			{
// 				port = strtod(tmp.c_str(), &rest);
// 				if ((!rest[0] && (port > 0 && port < 10000)) || 
// 				    ((rest[0] && !strncmp(rest, ";", strlen(rest)) && !semi_colon && !semi_colon++) 
// 				    && (port > 0 && port < 10000)))
// 		            break;
// 				else
// 					throw 7;
// 			}
// 			else
// 			    throw 8;
// 		}
// 	}
	// std::string value;
	// std::string rest;
	// while (!semi_colon && std::getline(ss, value, ' '))
	// {
	// 	if (value.empty())
	// 		continue;
	// 	else if (value == ";" && !semi_colon && !semi_colon++)
	// 		break;
	// 	else if (value[0] == ';' && !semi_colon && !semi_colon++)
	// 	{
	// 		rest = value.erase(0);
	// 		break;
	// 	}
	// 	else if (value[value.size() - 1] == ';' && !semi_colon && !semi_colon++)
	// 		rest = value.erase(value.size() - 1);
			
	// 	std::stringstream s(value);
	// 	while (std::getline(s, value, ':'))
	// 	{
	// 		if (host.empty())
	// 			host = parse_host(value);
	// 		else if (port.empty())
	// 			port = parse_port(value);
	// 		else
	// 			throw 8;
	// 	}	
	// }
	// if (host.empty() || port.empty())
	// 	throw 33;
}

void Server::set_server_names(std::string& rest) { 
	(void)rest;
	// std::string value;
	// std::cout << "hello from server name" << std::endl;
	// exit(0);
	// int semi_colon = 0;
	// while (!semi_colon && std::getline(ss, value, ' '))
	// {
	// 	if (value.empty())
	// 		continue;
	// 	else if (value == ";" && server_names.size() > 0)
	// 		return;
	// 	else if (*(--value.end()) == ';' && !semi_colon && !semi_colon++)
	// 		server_names.push_back(value.erase(value.size() - 1));
	// 	else
	// 		server_names.push_back(value);
	// }
	// if (!semi_colon)
	// 	throw 10;
}

void Server::set_error_pages(std::string& rest) { (void)rest; }

void Server::set_max_body_size(std::string& rest) { (void)rest; }

void Server::set_root(std::string& rest) { (void)rest; } 

void Server::set_locations(std::string& rest) { (void)rest; }

// Server getters
std::string Server::get_host( void ) const { return host; }

std::string Server::get_port( void ) const {return port; }

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
void Server::pick_directive(std::string& rest)
{
	std::string str[6] = {"listen", "server_name", "error_page", "client_max_body_size", "root", "location"};
	void (Server::* ptr[6]) (std::string &) = {&Server::set_listen, &Server::set_server_names, &Server::set_error_pages \
		, &Server::set_max_body_size, &Server::set_root, &Server::set_locations};
	std::string tmp;
	std::stringstream ss(rest);
    std::getline(ss, tmp, ' ');
	for (int i = 0; i < 6; i++)
	{
		if (str[i] == tmp)
		{
			rest = rest.erase(0, tmp.size());
			return ((this->*(ptr[i]))(rest));
		}
	}
	throw 3;
}