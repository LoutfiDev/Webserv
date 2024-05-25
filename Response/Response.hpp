/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaji <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 09:30:42 by soulang           #+#    #+#             */
/*   Updated: 2024/05/25 19:42:41 by anaji            ###   ########.fr       */
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
#include <sys/socket.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <map>
#include <vector>
#include <sys/stat.h>

#include "../AI_part/Request.hpp"

#define PORT 8080

class Response
{
	private:
		
	public:
		std::map<std::string, std::string>messages;
		std::string response;
		std::string body_path;
		
		std::string method;
		std::string path;
		std::string http_v;
		std::string status_code;
		
		Response();
		Response(const Response& copy);
		Response& operator=(const Response& src);
		~Response();
		

		std::string Get();
		std::string Post();
		std::string Delete();
		
		
		std::string getMessage(std::string code);
		std::string getContentLenght(std::string file);
		std::string getContentType(std::string file);
		std::string pick_method(std::string method);
		std::string form_response( void );
		void fill_messages( void );		
};

#endif
