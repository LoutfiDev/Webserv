/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soulang <soulang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 08:45:07 by yloutfi           #+#    #+#             */
/*   Updated: 2024/05/02 11:32:39 by soulang          ###   ########.fr       */
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

extern int line;
extern int semi_colon;

class Parser
{
	private:
		std::vector<Server> servers;
		
	public:
		Parser();
		Parser(std::string fileName);
		Parser(const Parser& copy);
		Parser& operator=(const Parser& src);
		~Parser();
		
		void error(std::string fileName, int line, std::string msg);
};

#endif
