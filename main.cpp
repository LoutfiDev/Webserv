/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaji <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 08:53:41 by soulang           #+#    #+#             */
/*   Updated: 2024/07/20 14:33:58 by anaji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request/utils.hpp"
#include "Parser/parser.hpp"
#include "Response/Response.hpp"
#include "./Request/ServerManager.hpp"
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <iostream>

int main (int ac, char **av)
{
	signal(SIGPIPE, SIG_IGN);
	std::string fileName = "Conf/default.conf";
	if (ac <= 2)
	{
		if (av[1])
			fileName = av[1];
		try {
			Parser parser(fileName);
			ServerManager server;
			server.start(parser);
		}
		catch (char const *str)
		{
			std::cout << str << "\n";
			std::exit(1);
		}
	}
	else
	{
		std::cout << av[0] << \
			" accept 1 param (Configuration file) only!" << std::endl;
	}
	return (0);
}
