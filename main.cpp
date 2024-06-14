/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaji <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 08:53:41 by soulang           #+#    #+#             */
/*   Updated: 2024/06/14 18:26:59 by anaji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser/parser.hpp"
#include "Response/Response.hpp"
#include "./AI_part/ServerManager.hpp"
#include <csignal>
#include <cstdio>
#include <cstdlib>

// NOTE: if double free occure then check Client.cpp:14 (copy vector Server) Client.cpp:152 (delete Server)
// NOTE: the closing of sockets when user press CTR-C (swl kamal ila ban lik)
// NOTE: DELETE aborts sometimes.
// QUESTION: do we need a body when showing and error (like 404 NOT found), because you are just setting that on headers only
// NOTE: do some test too

int main (int ac, char **av)
{
	signal(SIGPIPE, SIG_IGN);
	std::string fileName = "Conf/default.conf";
	if (ac <= 2)
	{
		if (av[1])
			fileName = av[1];
		Parser parser(fileName);
		ServerManager server;
		server.start(parser);
	}
	else
	{
		std::cout << av[0] << \
			" accept 1 param (Configuration file) only!" << std::endl;
	}
	return (0);
}
