/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soulang <soulang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 10:20:30 by soulang           #+#    #+#             */
/*   Updated: 2024/05/14 12:54:12 by soulang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "location.hpp"

Location::Location()
{
}


Location::Location(std::string& rest){
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

Location::Location(const Location& copy)
{
	(void)copy;
}

Location& Location::operator=(const Location& src)
{
	(void)src;
	return (*this);
}

Location::~Location()
{
}


int parse_value(std::string value)
{
	std::string::iterator it = value.begin();
	for(; it != value.end(); ++it) { *it =  std::tolower(*it); }
	if (value == "on")
		return (1);
	else if (value == "off")
		return (0);
	else
		throw 4;
}

void Location::set_auto_index(std::string& rest){
	std::string value;
	int nb = -1;

	while (!rest.empty())
	{
		while (rest[0] == ' ')
			rest.erase(0,1);
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
			else if (nb < 1)
				nb = parse_value(value);
			else
				throw 2;
		}
	}
	if (nb == -1)
		throw 3;
	autoindex = nb;
}

void Location::set_root(std::string& rest){
	// 		std::cout << "hello" << std::endl;
	// std::cout << "|" << rest << "|" << std::endl;
	// exit(0);
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
			throw 55;
	}
	for (int i = 0; i < 3; i++)
	{
		if (str[i] == value)
			return (value);
	}
	throw 3;
}

void Location::set_allow_methods(std::string& rest){
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
		throw 66;
}
void Location::set_index(std::string& rest){
	(void)rest;
}
void Location::set_upload_dir(std::string& rest){
	(void)rest;
}
void Location::set_cgi(std::string& rest){
	(void)rest;
}
void Location::set_redirection(std::string& rest){
	(void)rest;
}


void Location::pick_directive(std::string& rest)
{
	std::string str[7] = {"autoindex", "root", "allow_methods", "index", "upload_dir", "cgi", "return"};
	void (Location::* ptr[7]) (std::string &) = {&Location::set_auto_index, &Location::set_root, &Location::set_allow_methods \
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
	throw 3;
}