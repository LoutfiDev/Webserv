/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaji <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 09:30:42 by soulang           #+#    #+#             */
/*   Updated: 2024/06/14 09:39:37 by anaji            ###   ########.fr       */
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
		
		std::string cgiFile;
		std::string cgiOut;
		std::string extension;
		std::string cgiPath;
		std::string query;
		std::string http_cookie;
		std::string SessionId;
		bool		isSessionIdSend;
		
		pid_t pid;
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

		std::map<std::string, std::string> response_map;


};

#endif
