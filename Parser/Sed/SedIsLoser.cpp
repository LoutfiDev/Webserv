/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SedIsLoser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soulang <soulang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 12:01:01 by yloutfi           #+#    #+#             */
/*   Updated: 2024/05/21 11:33:52 by soulang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SedIsLoser.hpp"

Sed::Sed()
{
}

Sed::~Sed()
{
}

void Sed::execute (std::string fileName, std::string s1, std::string s2, std::string s3)
{
	std::ifstream 	myFile(fileName.c_str());
	std::ofstream 	myFileCopy("tmp.conf");
	std::string		tmp;
	std::size_t 	pos;
	
	if (!myFile.is_open() || !myFileCopy.is_open())
		throw 26;
	while (1)
	{
		pos = 0;
		std::getline(myFile, tmp);
		while (1) 
		{
			pos = tmp.find(s1, pos);
			if (pos == std::string::npos)
					break;
			tmp.erase (pos, s1.length());
			tmp.insert (pos, s3);
			pos += s3.length();
		}
		
		pos = 0;
		while (1) 
		{
			pos = tmp.find(s2, pos);
			if (pos == std::string::npos)
					break;
			tmp.erase (pos, s2.length());
			tmp.insert (pos, s3);
			pos += s3.length();
		}
		
		myFileCopy << tmp << " ";
		if (myFile.eof())
			break;
	}
		
	myFile.close();
	myFileCopy.close();
}
