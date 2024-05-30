/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaji <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 08:53:41 by soulang           #+#    #+#             */
/*   Updated: 2024/05/29 23:53:48 by anaji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser/parser.hpp"
#include "Response/Response.hpp"
#include "./AI_part/ServerManager.hpp"
#include <cstdio>
#include <cstdlib>

// NOTE: set the path for your response on the AI_part/Client.cpp:96
// check the value of requested path in Ai_part/request.cpp:192 (variable named finale_path). is this the value that you want use in your response constructor?
// i did make my path attribute public so just change the value of it to use it in the Client class
// POST almost done (i think)

#define PORT 8080
int main (int ac, char **av)
{
	std::string fileName = "Conf/default.conf";
	if (ac <= 2)
	{
		if (av[1])
			fileName = av[1];
		Parser parser(fileName);
		ServerManager server;
		server.start(parser);

		// int server_fd, new_socket;
		// struct sockaddr_in address;
		// int addrlen = sizeof(address);
		// // Creating socket file descriptor
		// if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
		// {
		// 	perror("In socket");
		// 	exit(EXIT_FAILURE);
		// }
		// 
		//
		// address.sin_family = AF_INET;
		// address.sin_addr.s_addr = INADDR_ANY;
		// address.sin_port = htons( PORT );
		// 
		// memset(address.sin_zero, '\0', sizeof address.sin_zero);
		// 
		// 
		// if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
		// {
		// 	perror("In bind");
		// 	exit(EXIT_FAILURE);
		// }
		// if (listen(server_fd, 10) < 0)
		// {
		// 	perror("In listen");
		// 	exit(EXIT_FAILURE);
		// }
		// while(1)
		// {
		// 	printf("\n+++++++ Waiting for new connection ++++++++\n\n");
		// 	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
		// 	{
		// 		perror("In accept");
		// 		exit(EXIT_FAILURE);
		// 	}
		// 	
		// 	char buffer[30000] = {0};
		// 	read( new_socket , buffer, 30000);
		// 	printf("%s\n",buffer);
		// 	Response res(parser.get_servers()[0]->locations[0]);
		// 	write(new_socket, res.response.c_str(), strlen(res.response.c_str()));
		// 	// printf("------------------Hello message sent-------------------");
		// 	close(new_socket);
		// }

	}
	else
	{
		std::cout << av[0] << \
			" accept 1 param (Configuration file) only!" << std::endl;
	}
	return (0);
}
