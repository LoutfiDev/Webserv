/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soulang <soulang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 09:04:45 by soulang           #+#    #+#             */
/*   Updated: 2024/05/17 15:16:17 by soulang          ###   ########.fr       */
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
				error(fileName, "unknown directive");
				exit(0);
			case 2: 
				error(fileName, "directive \"server\" has no opening \"{\"");
				exit(0);
			case 3: 
				error(fileName, "unexpected end of file, expecting \"}\"");
				exit(0);
			case 4: 
				error(fileName, "directive \"location\" has no opening \"{\"");
				exit(0);
			case 5: 
				error(fileName, "host not found in \"listen\" directive");
				exit(0);
			case 6: 
				error(fileName, "port not found in \"listen\" directive");
				exit(0);
			case 7: 
				error(fileName, "invalid parameter in \"listen\" directive");
				exit(0);
			case 8: 
				error(fileName, "\"listen\" directive accept only [host:port] as parameter");
				exit(0);
			case 9: 
				error(fileName, "invalid number of arguments in \"server_name\" directive");
				exit(0);
			case 10: 
				error(fileName, "invalid number of arguments in \"error_page\" directive");
				exit(0);
			case 11: 
				error(fileName, "invalid value in \"error_page\" directive");
				exit(0);
			case 12: 
				error(fileName, "invalid number of arguments in \"client_max_body_size\" directive");
				exit(0);
			case 13: 
				error(fileName, "invalid value in \"client_max_body_size\" directive");
				exit(0);
			case 14: 
				error(fileName, "invalid number of arguments in \"root\" directive");
				exit(0);
			case 15: 
				error(fileName, "invalid number of arguments in \"location\" directive");
				exit(0);
			case 16: 
				error(fileName, "invalid number of arguments in \"autoindex\" directive");
				exit(0);
			case 17: 
				error(fileName, "invalid value in \"autoindex\" directive");
				exit(0);
			case 18: 
				error(fileName, "invalid number of arguments in \"allowed_methods\" directive");
				exit(0);
			case 19: 
				error(fileName, "invalid value in \"allowed_methods\" directive");
				exit(0);
			case 20: 
				error(fileName, "invalid number of arguments in \"index\" directive");
				exit(0);
			case 21: 
				error(fileName, "invalid number of arguments in \"upload_dir\" directive");
				exit(0);
			case 22: 
				error(fileName, "invalid number of arguments in \"cgi\" directive");
				exit(0);
			case 23: 
				error(fileName, "invalid value in \"cgi\" directive");
				exit(0);
			case 24: 
				error(fileName, "invalid number of arguments in \"return\" directive");
				exit(0);
			case 25: 
				error(fileName, "invalid value in \"return\" directive");
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


void Parser::error(std::string fileName, std::string msg)
{
	std::cout << "[error]: " << msg << " in ../" << fileName << std::endl;
}