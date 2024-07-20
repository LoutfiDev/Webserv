/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaji <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 10:19:40 by soulang           #+#    #+#             */
/*   Updated: 2024/07/20 16:44:02 by anaji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "location.hpp"
#include <cstddef>
#include <map>
#include <string>

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
			if (!locations.size())
				throw "directive \"server\" has no location";
			if (host.empty() || port.empty())
				throw "directive \"server\" must have \"listen\" directive";
			return ;
		}
		else if (open_brace)
			pick_directive(rest);
		else
			throw "directive \"server\" has no opening \"{\"";
	}
	if (!close_brace)
		throw "unexpected end of file, expecting \"}\"";
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
	std::map<std::string, Location *>::iterator it = locations.begin();

	while (it != locations.end())
	{
		delete it->second;
		break;
		it++;
	}
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
	error_pages["504"] = "error_pages/504.html";
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
			throw "host not found in \"listen\" directive";
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
				throw "port not found in \"listen\" directive";
			rest++;
		}
	}
	if (!(nbr > 0 && nbr < 10000))
		throw "port not found in \"listen\" directive";

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
				throw "invalid parameter in \"listen\" directive";
		}
	}
	if (host.empty() || port.empty())
		throw "\"listen\" directive accept only [host:port] as parameter";
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
		throw "invalid number of arguments in \"server_name\" directive";
}

std::string is_status_code(std::string status_code)
{
	char *rest;
	int nb;
	nb = strtod(status_code.c_str(), &rest);
	if (rest[0])
		throw "invalid value in \"error_page\" directive";
	else if (!(nb > 299 && nb < 600))
		throw "invalid value in \"error_page\" directive";
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
			throw "invalid number of arguments in \"error_page\" directive";
		std::vector<std::string>::iterator it = tmp.begin();
		for (; it+1 != tmp.end(); ++it)
			tmp_error_pages[is_status_code(*it)] = *tmp.rbegin();
	}
	if (tmp_error_pages.size() == 0)
		throw "invalid number of arguments in \"error_page\" directive";
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
		throw "invalid value in \"client_max_body_size\" directive";
	else if (nb > std::numeric_limits<unsigned int>::max() || nb < 0)
		throw "invalid value in \"client_max_body_size\" directive";
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
				throw "\"location\" directive must have root";
		}
	}
	if (max_body_size.empty())
		throw "invalid number of arguments in \"client_max_body_size\" directive";
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
				throw "invalid number of arguments in \"root\" directive";
		}
	}
	if (root.empty())
		throw "invalid number of arguments in \"root\" directive";
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
				throw "invalid number of arguments in \"location\" directive";
		}
		if (path.empty())
			throw "invalid number of arguments in \"location\" directive";
		else
		{
			if (locations.find(path) != locations.end())
				throw "\"server\" directive dosn't accept duplicated \"location\" directive";
			locations[path] = new Location(rest);
			if (locations[path]->root.empty())
			{
				if (!root.empty())
					locations[path]->root = root;
				else
				{
					if (!locations[path]->redirection.size())
						throw "\"location\" directive must have root";
				}

			}
			if (!locations[path]->allow_methods.size())
				locations[path]->allow_methods.push_back("GET");
			break;
		}
	}
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
	throw "unknown directive";
}
