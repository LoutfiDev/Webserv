/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soulang <soulang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 09:04:45 by soulang           #+#    #+#             */
/*   Updated: 2024/05/06 15:47:02 by soulang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.hpp"

int line = 1;
int semi_colon = 0;

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
			while (rest[0] == ' ')
				rest.erase(0,1);
			if (rest.substr(0, 6) == "server")
			{
				rest = rest.substr(6);

				servers.push_back(Server(rest));
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
				std::cout << "no server block" << std::endl;
				exit(0);
			case 2: 
				std::cout << "open brace" << std::endl;
				exit(0);
			case 3: 
				std::cout << "no matching directives" << std::endl;
				exit(0);
			case 4: 
				std::cout << "close brace" << std::endl;
				exit(0);
			case 9: 
				std::cout << "error in host" << std::endl;
				exit(0);
			case 7:
				std::cout << "error in port" << std::endl;
				exit(0);
			case 8:
				std::cout << "error in listen directive" << std::endl;
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

void Parser::error(std::string fileName, int line, std::string msg)
{
	std::ifstream 	inputFile(fileName.c_str());
	std::string tmp;
	int i = 0;
	
	while (std::getline(inputFile, tmp))
	{
		if (i++ != line)
			continue;
	}
	std::cout << fileName << ":" << line << ": error: " << msg << std::endl;
	std::cout << "\t" << line << " |" << tmp << std::endl;
}