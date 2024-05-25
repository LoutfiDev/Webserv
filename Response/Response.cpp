/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soulang <soulang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 09:30:45 by soulang           #+#    #+#             */
/*   Updated: 2024/05/25 11:36:18 by soulang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"


// you can pass a request object as param to the constructer
// Response::Response(Request req) : status_code("200")

Response::Response() {}

Response::Response(Location *location) : method("GET"), path("web_root/index.html"), http_v("HTTP/1.1"), status_code("200")
{
	fill_messages();
	//you can pass req.method to this funct to call the method and form the response 
	pick_method(location);
	send_response();
}

Response::Response(const Response& copy) { (void)copy; }

Response& Response::operator=(const Response& src)
{
	(void)src;
	return (*this);
}

Response::~Response() {}

void Response::Get(Location *location) {
	DIR *dir;
	
	if ((dir = opendir(path.c_str())))
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
				//append index to path
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
			{
				struct dirent *dent;
				while((dent=readdir(dir)))
				{
					struct stat st_buf;
					stat (dent->d_name, &st_buf);
					// //  if (stat (dent->d_name, &st_buf) != 0) no_such_file
					if (std::string(dent->d_name) == ".." || std::string(dent->d_name) == ".")
						continue;
					if (S_ISDIR (st_buf.st_mode))
						std::cout << dent->d_name << "\t DIRECTORY\n";
					else if (S_ISREG (st_buf.st_mode)) 
						std::cout << dent->d_name << "\t FILE\n";
				}
			}
			else
				status_code = "403";
		}
		closedir(dir);
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

void Response::Post(Location *location) { (void)location; }

void Response::Delete_folder(std::string path)
{
	DIR *dir;
	dir=opendir(path.c_str());
	
	struct dirent *dent;
	while((dent=readdir(dir)) && status_code == "200")
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
	closedir(dir);
}

void Response::Delete(Location *location) 
{ 
	(void)location;
	DIR *dir;
	//path now it's temporary

	if((dir=opendir(path.c_str())))
	{
		Delete_folder(path);
		closedir(dir);
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
void Response::send_response()
{
	// HTTP/1.1 200 OK\r\n
	response += http_v + " " + status_code + " " + getMessage(status_code) + "\r\n"; 
	// Content-Length: 55\r\n
	// if (status_code != "200")
		// path = getPath(status_code);
	response += "Content-Length: " + getContentLenght(path) + "\r\n";
	// Content-Type: text/html\r\n
	response += "Content-Type: " + getContentType(path) + "\r\n"; 
	// \r\n
	response += "\r\n";
	//for the body just use this msg as the body
	response += "My First Heading";
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

void Response::pick_method(Location *location)
{
	std::string methods[3] = {"GET", "POST", "DELETE"};
	void (Response::* ptr[3]) (Location *location) = {&Response::Get, &Response::Post, &Response::Delete};
	for (int i = 0; i < 3; i++)
	{
		if (methods[i] == method)
		{
			if (std::find(location->allow_methods.begin(), location->allow_methods.end(), method) != location->allow_methods.end())
				return ((this->*(ptr[i]))(location));
			status_code = "405";
		}
	}
}
