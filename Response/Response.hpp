/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soulang <soulang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 09:30:42 by soulang           #+#    #+#             */
/*   Updated: 2024/06/07 11:36:38 by soulang          ###   ########.fr       */
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
		
		std::string method;
		std::string path;
		std::string uri;
		std::string http_v;
		std::string status_code;

		
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
