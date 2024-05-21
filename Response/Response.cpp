/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soulang <soulang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 09:30:45 by soulang           #+#    #+#             */
/*   Updated: 2024/05/21 12:04:06 by soulang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"



Response::Response() : method("GET"), path("../web_root/index.html"), http_v("HTTP/1.1"), status_code("200")
{
	fill_messages();
	response = pick_method(method);
}

Response::Response(const Response& copy) { (void)copy; }

Response& Response::operator=(const Response& src)
{
	(void)src;
	return (*this);
}

Response::~Response() {}

std::string Response::Get() {
	if (opendir(path.c_str()))
	{
		if (path[path.size()] != '/')
		{
			// path.append("/");
			// status_code = 301;
		}
		else
		{
			// if (location.index)
			// {
			// 	//append index to path
			// 	return(form_response());
			// }
			// if (location.autoindex)
			// 	//list all files
			// else
			// 	status_code = 403;
		}
	}
	else
	{
		if (access(path.c_str(), F_OK) == 0)
		{
			if (access(path.c_str(), R_OK) != 0)
				status_code = "403";
		}
		else
			status_code = "404";
	}
	return (form_response());
}
std::string Response::Post() {}
std::string Response::Delete() {}

std::string Response::getMessage(std::string code)
{
	std::string message = "";
	if (messages.find(code) != messages.end())
		message = messages[code];
	return (message);
}

std::string Response::getContentLenght(std::string file)
{
	std::stringstream ss;  
	std::string size;
	std::ifstream in_file(file, std::ios::binary);
	in_file.seekg(0, std::ios::end);
	int file_size = in_file.tellg();
	ss << file_size;  
	ss >> size;
	return (size);
}

std::string Response::getContentType(std::string file)
{
	std::string tmp;
	std::string extention(file);
	extention.erase(0, extention.find('.') + 1);
	std::ifstream 	inputFile("mime.types");
	if (!inputFile.is_open())
		throw 26;
	while(std::getline(inputFile, tmp))
	{
		if (tmp.find(extention) != std::string::npos)
		{
			std::stringstream ss(tmp);
			while(std::getline(ss, tmp, ' '))
			{
				if (tmp.empty())
					continue;
				return (tmp);
			}
		}
	}
	return ("");
}
std::string Response::form_response()
{
	// HTTP/1.1 200 OK\r\n
	response += http_v + " " + status_code + " " + getMessage(status_code) + "\r\n"; 
	// Content-Length: 55\r\n
	response += "Content-Length: " + getContentLenght(path) + "\r\n"; 
	// Content-Type: text/html\r\n
	response += "Content-Type: " + getContentType(path) + "\r\n"; 
	// \r\n
	response += "\r\n";
	return (response);
}

void Response::fill_messages( void )
{
	messages["200"] = "OK";
	messages["201"] = "Created";
	messages["204"] = "Deleted";
	messages["400"] = "Bad Request";
	messages["403"] = "Forbidde";
	messages["404"] = "Page Not Found";
	messages["405"] = "Method Not Allowed";
	messages["408"] = "Timed out";
	messages["413"] = "Request Entity Too Large";
	messages["414"] = "Request-URI Too Long";
	messages["415"] = "Unsupported Media Type";
	messages["500"] = "Internal Server Error";
	messages["501"] = "Not Implemented";
	messages["505"] = "HTTP Version Not Supported";
}

std::string Response::pick_method(std::string method)
{
	std::string methods[3] = {"GET", "POST", "DELETE"};
	std::string (Response::* ptr[3]) (void) = {&Response::Get, &Response::Post, &Response::Delete};
	for (int i = 0; i < 3; i++)
	{
		if (methods[i] == method)
			return ((this->*(ptr[i]))());
	}
}