/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soulang <soulang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 10:17:16 by soulang           #+#    #+#             */
/*   Updated: 2024/04/23 12:36:49 by soulang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <map>

#include "location.hpp"

class Location;

class Server
{
	private:
		std::map<Location, std::string> locations;
		
	public:
		Server();
		Server(std::ifstream& inputFile);
		Server(const Server& copy);
		Server& operator=(const Server& src);
		~Server();
};

#endif