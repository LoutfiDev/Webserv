/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soulang <soulang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 09:30:45 by soulang           #+#    #+#             */
/*   Updated: 2024/06/13 23:46:41 by soulang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include <iostream>
#include <netdb.h>
#include <string>


Response::Response() : STAGE(0), index(0), HEADERISWRITTEN(0), status(-1)
{
	status_code = "200";
	http_v = "HTTP/1.1";
	postState = PROCESSING;
	server = NULL;
	location = NULL;
	fill_messages();
}

Response::Response(const Response& copy) { (void)copy; }

Response& Response::operator=(const Response& src)
{
	(void)src;
	return (*this);
}

Response::~Response() {}

void Response::Get() {
	DIR *directory;
	if (location->redirection.size())
	{
		std::map<std::string, std::string>::iterator it = location->redirection.begin();
		status_code = it->first;
	}
	else if ((directory = opendir(path.c_str())))
	{
		if (uri[uri.size() - 1] != '/')
		{
			uri.append("/");
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
						if (access(path.c_str(), R_OK) != 0)
							status_code = "403";
						closedir(directory);
						return ;
					}
				}
				status_code = "403";
			}
			else if (location->autoindex)
			{
				std::string tmp(path + "index.html");
				if (access(tmp.c_str(), F_OK) == 0)
				{
					if (access(tmp.c_str(), R_OK) != 0)
						status_code = "403";
					else
						path = tmp;
				}
			}
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


void Response::Delete_folder(std::string path)
{
	DIR *directory;
	directory=opendir(path.c_str());
	
	struct dirent *dent;
	while((dent=readdir(directory)) && status_code == "200")
	{
		struct stat st_buf;
		stat (std::string(path + "/" + dent->d_name).c_str(), &st_buf);
		if (std::string(dent->d_name) == ".." || std::string(dent->d_name) == ".")
			continue;
		if (S_ISDIR(st_buf.st_mode))
		{
			Delete_folder(path + "/" + dent->d_name);
			if ((remove(std::string(path + "/" + dent->d_name).c_str())) != 0)
				status_code = "403";
		}
		else if (S_ISREG(st_buf.st_mode)) 
		{
			if ((remove(std::string(path + "/" + dent->d_name).c_str())) != 0)
				status_code = "403";
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
		if (status_code == "200")
		{
			if ((remove(path.c_str())) != 0)
				status_code = "403";
			else
				status_code = "204";
		}
	}
	else
	{
		if (access(path.c_str(), F_OK) == 0)
		{
			if ((remove(path.c_str())) != 0)
				status_code = "403";
			else 
				status_code = "204";
		}
		else
			status_code = "404";
	}
}


void Response::formEnv( void )
{
	env = new char*[8];
	
	env[0] = new char[std::string(std::string("REQUEST_METHOD") + "=" + method).size() + 1];
	strcpy(env[0], std::string(std::string("REQUEST_METHOD") + "=" + method).c_str());
	
	env[1] = new char[std::string(std::string("QUERY_STRING") + "=" + query).size() + 1];
	strcpy(env[1], std::string(std::string("QUERY_STRING") + "=" + query).c_str());
	
	env[2] = new char[std::string(std::string("REDIRECT_STATUS") + "=" + "200").size() + 2];
	strcpy(env[2], std::string(std::string("REDIRECT_STATUS") + "=" + "200").c_str());
	
	env[3] = new char[std::string(std::string("SCRIPT_FILENAME") + "=" + path).size() + 1];
	strcpy(env[3], std::string(std::string("SCRIPT_FILENAME") + "=" + path).c_str());
	
	env[4] = new char[std::string(std::string("CONTENT_TYPE") + "=" + getContentType(path)).size() + 1];
	strcpy(env[4], std::string(std::string("CONTENT_TYPE") + "=" + getContentType(path)).c_str());
	
	env[5] = new char[std::string(std::string("HTTP_COOKIE") + "=" + http_cookie).size() + 1];
	strcpy(env[5], std::string(std::string("HTTP_COOKIE") + "=" + http_cookie).c_str());
	
	env[6] = new char[std::string(std::string("CONTENT-LENGTH") + "=" + getContentLenght(path)).size() + 1];
	strcpy(env[6], std::string(std::string("CONTENT-LENGTH") + "=" + getContentLenght(path)).c_str());
	
	env[7] = NULL;
}

int Response::isValid( void )
{
	std::stringstream ss(cgiFile);
	while (getline(ss, extension, '.'))
		continue;
	extension.insert (0, 1, '.');
	std::map<std::string, std::string>::iterator it = location->cgi.begin();
	for (; it != location->cgi.end(); ++it)
	{
		if (extension == it->first)
		{
			cgiPath = it->second;
			return (0);
		}
	}
	return 1;
}

int Response::is_cgi()
{
	DIR *directory;
	
	if ((directory = opendir(cgiFile.c_str())))
	{
		if (location->index.size())
		{
			std::string tmp;
			std::vector<std::string>::iterator it = location->index.begin();
			for (; it != location->index.end(); ++it)
			{
				tmp = cgiFile + *it;
				if (access(tmp.c_str(), F_OK) == 0)
				{
					if (access(tmp.c_str(), W_OK) != 0)
						return 1;
					else
					{
						cgiFile.append(*it);
						if (isValid())
							return 1;
						return 0;
					}
				}
			}
			return 1;
		}
		else
			return 1;
		closedir(directory);
	}
	else
	{
		if (access(cgiFile.c_str(), F_OK) == 0)
		{
			if (access(cgiFile.c_str(), W_OK) != 0 || isValid())
				return 1;			
		}
		else
			return 1;
	}
	return 0;
}
void Response::resetTimer()
{
	timespan = time(0);
}

bool Response::istimeOut()
{
	if (time(0) - timespan > CGITIMEOUT)
		return true;
	return false;
} 

int Response::execute_cgi( void )
{
		cgiFile = path;
		if (location->cgi.size() && !is_cgi())
		{
			if (STAGE == EXEC_CGI)
			{
				resetTimer();
				argv = new char*[3];
				argv[0] = new char[cgiPath.size() + 1];
				strcpy(argv[0], cgiPath.c_str());
				argv[1] = new char[cgiFile.size() + 1];
				strcpy(argv[1], cgiFile.c_str());
				argv[2] = NULL;
				formEnv();
				cgiOut = generateFileName() + ".html";
				if ((pid = fork()) == 0)
				{
					freopen (cgiOut.c_str(),"w",stdout);
					if (method == "POST")
						freopen (responseBody.c_str(),"r",stdin);
					if (execve(argv[0], argv, env) == -1)
						exit(1);
				}
				STAGE += 1;
			}
			waitpid(pid, &status, WNOHANG);
			if (status != -1)
			{
				if (status != 0)
				{
					status_code = "500";
					STAGE += 1;
					return 2;
				}
				STAGE += 1;
			}
			else if (istimeOut())
			{
				kill(pid, SIGKILL);
				status_code = "408";
				std::cout << "STAGE =>" <<STAGE << "\n";
				STAGE += 1;
				std::cout << "STAGE =>" <<STAGE << "\n";
				return 2;
			}
		}
		else
			STAGE = CGI_PROCESSING + 1;
	return 1;
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
				inputFile.close();
				return (tmp);
			}
		}
	}
	inputFile.close();
	return ("");
}
std::string Response::getPath( void )
{

	if (server && server->error_pages.find(status_code) != server->error_pages.end())
		return server->error_pages[status_code];
	return "";
}

int Response::send_response()
{
	if (!cgiOut.empty() && STAGE == HEADER_PROCESSING)
	{
		path = cgiOut;
		if (extension == ".php")
			STAGE += 1;
		response += http_v + " " + status_code + " " + getMessage(status_code) + "\r\n";
		write(socket, response.c_str() , response.size());
	}
	if (STAGE == HEADER_PROCESSING)
	{
		response += http_v + " " + status_code + " " + getMessage(status_code) + "\r\n";
		if (status_code != "200")
		{
			if (status_code == "301")
			{
				if (location->redirection.size())
				{
					std::map<std::string, std::string>::iterator it = location->redirection.begin();
					response += "Location: " + it->second + "\r\n";
				}
				else
					response += "Location: " + uri + "\r\n";
			}
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
	else if (STAGE == BODY_PROCESSING)
	{			
			DIR *directory;
			memset(buffer, 0, 1024);

			if((directory=opendir(path.c_str())))
			{
				int portion = 1;
				struct dirent *dent;
				if (!HEADERISWRITTEN)
				{
					dir = directory;
					response.clear();
					response += "<html><head><title>Index of /</title></head><body><h1>Index of /</h1><hr><pre>";
					HEADERISWRITTEN++;
				}
				while((portion % 4) && (dent=readdir(dir)))
				{
					struct stat st_buf;
					stat (std::string(path + "/" + dent->d_name).c_str(), &st_buf);
					if (std::string(dent->d_name) == ".." || std::string(dent->d_name) == ".")
						continue;
					if (S_ISDIR (st_buf.st_mode))
						response += "<a href=\"" + std::string(dent->d_name) + "/\">" + std::string(dent->d_name) + "/</a></br>";
					else if (S_ISREG (st_buf.st_mode))
						response += "<a href=\"" + std::string(dent->d_name) + "\">" + std::string(dent->d_name) + "</a></br>";
					portion++;
				}
				if(!dent)
				{
					response += "</pre><hr></body></html>";
					write(socket, response.c_str() , response.size());
					response.clear();
					closedir(directory);
					STAGE += 1;
					return -1;
				}
				write(socket, response.c_str() , response.size());
				response.clear();
			}
			else
			{
				if (postState != END && processPostResponse() == 0)
					return 0;
				std::ifstream is (path.c_str(), std::ifstream::binary);
				if (is) 
				{
					std::cout << "index :" << index << "\n";
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
						{
							STAGE += 1;
							return -1;
						}
						write(socket, buffer , is.gcount());
						index += is.gcount();
					}	
					is.close();
				}
				else
				{
					STAGE += 1;
					return -1;
				}
			}
	}
	return 0;
}

void Response::fill_messages( void )
{
	messages["200"] = "OK";
	messages["201"] = "Created";
	messages["204"] = "Deleted";
	messages["301"] = "Moved Permanently";
	messages["400"] = "Bad Request";
	messages["403"] = "Forbidden";
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

void Response::pick_method()
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
