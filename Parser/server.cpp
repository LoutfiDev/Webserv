/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soulang <soulang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 10:19:40 by soulang           #+#    #+#             */
/*   Updated: 2024/05/25 11:40:56 by soulang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

Server::Server() {}

Server::Server(std::string& rest)
{
	int open_brace = 0, close_brace = 0;
	set_default_error_pages();
	
	while (!rest.empty())
	{
	    if (rest[0] == ' ')
		{
			rest.erase(0,1);
			continue;
		}
		else if (rest[0] == '{' && !open_brace && !open_brace++)
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
	if (!close_brace)
		throw 3;
}

Server::Server(const Server& copy) { (void)copy; }

Server& Server::operator=(const Server& src)
{
	(void)src;
	return (*this);
}

Server::~Server() {}

void Server::set_default_error_pages( void ){
	default_error_pages["201"] = "error_pages/201.html";
	default_error_pages["204"] = "error_pages/204.html";
	default_error_pages["400"] = "error_pages/400.html";
	default_error_pages["403"] = "error_pages/403.html";
	default_error_pages["404"] = "error_pages/404.html";
	default_error_pages["405"] = "error_pages/405.html";
	default_error_pages["408"] = "error_pages/408.html";
	default_error_pages["413"] = "error_pages/413.html";
	default_error_pages["414"] = "error_pages/414.html";
	default_error_pages["415"] = "error_pages/415.html";
	default_error_pages["500"] = "error_pages/500.html";
	default_error_pages["501"] = "error_pages/501.html";
	default_error_pages["505"] = "error_pages/505.html";
}
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
			throw 5;
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
				throw 6;
			rest++;
		}
	}
	if (!(nbr > 0 && nbr < 10000))
		throw 6;

	return port;
}

void Server::set_listen(std::string& rest) {
	
	std::string value;
	    
	while (!rest.empty())
	{
		if (rest[0] == ' ')
		{
			rest.erase(0, 1);
			continue;
		}
		if (rest[0] == ';')
		{
			rest.erase(0, 1);
			break;
		}
		std::stringstream s(rest);
		std::getline(s, value, ';');
		rest.erase(0, value.size());
		std::stringstream ss(value);
		while (std::getline(ss, value, ':'))
		{
			if (host.empty())
				host = parse_host(value);
			else if (port.empty())
				port = parse_port(value);
			else
				throw 7;
		}
	}
	if (host.empty() || port.empty())
		throw 8;
}

void Server::set_server_names(std::string& rest) { 
	
	std::string value;
	    
	while (!rest.empty())
	{
		if (rest[0] == ' ')
		{
			rest.erase(0, 1);
			continue;
		}
		if (rest[0] == ';')
		{
			rest.erase(0, 1);
			break;
		}
		std::stringstream s(rest);
		std::getline(s, value, ';');
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
		throw 9;
}

std::vector<std::string> is_status_code(std::vector<std::string> status_codes)
{
	char *rest;
	int nb;

	if (status_codes.size() < 2)
		throw 10;
	std::vector<std::string>::iterator it = status_codes.begin();
	for (; it+1 != status_codes.end(); ++it)
	{
		nb = strtod((*it).c_str(), &rest);
		if (rest[0])
			throw 11;
		else if (!(nb > 299 && nb < 600))
			throw 11;
	}
	status_codes.erase(it);
	return (status_codes);
}
void Server::set_error_pages(std::string& rest) { 
	
	std::string value;
	std::vector<std::string>tmp;
	    
	while (!rest.empty())
	{
		if (rest[0] == ' ')
		{
			rest.erase(0, 1);
			continue;
		}
		else if (rest[0] == ';')
		{
			rest.erase(0, 1);
			break;
		}
		std::stringstream s(rest);
		std::getline(s, value, ';');
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
		throw 10;
}

std::string parse_max_body_size(std::string value)
{
	double nb;
	char *rest;
	
	nb = strtod(value.c_str(), &rest);
	if (rest[0])
		throw 13;
	else if (nb > std::numeric_limits<unsigned int>::max() || nb < 0)
		throw 13;
	return value;
}

void Server::set_max_body_size(std::string& rest) { 
	std::string value;
	    
	while (!rest.empty())
	{
		if (rest[0] == ' ')
		{
			rest.erase(0, 1);
			continue;
		}
		else if (rest[0] == ';')
		{
			rest.erase(0, 1);
			break;
		}
		std::stringstream s(rest);
		std::getline(s, value, ';');
		rest.erase(0, value.size());
		std::stringstream ss(value);
		while (std::getline(ss, value, ' '))
		{
			if (value.empty())
				continue;
			else if (max_body_size.empty())
				max_body_size = parse_max_body_size(value);
			else
				throw 12;
		}
	}
	if (max_body_size.empty())
		throw 12;
}

void Server::set_root(std::string& rest) { 
	std::string value;
	    
	while (!rest.empty())
	{
		if (rest[0] == ' ')
		{
			rest.erase(0, 1);
			continue;
		}
		else if (rest[0] == ';')
		{
			rest.erase(0, 1);
			break;
		}
		std::stringstream s(rest);
		std::getline(s, value, ';');
		rest.erase(0, value.size());
		std::stringstream ss(value);
		while (std::getline(ss, value, ' '))
		{
			if (value.empty())
				continue;
			else if (root.empty())
				root = value;
			else
				throw 14;
		}
	}
	if (root.empty())
		throw 14;
}	 

void Server::set_locations(std::string& rest) { 
	std::string value;
	std::string path;
	
	while (!rest.empty())
	{
		if (rest[0] == ' ')
		{
			rest.erase(0,1);
			continue;
		}
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
				throw 15;
		}
		if (path.empty())
			throw 15;
		else
		{
			locations[path] = new Location(rest);
			if (locations[path]->root.empty())
				locations[path]->root = root;
			break;
		}
	}
}

// Server getters
// std::string Server::get_host( void ) const { return host; }

// std::string Server::get_port( void ) const {return port; }

// std::string Server::get_server_names(const std::string& server_name) {}

// std::string Server::get_error_pages(const unsigned int error_code) {}
 
// std::string Server::get_max_body_size( void ) const { return max_body_size; }

// std::string Server::get_root( void ) const { return root;}

// Location *Server::get_locations(const std::string& path) {}

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
	throw 1;
}