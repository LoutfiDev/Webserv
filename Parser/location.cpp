/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soulang <soulang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 10:20:30 by soulang           #+#    #+#             */
/*   Updated: 2024/06/14 20:16:38 by soulang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "location.hpp"

Location::Location() {}

Location::Location(std::string& rest)
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
			throw "directive \"location\" has no opening \"{\"";
	}
	if (!close_brace)
		throw "unexpected end of file, expecting \"}\"";
}

Location::Location(const Location& copy) { *this = copy; }

Location& Location::operator=(const Location& src)
{
	if (this == &src)
		return (*this);
	autoindex = src.autoindex;
	allow_methods = src.allow_methods;
	root = src.root;
	index = src.index;
	upload_dir = src.upload_dir;
	cgi = src.cgi;
	redirection = src.redirection;
	return *this;
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
		throw "invalid value in \"autoindex\" directive";
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
				throw "invalid number of arguments in \"autoindex\" directive";
		}
	}
	if (nb == -1)
		throw "invalid number of arguments in \"autoindex\" directive";
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
				throw "invalid number of arguments in \"root\" directive";
		}
	}
	if (root.empty())
		throw "invalid number of arguments in \"root\" directive";
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
			throw "invalid value in \"allowed_methods\" directive";
	}
	for (int i = 0; i < 3; i++)
	{
		if (str[i] == value)
			return (value);
	}
	throw "invalid value in \"allowed_methods\" directive";
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
		throw "invalid number of arguments in \"allowed_methods\" directive";
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
		throw "invalid number of arguments in \"index\" directive";
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
				throw "invalid number of arguments in \"upload_dir\" directive";
		}
	}
	if (upload_dir.empty())
		throw "invalid number of arguments in \"upload_dir\" directive";
}

std::string	parse_extention(std::map<std::string, std::string> map, std::string value)
{
	std::string str[3] = {".php", ".py", ".sh"};
	std::string::iterator it = value.begin();
	for(; it != value.end(); ++it) { *it =  std::tolower(*it); }
	std::map<std::string, std::string>::iterator ite = map.begin();
	for (; ite != map.end(); ++ite)
	{
		if (value == ite->first)
			throw "invalid value in \"cgi\" directive";
	}
	for (int i = 0; i < 3; i++)
	{
		if (str[i] == value)
			return value;
	}
	throw "invalid value in \"cgi\" directive";
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
				throw "invalid number of arguments in \"cgi\" directive";
		}
		if (!path.empty() && !extention.empty())
			cgi[extention] = path;
	}
	if (cgi.size() == 0)
		throw "invalid number of arguments in \"cgi\" directive";
}

std::string	parse_status_code(std::string value)
{
	char *rest;
	int nb;
	
	nb = strtod(value.c_str(), &rest);
	if (rest[0])
		throw "invalid value in \"return\" directive";
	else if (!(nb > 299 && nb < 600))
		throw "invalid value in \"return\" directive";
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
				throw "invalid number of arguments in \"return\" directive";
		}
		if (!path.empty() && !status_code.empty())
		{
			if (redirection.size())
				throw "directive \"location\" accept on \"return\" directive";
			redirection[status_code] = path;
		}
	}
	if (redirection.size() == 0)
		throw "invalid number of arguments in \"return\" directive";
}



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
	throw "unknown directive";
}