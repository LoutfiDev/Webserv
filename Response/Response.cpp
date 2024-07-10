/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaji <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 09:30:45 by soulang           #+#    #+#             */
/*   Updated: 2024/07/09 19:27:04 by anaji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "../AI_part/utils.hpp"

Response::Response() : STAGE(0), index(0), HEADERISWRITTEN(0), status(-1)
{
	status_code = "200";
	http_v = "HTTP/1.1";
	postState = PROCESSING;
	server = NULL;
	location = NULL;
	dir = NULL;
	in = NULL;
	out = NULL;
	err = NULL;
	fill_messages();
}

Response::Response(const Response& copy) { (void)copy; }

Response& Response::operator=(const Response& src)
{
	(void)src;
	return (*this);
}

Response::~Response() {}


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
		std::ifstream file(path.c_str());
		std::string line;
		while (std::getline(file, line))
		{
			if (line == "\r")
				break;
			std::string key, value;
			size_t found;
			found = line.find(":");
			key = trim(line.substr(0, found));
			value = trim(line.substr(found + 1));
			cgi_headers.insert(std::make_pair(key, value.erase(value.length(), 1)));
		}
		std::multimap<std::string, std::string>::iterator it = cgi_headers.begin();
		if (cgi_headers.size())
		{
			response += http_v + " ";
			if ((it = cgi_headers.find("Status")) != cgi_headers.end())
				response += it->second + "\r\n";
			else
				response += status_code + " " + getMessage(status_code) + "\r\n";
			if (write(socket, response.c_str() , response.size()) == -1)
				return -1;
			for (it = cgi_headers.begin(); it != cgi_headers.end(); ++it)
			{
				std::string tmp;
				if (it->second == cgi_headers.rbegin()->second)
					tmp = it->first + ":" + it->second + "\r\n\r\n";
				else
					tmp = it->first + ":" + it->second + "\r\n";
				index += tmp.size();
				if (write(socket, tmp.c_str() , tmp.size()) == -1)
					return -1;
			}
			if (status_code != "200")
				path = getPath();
			STAGE += 1;
		}
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

		if (write(socket, response.c_str() , response.size()) == -1)
			return -1;
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
			else
				closedir(directory);
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
				STAGE += 1;
				return -1;
			}
			if (write(socket, response.c_str() , response.size()) == -1)
				return -1;

			response.clear();
		}
		else
		{
			if (postState != END && processPostResponse() == 0)
				return 0;
			std::ifstream is (path.c_str(), std::ifstream::binary);
			if (is)
			{
				is.seekg (index, is.beg);
				is.read (buffer,1024);
				if (is)
				{
					if (write(socket, buffer , 1024) == -1)
						return -1;
					index += 1024;
				}
				else
				{
					if (is.gcount() == 0)
						return -1;
					if (write(socket, buffer , is.gcount()) == -1)
						return -1;
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
	messages["504"] = "Gateway Timeout";
	messages["505"] = "HTTP Version Not Supported";
}

void Response::pick_method()
{
	std::string tmp_method = method;

	toLower(tmp_method);
	std::string methods[3] = {"GET", "POST", "DELETE"};
	void (Response::* ptr[3]) ( void ) = {&Response::Get, &Response::Post, &Response::Delete};
	for (int i = 0; i < 3; i++)
	{
		if (methods[i] == method)
		{
			if (location && std::find(location->allow_methods.begin(), location->allow_methods.end(), tmp_method) != location->allow_methods.end())
				return ((this->*(ptr[i]))());
			status_code = "405";
		}
	}
}





