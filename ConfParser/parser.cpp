/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soulang <soulang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 09:04:45 by soulang           #+#    #+#             */
/*   Updated: 2024/04/26 08:54:06 by soulang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.hpp"

int line = 1;
int col = 1;

Parser::Parser()
{
}

Parser::Parser(std::string fileName)
{
	std::ifstream 	inputFile(fileName.c_str());
	std::string tmp;
	
	if (!inputFile.is_open())
	{
		std::cout << "Error: opening Configuration file failed" << std::endl;
		return ;
	}
	
	while (std::getline(inputFile, tmp))
	{	
		if (tmp == "" && line++)
			continue;
		try
		{
			std::string key;
			std::string rest;
			std::stringstream ss(tmp);
			std::getline(ss,key,' ');
			std::getline(ss,rest,'\n');
			if (key == "server")
				servers.push_back(Server(inputFile, rest));
			else
				throw 1;
		}
		catch(const int e)
		{
			switch (e)
			{
				case 1:
					error(fileName, line, "");
					break;
				
				default:
					break;
			}
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