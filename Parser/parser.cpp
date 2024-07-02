/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaji <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 09:04:45 by soulang           #+#    #+#             */
/*   Updated: 2024/06/22 23:44:11 by anaji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.hpp"

Parser::Parser()
{
}

Parser::Parser(std::string fileName)
{
	Sed::execute(fileName, "	", "\n", " ");
	std::ifstream 	inputFile("tmp.conf");
	std::string rest;
	
	if (!inputFile.is_open())
		throw "Error: opening Configuration file failed";
	std::getline(inputFile, rest);	
	try
	{
		while(!rest.empty())
		{
			if (rest[0] == ' ')
			{
				rest.erase(0,1);
				continue;
			}
			else if (rest.substr(0, 6) == "server")
			{
				rest = rest.substr(6);
				servers.push_back(new Server(rest));
			}
			else
				throw "unknown directive";
		}
	}
	catch(const char* e)
	{
		error(fileName, e);
		exit(0);
	}
	
	inputFile.close();
}

Parser::Parser(const Parser& copy)
{
	*this = copy;
}

Parser& Parser::operator=(const Parser& src)
{
	if (this == &src)
		return (*this);
	servers = src.servers;
	return (*this);
}

Parser::~Parser()
{
}


std::vector<Server*> Parser::get_servers( void ) {
	return (servers);
}


void Parser::error(std::string fileName, std::string msg)
{
	std::cout << "[error]: " << msg << " in ../" << fileName << std::endl;
}
