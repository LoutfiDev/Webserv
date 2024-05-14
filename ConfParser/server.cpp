/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soulang <soulang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 10:19:40 by soulang           #+#    #+#             */
/*   Updated: 2024/05/12 15:58:32 by soulang          ###   ########.fr       */
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
	if (*rest)
	{
		while (*rest)
		{
			if (*rest != ' ')
				return "";
			rest++;
		}
	}
	if (!(nbr > 0 && nbr < 10000))
		return "";

	return port;
}

void Server::set_listen(std::string& rest) {
	
	std::string value;
	    
	while (!rest.empty())
	{
		while (rest[0] == ' ')
			rest.erase(0, 1);
		if (rest[0] == ';')
		{
			rest.erase(0, 1);
			break;
		}
		std::stringstream s(rest);
		std::getline(s, value, ';');
		if (value.empty())
			throw 44;
		rest.erase(0, value.size());
		std::stringstream ss(value);
		while (std::getline(ss, value, ':'))
		{
			if (host.empty())
				host = parse_host(value);
			else if (port.empty())
				port = parse_port(value);
			else
				throw 1;
		}
	}
	if (host.empty() || port.empty())
		throw 33;
}

void Server::set_server_names(std::string& rest) { 
	
	std::string value;
	    
	while (!rest.empty())
	{
		while (rest[0] == ' ')
			rest.erase(0, 1);
		if (rest[0] == ';')
		{
			rest.erase(0, 1);
			break;
		}
		std::stringstream s(rest);
		std::getline(s, value, ';');
		if (value.empty())
			throw 55;
		rest.erase(0, value.size());
		std::stringstream ss(value);
		while (std::getline(ss, value, ' '))
		{
			if (value.empty())
				continue;
			else
				server_names.push_back(value);
		}
	}
	if (server_names.size() == 0)
		throw 66;
}


std::vector<std::string> is_status_code(std::vector<std::string> status_codes)
{
	char *rest;
	int nb;

	if (status_codes.size() < 2)
		throw 90;
	std::vector<std::string>::iterator it = status_codes.begin();
	for (; it+1 != status_codes.end(); ++it)
	{
		nb = strtod((*it).c_str(), &rest);
		if (rest[0])
			throw 70;
		else if (!(nb > 299 && nb < 600))
			throw 80;
	}
	status_codes.erase(it);
	return (status_codes);
}
void Server::set_error_pages(std::string& rest) { 
	
	std::string value;
	std::vector<std::string>tmp;
	    
	while (!rest.empty())
	{
		while (rest[0] == ' ')
			rest.erase(0, 1);
		if (rest[0] == ';')
		{
			rest.erase(0, 1);
			break;
		}
		std::stringstream s(rest);
		std::getline(s, value, ';');
		if (value.empty())
			throw 55;
		rest.erase(0, value.size());
		std::stringstream ss(value);
		while (std::getline(ss, value, ' '))
		{
			if (value.empty())
				continue;
			else
				tmp.push_back(value);
		}
		error_pages[is_status_code(tmp)] = *tmp.rbegin();
	}
	if (error_pages.size() == 0)
		throw 66;
}

std::string parse_max_body_size(std::string value)
{
	double nb;
	char *rest;
	
	nb = strtod(value.c_str(), &rest);
	if (rest[0])
		throw 70;
	else if (nb > std::numeric_limits<unsigned int>::max() || nb < 0)
		throw 80;
	return value;
}

void Server::set_max_body_size(std::string& rest) { 
	std::string value;
	    
	while (!rest.empty())
	{
		while (rest[0] == ' ')
			rest.erase(0, 1);
		if (rest[0] == ';')
		{
			rest.erase(0, 1);
			break;
		}
		std::stringstream s(rest);
		std::getline(s, value, ';');
		if (value.empty())
			throw 50;
		rest.erase(0, value.size());
		std::stringstream ss(value);
		while (std::getline(ss, value, ' '))
		{
			if (value.empty())
				continue;
			else if (max_body_size.empty())
				max_body_size = parse_max_body_size(value);
			else
				throw 40;
		}
	}
	if (max_body_size.empty())
		throw 30;
}

void Server::set_root(std::string& rest) { 
	std::string value;
	    
	while (!rest.empty())
	{
		while (rest[0] == ' ')
			rest.erase(0, 1);
		if (rest[0] == ';')
		{
			rest.erase(0, 1);
			break;
		}
		std::stringstream s(rest);
		std::getline(s, value, ';');
		if (value.empty())
			throw 50;
		rest.erase(0, value.size());
		std::stringstream ss(value);
		while (std::getline(ss, value, ' '))
		{
			if (value.empty())
				continue;
			else if (root.empty())
				root = value;
			else
				throw 40;
		}
	}
	if (root.empty())
		throw 30;
}	 

void Server::set_locations(std::string& rest) { 
	std::string value;
	std::string path;
	
	while (!rest.empty())
	{
		while (rest[0] == ' ')
		rest.erase(0,1);
	
		std::stringstream s(rest);
		std::getline(s, value, '{');
		rest.erase(0, value.size());
		std::stringstream ss(value);
		while (std::getline(ss, value, ' '))
		{
			if (value.empty())
				continue;
			else if (path.empty())
				path = value;
			else
				throw 2;
		}
		if (path.empty())
			throw 1;
		else
		{
			locations[path] = Location(rest);
			break;
		}
	}
}

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
	(void)error_code;
	std::string path = "";
	// if (error_pages.find(error_code) != error_pages.end())
	// 	path = error_pages[error_code];
	return (path);
}
 
std::string Server::get_max_body_size( void ) const { return max_body_size; }

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