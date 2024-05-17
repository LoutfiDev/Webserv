/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soulang <soulang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 10:20:30 by soulang           #+#    #+#             */
/*   Updated: 2024/05/16 20:19:08 by soulang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "location.hpp"

Location::Location() {}

Location::Location(std::string& rest){
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
			throw 4;
	}
	if (!close_brace)
		throw 3;
}

Location::Location(const Location& copy) { (void)copy; }

Location& Location::operator=(const Location& src)
{
	(void)src;
	return (*this);
}

Location::~Location() {}

// Location setters
int parse_value(std::string value)
{
	std::string::iterator it = value.begin();
	for(; it != value.end(); ++it) { *it =  std::tolower(*it); }
	if (value == "on")
		return (1);
	else if (value == "off")
		return (0);
	else
		throw 17;
}

void Location::set_autoindex(std::string& rest){
	std::string value;
	int nb = -1;

	while (!rest.empty())
	{
		if (rest[0] == ' ')
		{
			rest.erase(0,1);
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
			else if (nb < 1)
				nb = parse_value(value);
			else
				throw 16;
		}
	}
	if (nb == -1)
		throw 16;
	autoindex = nb;
}

void Location::set_root(std::string& rest){
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

std::string parse_methods(std::vector<std::string>v, std::string value)
{
	std::string str[3] = {"post", "get", "delete"};
	std::string::iterator it = value.begin();
	for(; it != value.end(); ++it) { *it =  std::tolower(*it); }
	std::vector<std::string>::iterator ite = v.begin();
	for (; ite != v.end(); ++ite)
	{
		if (value == *ite)
			throw 19;
	}
	for (int i = 0; i < 3; i++)
	{
		if (str[i] == value)
			return (value);
	}
	throw 19;
}

void Location::set_allow_methods(std::string& rest){
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
			else
				allow_methods.push_back(parse_methods(allow_methods, value));
		}
	}
	if (allow_methods.size() == 0)
		throw 18;
}

void Location::set_index(std::string& rest){
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
			else
				index.push_back(value);
		}
	}
	if (index.size() == 0)
		throw 20;
}

void Location::set_upload_dir(std::string& rest){
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
			else if (upload_dir.empty())
				upload_dir = value;
			else
				throw 21;
		}
	}
	if (upload_dir.empty())
		throw 21;
}

std::string	parse_extention(std::map<std::string, std::string> map, std::string value)
{
	std::string str[2] = {".php", ".py"};
	std::string::iterator it = value.begin();
	for(; it != value.end(); ++it) { *it =  std::tolower(*it); }
	std::map<std::string, std::string>::iterator ite = map.begin();
	for (; ite != map.end(); ++ite)
	{
		if (value == ite->first)
			throw 23;
	}
	for (int i = 0; i < 2; i++)
	{
		if (str[i] == value)
			return value;
	}
	throw 23;
}
void Location::set_cgi(std::string& rest){
	std::string value;
	std::string extention;
	std::string path;
	    
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
			else if (extention.empty())
				extention = parse_extention(cgi, value);
			else if (path.empty())
				path = value;
			else
				throw 22;
		}
		if (!path.empty() && !extention.empty())
			cgi[extention] = path;
	}
	if (cgi.size() == 0)
		throw 22;
}

std::string	parse_status_code(std::string value)
{
	char *rest;
	int nb;
	
	nb = strtod(value.c_str(), &rest);
	if (rest[0])
		throw 25;
	else if (!(nb > 299 && nb < 600))
		throw 25;
	return value;
}
void Location::set_redirection(std::string& rest){
	std::string value;
	std::string status_code;
	std::string path;
	    
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
			else if (status_code.empty())
				status_code = parse_status_code(value);
			else if (path.empty())
				path = value;
			else
				throw 24;
		}
		if (!path.empty() && !status_code.empty())
			redirection[status_code] = path;
	}
	if (redirection.size() == 0)
		throw 24;
}

// Location getters


//Additional memberFunc
void Location::pick_directive(std::string& rest)
{
	std::string str[7] = {"autoindex", "root", "allow_methods", "index", "upload_dir", "cgi", "return"};
	void (Location::* ptr[7]) (std::string &) = {&Location::set_autoindex, &Location::set_root, &Location::set_allow_methods \
		, &Location::set_index, &Location::set_upload_dir, &Location::set_cgi, &Location::set_redirection};
	std::string tmp;
	std::stringstream ss(rest);
    std::getline(ss, tmp, ' ');
	for (int i = 0; i < 7; i++)
	{
		if (str[i] == tmp)
		{
			rest = rest.erase(0, tmp.size());
			return ((this->*(ptr[i]))(rest));
		}
	}
	throw 1;
}