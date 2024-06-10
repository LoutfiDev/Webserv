/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaji <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 09:30:42 by soulang           #+#    #+#             */
/*   Updated: 2024/06/10 04:14:41 by anaji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <limits>
#include <string>
#include <sys/socket.h>
#include <algorithm>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <map>
#include <vector>
#include <sys/stat.h>


#include "../Parser/parser.hpp"

#define CGIISDONE 3
#define HEADERISSENT 1
#define BODYISSENT 2

// post method states
#define PROCESSING 1
#define SENDING 2
#define END 3

#define BODY 1
#define FILE 2

class Response
{
	private:
		
	public:
		std::map<std::string, std::string>messages;
		std::string response;
		Location *location;
		Server *server;
		int socket;
		
		DIR *dir;
		char buffer[1024];
		int STAGE;
		unsigned int index;
		int HEADERISWRITTEN;
		
		std::string method;
		std::string path;
		std::string uri;
		std::string http_v;
		std::string status_code;

		//added by Ai for post method
		int postState;
		std::ifstream infile;
		std::ofstream outfile;
		int typeInfile;

		int processPostResponse();
		
		Response();
		Response(const Response& copy);
		Response& operator=(const Response& src);
		~Response();
		

		void Get( void );
		//Post Defintion at AI_Part/utils.cpp
		std::string responseBody;
		void Post( void );
		void Delete( void );
		
		
		void fill_messages( void );	
		std::string getPath ( void );	
		void pick_method(Location *location);
		int send_response( void );
	
		void Delete_folder(std::string path);
		std::string getMessage(std::string code);
		std::string getContentLenght(std::string file);
		std::string getContentType(std::string file);


		// to send an error response
		void send_errorResponse();


};

#endif
