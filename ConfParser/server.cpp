/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soulang <soulang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 10:19:40 by soulang           #+#    #+#             */
/*   Updated: 2024/04/23 12:38:37 by soulang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

Server::Server()
{
}

Server::Server(std::ifstream& inputFile)
{
	std::string tmp;

	while (std::getline(inputFile, tmp))
	{
		std::cout << tmp << std::endl;
		exit(0);
	}
}

Server::Server(const Server& copy)
{
	(void)copy;
}

Server& Server::operator=(const Server& src)
{
	(void)src;
	return (*this);
}

Server::~Server()
{
}