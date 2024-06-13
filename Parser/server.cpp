/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaji <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 10:19:40 by soulang           #+#    #+#             */
/*   Updated: 2024/06/13 10:45:23 by anaji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

Server::Server() {}

Server::Server(std::string& rest)
{
	int open_brace = 0, close_brace = 0;

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

Server::Server(const Server& copy) { *this = copy; }

Server& Server::operator=(const Server& src)
{
	if (this == &src)
		return (*this);
	host = src.host;
	port = src.port;
	server_names = src.server_names;
	error_pages = src.error_pages;
	max_body_size = src.max_body_size;
	root = src.root;
	locations = src.locations;
	return (*this);
}

Server::~Server() {
}

void Server::set_default_error_pages( void ){
	error_pages["201"] = "error_pages/201.html";
	error_pages["204"] = "error_pages/204.html";
	error_pages["400"] = "error_pages/400.html";
	error_pages["403"] = "error_pages/403.html";
	error_pages["404"] = "error_pages/404.html";
	error_pages["405"] = "error_pages/405.html";
	error_pages["408"] = "error_pages/408.html";
	error_pages["413"] = "error_pages/413.html";
	error_pages["414"] = "error_pages/414.html";
	error_pages["415"] = "error_pages/415.html";
	error_pages["500"] = "error_pages/500.html";
	error_pages["501"] = "error_pages/501.html";
	error_pages["505"] = "error_pages/505.html";
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

std::string is_status_code(std::string status_code)
{
	char *rest;
	int nb;
	nb = strtod(status_code.c_str(), &rest);
	if (rest[0])
		throw 11;
	else if (!(nb > 299 && nb < 600))
		throw 11;
	return (status_code);
}
void Server::set_error_pages(std::string& rest) { 

	std::string value;
	std::vector<std::string>tmp;
	std::map<std::string, std::string> tmp_error_pages;

	set_default_error_pages();
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
		if (tmp.size() < 2)
			throw 10;
		std::vector<std::string>::iterator it = tmp.begin();
		for (; it+1 != tmp.end(); ++it)
			tmp_error_pages[is_status_code(*it)] = *tmp.rbegin();
	}
	if (tmp_error_pages.size() == 0)
		throw 10;
	std::map<std::string, std::string>::iterator ite = tmp_error_pages.begin();
	for (; ite != tmp_error_pages.end(); ++ite)
		error_pages[ite->first] = ite->second;
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
			if (!locations[path]->allow_methods.size())
				locations[path]->allow_methods.push_back("GET");
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
