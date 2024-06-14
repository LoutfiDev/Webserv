/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soulang <soulang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 08:45:07 by yloutfi           #+#    #+#             */
/*   Updated: 2024/06/14 18:28:48 by soulang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>

#include "server.hpp"
#include "Sed/SedIsLoser.hpp"

class Server;

class Parser
{
	private:
		std::vector<Server*> servers;
		
	public:
		Parser();
		Parser(std::string fileName);
		Parser(const Parser& copy);
		Parser& operator=(const Parser& src);
		~Parser();
		
		std::vector<Server*> get_servers( void );
		void error(std::string fileName, std::string msg);
};

#endif
