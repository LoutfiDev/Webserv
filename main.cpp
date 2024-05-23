/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soulang <soulang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 08:53:41 by soulang           #+#    #+#             */
/*   Updated: 2024/05/23 22:58:02 by soulang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser/parser.hpp"
#include "Response/Response.hpp"


#define PORT 8080
int main (int ac, char **av)
{
	if (ac == 2)
	{
		std::string fileName = av[1];
		Parser parser(fileName);


	}
	else 
		std::cout << av[0] << \
			" accept 1 param (Configuration file) only!" << std::endl;
	return (0);
}
	
	
	
	
		// int server_fd, new_socket;
		// struct sockaddr_in address;
		// int addrlen = sizeof(address);
		// // Creating socket file descriptor
		// if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
		// {
		// 	perror("In socket");
		// 	exit(EXIT_FAILURE);
		// }
		

		// address.sin_family = AF_INET;
		// address.sin_addr.s_addr = INADDR_ANY;
		// address.sin_port = htons( PORT );
		
		// memset(address.sin_zero, '\0', sizeof address.sin_zero);
		
		
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
			
		// 	char buffer[30000] = {0};
		// 	read( new_socket , buffer, 30000);
		// 	printf("%s\n",buffer);
		// 	// char response[100] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 12\r\n\r\nHello world!";
		// 	// write(new_socket, response, strlen(response));
		// 	Response res;
		// 	write(new_socket, res.response.c_str(), strlen(res.response.c_str()));
		// 	// printf("------------------Hello message sent-------------------");
		// 	close(new_socket);
		// }
