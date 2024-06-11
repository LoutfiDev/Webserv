#include "utils.hpp"
#include "../Response/Response.hpp"
#include <cctype>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fcntl.h>
#include <fstream>
#include <ios>
#include <iostream>
#include <istream>
#include <numeric>
#include <string>
#include <sys/stat.h>
#include <unistd.h>


void handleError(const char *errorName, int errorCode)
{
	perror(errorName);
	std::exit(errorCode);
}

std::string trim(const std::string& str, std::string toTrim)
{
    size_t first = str.find_first_not_of(toTrim);
    size_t last = str.find_last_not_of(toTrim);

    if (first == std::string::npos || last == std::string::npos)
        return "";

    return str.substr(first, last - first + 1);
}

void toLower(std::string &str)
{
	for (size_t i = 0; i < str.length(); i++) {
		str[i] = std::tolower(str[i]);
	}
}

long Response::generateNum()
{
	srand((unsigned) time(NULL) + clock());
	long num = rand();
	return num;
}

char Response::randomChar(int old)
{
	long character = generateNum() * old;
	char charset[54] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ\0";
	character *= rand();
	if (character < 0)
		character *= -1;
	return (charset[character %= 54]);
}

std::string Response::generateFileName()
{
	std::string name;
	int len = generateNum() % 20 + 10;
	for (int i = 1; i <= len; i++) {
		name.append(1, randomChar(i));
	}
	name.insert(0, "gen_");
	return name;	
}

std::string removeLastChar(std::string str) // remove last '/'
{
	size_t i = str.size();

	if (str[i - 1] == '/')
	{
		i--;
		str = str.substr(0, i);
	}
	return str;
}

std::string getFileName(std::string &filename)
{
	long i = filename.size();
	std::string name;

	if (filename[i] == '/')
		i--;
	while (i >= 0)
	{
		if (filename[i] == '/')
			break;
		i--;
	}
	
	name = filename.substr(0, i);
	return name;
}

std::string getUri(std::string &requestedUri, std::string &locationName)
{
	size_t pos;
	std::string uri;

	if (locationName == "/\0")
		return requestedUri;
	pos = requestedUri.find(locationName);
	if (pos == std::string::npos)
		return requestedUri;
	uri = requestedUri.substr(pos + locationName.length());
	return uri;
}

int Response::processPostResponse()
{
	static int count = 0;
	char buff[1024];

	std::cout << count++ << "\n";
	memset(buff, '\0', 1023);
	if (method != "POST")
		return (postState = END, 1);
	if (typeInfile == BODY)
	{
		outfile << responseBody;
		postState = END;
		return 1;
	}
	else 
	{
		infile.read(buff, 1023);
		if (infile)
			outfile.write(buff, 1023);
		else
		{
			outfile.write(buff, infile.gcount());
		}
		if (infile.eof()) 
		{
			infile.close();
			outfile.close();
			postState = END;
			return 1;
		}
		return 0;
	}
}

/*
 * @Description : Post method
 * @param none 
 * 
 * @return string the response so that i can read it and send it to the Client
 *
 */

void Response::Post()
{
	// path = "/nfs/homes/anaji/Desktop/webServer/web_root/index.html";
	std::string requestedfile = path;
	struct stat st_stat;
	std::string pathname;
	std::string path_dir;

	if (postState != PROCESSING)
		return;
	std::cout << requestedfile << "\n";
	if (access(requestedfile.c_str(), F_OK) == -1)
	{
		status_code = "404";
		send_response();
		return;
	}
	std::cout << "requested =>" << requestedfile << "\n";
	stat(requestedfile.c_str(), &st_stat);
	if (st_stat.st_mode & S_IFDIR && requestedfile[requestedfile.size() - 1] != '/')
	{
		uri.append("/");
		status_code = "301";
		send_response();
		return;
	}
	if (st_stat.st_mode & S_IFDIR)
	{
		std::cout << "need a DIR\n";
		if (location->upload_dir.length())
		{
			if (access(location->upload_dir.c_str(), F_OK) == -1)
			{
				status_code = "404";
				send_response();
				return;
			}
			if (access(location->upload_dir.c_str(), W_OK) == -1)
			{
				status_code = "403";
				send_response();
				return;
			}
			path_dir = removeLastChar(location->upload_dir);
			pathname = path_dir + '/' + generateFileName();
		}
		else if (location->root.length())
		{
			// std::cout << "1-> "<<location->root << " " << location->root << "\n";
			if (access(location->root.c_str(), F_OK) == -1)
			{
				status_code = "404";
				send_response();
				return;
			}
			if (access(location->root.c_str(), W_OK) == -1)
			{
				status_code = "403";
				send_response();
				return;
			}
			path_dir = removeLastChar(location->root);
			pathname = path_dir + '/' + generateFileName();
		}
		else
		{
			status_code = "404";
			send_response();
			return;
		}
		outfile.open(pathname.c_str(), std::ofstream::binary);
		postState = SENDING;
		typeInfile = BODY;
		status_code = "201";
		processPostResponse();
	}
	else if (st_stat.st_mode & S_IFREG)
	{
		std::cout << "need a REGFILE\n";
		if (location->upload_dir.length())
		{
			std::cout << "have an upload_dir location =>" << location->upload_dir << "\n";
			if (access(location->upload_dir.c_str(), F_OK) == -1)
			{
				status_code = "404";
				send_response();
				return;
			}
			if (access(location->upload_dir.c_str(), W_OK) == -1)
			{
				status_code = "403";
				send_response();
				return;
			}
			path_dir = removeLastChar(location->upload_dir);
			pathname = path_dir + '/' + generateFileName();
		}
		else if (location->root.length())
		{
			std::cout << "have a root location =>" << location->root << "\n";
			if (access(location->root.c_str(), F_OK) == -1)
			{
				status_code = "404";
				send_response();
				return;
			}
			if (access(location->root.c_str(), W_OK) == -1)
			{
				status_code = "403";
				send_response();
				return;
			}
			path_dir = removeLastChar(location->root);
			pathname = path_dir + '/' + generateFileName();
		}
		else
		{
			status_code = "404";
			send_response();
			return;

		}
		infile.open(requestedfile.c_str(), std::ifstream::binary);
		outfile.open(pathname.c_str(), std::ofstream::binary);
		status_code = "201";
		postState = SENDING;
		typeInfile = FILE;
		processPostResponse();
		// i think in case of a regular file that can pass throught CGI we need to call GET method (arabic : dakchi li galina youssef l2ostora)
	}
	else
		std::cout << "ALO\n";
}

void Response::send_errorResponse()
{
	response += http_v + " " + status_code + " " + getMessage(status_code) + "\r\n";
	// response += "Content-Length: " + getMessage(status_code).length() + "\r\n";
	response += "Content-Type: text/html\r\n\r\n";
	response += getMessage(status_code) + "\r\n";
	std::cout << response;
	write(socket, response.c_str(), response.length());
}
