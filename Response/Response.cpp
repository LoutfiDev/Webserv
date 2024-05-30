/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaji <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 09:30:45 by soulang           #+#    #+#             */
/*   Updated: 2024/05/30 03:03:50 by anaji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include <netdb.h>
#include <string>

//******************TO DO***************************//
// v- list files while autoIndex is on              //
// v- create a buffer to write response             //
// v- create FLAGS to protect overwrite             //
// v- Add location header to response if "301"      // 
// v- ask for text/html resp if "301"               //
// v- set index.html as default index directive     //
// v- set index to 0 after sending the body         //
// v- set Response constructed by def constructor   //
// v- Add location* server* as atrrib to response   //
// v- set an int in send_response as return         //
// v- default server Configuration					//
//**************************************************//


// you can pass a request object as param to the constructer
// Response::Response(Request req) : status_code("200")

Response::Response() : STAGE(0), index(0), http_v("HTTP/1.1"), status_code("200")
{
	fill_messages();
}

// Response::Response() : STAGE(0), index(0), method("GET"), path("web_root/index.html"), http_v("HTTP/1.1"), status_code("200")
// {
// 	fill_messages();
// 	//you can pass req.method to this funct to call the method and form the response 
// 	if (status_code == "200")
// 		pick_method(location);
// 	// int i = send_response();
// }

Response::Response(const Response& copy) { (void)copy; }

Response& Response::operator=(const Response& src)
{
	(void)src;
	return (*this);
}

Response::~Response() {}

void Response::Get() {
	DIR *directory;
	
	if ((directory = opendir(path.c_str())))
	{
		if (path[path.size() - 1] != '/')
		{
			path.append("/");
			status_code = "301";
		}
		else
		{
			if (location->index.size())
			{
				std::string tmp;
				std::vector<std::string>::iterator it = location->index.begin();
				for (; it != location->index.end(); ++it)
				{
					tmp = path + *it;
					if (access(tmp.c_str(), F_OK) == 0)
					{
						path.append(*it);
						status_code = "301";
						return ;
					}
				}
				status_code = "403";
			}
			else if (location->autoindex)
				send_response();
			else
				status_code = "403";
		}
		closedir(directory);
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
}

// void Response::Post() { return; }

void Response::Delete_folder(std::string path)
{
	DIR *directory;
	directory=opendir(path.c_str());
	
	struct dirent *dent;
	while((dent=readdir(directory)) && status_code == "200")
	{
		struct stat st_buf;
		stat (dent->d_name, &st_buf);
		// //  if (stat (dent->d_name, &st_buf) != 0) no_such_file
		if (std::string(dent->d_name) == ".." || std::string(dent->d_name) == ".")
			continue;
		if (S_ISDIR (st_buf.st_mode))
			Delete_folder(path + "/" + dent->d_name);
		else if (S_ISREG (st_buf.st_mode)) 
		{
			if (access(path.c_str(), R_OK) != 0)
				status_code = "403";
			else 
				remove(path.c_str());
		}
	}
	closedir(directory);
}

void Response::Delete() 
{ 
	DIR *directory;

	if((directory=opendir(path.c_str())))
	{
		Delete_folder(path);
		closedir(directory);
	}
	else
	{
		if (access(path.c_str(), F_OK) == 0)
		{
			if (access(path.c_str(), R_OK) != 0)
				status_code = "403";
			else 
				remove(path.c_str());
		}
		else
			status_code = "404";
	}
}

std::string Response::getMessage(std::string code)
{
	std::string message = "";
	if (messages.find(code) != messages.end())
		message = messages[code];
	return (message);
}

std::string Response::getContentLenght(std::string file)
{
	std::string size;
	struct stat st;
	stat(file.c_str(), &st);
	long nb = st.st_size;
	std::stringstream ss;  
	ss << nb;  
	ss >> size; 
	return (size);
}

std::string Response::getContentType(std::string file)
{
	std::string tmp;
	file.erase(0, file.find('.') + 1);
	std::ifstream 	inputFile("Response/mime.types");
	if (!inputFile.is_open())
	{
		std::cout << "Error: opening Configuration file failed" << std::endl;
		exit(127);	
	}
	while(std::getline(inputFile, tmp))
	{
		if (tmp.find(file) != std::string::npos)
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
std::string Response::getPath( void )
{
	if (server->error_pages.find(status_code) != server->error_pages.end())
		return server->error_pages[status_code];
	return "";
}

int Response::send_response()
{
	if (STAGE < HEADERISSENT)
	{
		response += http_v + " " + status_code + " " + getMessage(status_code) + "\r\n";
		if (status_code != "200")
		{
			if (status_code == "301")
				response += "Location: " + path + "\r\n";
			path = getPath();
		}
		

		if (!path.empty())
		{
			if (path[path.size() - 1] == '/')
				response += "Content-Type: text/html\r\n\r\n";
			else
			{
				response += "Content-Length: " + getContentLenght(path) + "\r\n";
				response += "Content-Type: " + getContentType(path) + "\r\n";
				response += "\r\n";
			}
		}
		else
			response += "Content-Type: text/html\r\n\r\n";
		write(socket, response.c_str() , response.size());
			
		STAGE += 1;
	}
	else if (STAGE < BODYISSENT)
	{			
			memset(buffer, 0, 1024);
			if((dir=opendir(path.c_str())))
			{
				int portion = 1;
				struct dirent *dent;
				if (portion == 1)
				{
					response.clear();
					response += "<html><head><title>Index of /</title></head><body><h1>Index of /</h1><hr><pre>";	
				}
				while((dent=readdir(dir)) || (portion % 4))
				{
					struct stat st_buf;
					stat (dent->d_name, &st_buf);
					// //  if (stat (dent->d_name, &st_buf) != 0) no_such_file
					if (std::string(dent->d_name) == ".." || std::string(dent->d_name) == ".")
						continue;
					if (S_ISDIR (st_buf.st_mode))
						response += "<a href=\"" + std::string(dent->d_name) + "/\">" + std::string(dent->d_name) + "/</a></br>";
					else if (S_ISREG (st_buf.st_mode)) 
						response += "<a href=\"" + std::string(dent->d_name) + "\">" + std::string(dent->d_name) + "</a></br>";
					portion++;
				}
				closedir(dir);
				if(!dent)
				{
					response += "</pre><hr></body></html>";
					write(socket, response.c_str() , response.size());
					return -1;
				}
				write(socket, response.c_str() , response.size());
			}
			else
			{
				std::ifstream is (path.c_str(), std::ifstream::binary);
				if (is) 
				{
					is.seekg (index, is.beg);
					is.read (buffer,1024);
					if (is)
					{
						write(socket, buffer , 1024);
						index += 1024;
					}
					else
					{
						if (is.gcount() == 0)
							return -1;
						write(socket, buffer , is.gcount());
						index += is.gcount();
					}	
					is.close();
				}
				else
					return -1;
			}
	}
	return 0;
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

#include "../AI_part/utils.hpp"

// location.allow_methods are in lower case try cout << location.allow_methods[0]

void Response::pick_method(Location *location)
{
	std::string tmp_method = method;

	toLower(tmp_method); //from utils.cpp
	std::string methods[3] = {"GET", "POST", "DELETE"};
	void (Response::* ptr[3]) ( void ) = {&Response::Get, &Response::Post, &Response::Delete};
	for (int i = 0; i < 3; i++)
	{
		if (methods[i] == method)
		{
			if (std::find(location->allow_methods.begin(), location->allow_methods.end(), tmp_method) != location->allow_methods.end())
				return ((this->*(ptr[i]))());
			status_code = "405";
		}
	}
}
