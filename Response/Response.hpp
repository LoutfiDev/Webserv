/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soulang <soulang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 09:30:42 by soulang           #+#    #+#             */
/*   Updated: 2024/07/10 12:30:57 by soulang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <iomanip>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>
#include <fstream>
#include <cstdlib>
#include <limits>
#include <string>
#include <sys/socket.h>
#include <algorithm>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/param.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <map>
#include <vector>
#include <sys/stat.h>
#include <cstddef>
#include <netdb.h>
#include <sstream>


#include "../Parser/parser.hpp"
#include "../AI_part/utils.hpp"

#define EXEC_CGI 0
#define CGI_PROCESSING 1
#define HEADER_PROCESSING 2
#define BODY_PROCESSING 3

// post method states
#define PROCESSING 1
#define SENDING 2
#define END 3

#define CGITIMEOUT 5

class Response
{
		
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
		
		std::string cgiFile;
		std::string cgiOut;
		std::string cgiErr;
		std::string extension;
		std::string cgiPath;
		std::string contentType;
		std::string query;
		std::string http_cookie;
		std::multimap<std::string, std::string>cgi_headers;
		
		pid_t pid;
		FILE* in;
		FILE* out;
		FILE* err;
		int status;
		char **env;
		char **argv;
		time_t timespan;

		
		int execute_cgi( void );
		int is_cgi( void );
		int isValid ( void );
		void formEnv ( void );
		bool istimeOut();
		void  resetTimer();

		//added by Ai for post method
		int postState;
		std::string responseBody;
		std::ifstream infile;
		std::ofstream outfile;
		int typeInfile;
		std::string fileExtension;

		int processPostResponse();
		
		Response();
		Response(const Response& copy);
		Response& operator=(const Response& src);
		~Response();
		

		void Get( void );
		void Post( void );
		void Delete( void );
		
		
		void fill_messages( void );	
		std::string getPath ( void );	
		void pick_method( void );
		int send_response( void );
	
		void Delete_folder(std::string path);
		std::string getMessage(std::string code);
		std::string getContentLenght(std::string file);
		std::string getContentType(std::string file);

		// to send an error response
		void send_errorResponse();

};

#endif
