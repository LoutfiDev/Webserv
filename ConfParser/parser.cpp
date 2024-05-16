/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soulang <soulang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 09:04:45 by soulang           #+#    #+#             */
/*   Updated: 2024/05/16 12:20:18 by soulang          ###   ########.fr       */
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
	{
		std::cout << "Error: opening Configuration file failed" << std::endl;
		return ;
	}
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
				throw 1;
		}
	}
	catch(const int e)
	{
		switch (e)
		{
			case 1: 
				error(fileName, rest, "unknown directive");
				exit(0);
			case 2: 
				error(fileName, rest, "directive \"server\" has no opening \"{\"");
				exit(0);
			case 3: 
				error(fileName, rest, "unexpected end of file, expecting \"}\"");
				exit(0);
			case 4: 
				error(fileName, rest, "directive \"location\" has no opening \"{\"");
				exit(0);
			case 5: 
				error(fileName, rest, "host not found in \"listen\" directive");
				exit(0);
			case 6: 
				error(fileName, rest, "port not found in \"listen\" directive");
				exit(0);
			case 7: 
				error(fileName, rest, "invalid parameter in \"listen\" directive");
				exit(0);
			case 8: 
				error(fileName, rest, "\"listen\" directive accept only [host:port] as parameter");
				exit(0);
			case 9: 
				error(fileName, rest, "invalid number of arguments in \"server_name\" directive");
				exit(0);
			default:
				std::cout << "default" << std::endl;
		}
	}
	
	inputFile.close();
}

Parser::Parser(const Parser& copy)
{
	(void)copy;
}

Parser& Parser::operator=(const Parser& src)
{
	(void)src;
	return (*this);
}

Parser::~Parser()
{
}


std::vector<Server*> Parser::get_servers( void ) {
	return (servers);
}


void Parser::error(std::string fileName, std::string rest, std::string msg)
{
	std::ifstream 	inputFile(fileName.c_str());
	std::string value;
	std::string tmp;
	int line = 1;

	std::stringstream ss(rest);
	std::getline(ss, value, ' ');
	while (std::getline(inputFile, tmp))
	{
		if (tmp.find(value, 0) != std::string::npos)
				break;
		line++;
	}
	std::cout << "[error]: " << msg << " in ../" << fileName << ":" << line << std::endl;
}