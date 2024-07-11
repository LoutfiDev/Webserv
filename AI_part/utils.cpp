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
#include <map>
#include <numeric>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

void see(std::string str)
{
	size_t len = str.length();

	std::cout << str.c_str() << " | " << len <<"\n";
}

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

long generateNum()
{
	srand((unsigned) time(NULL) + clock());
	long num = rand();
	return num;
}

char randomChar(int old)
{
	long character = generateNum() * old;
	char charset[54] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ\0";
	character *= rand();
	if (character < 0)
		character *= -1;
	return (charset[character % 52]);
}

std::string generateFileName(int gen)
{
	std::string name;
	int len = generateNum() % 20 + 5;
	for (int i = 1; i <= len; i++) {
		name.append(1, randomChar(i));
	}
	if (gen == 1)
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

std::string getUri(std::string &requestedUri, std::string locationName)
{
	size_t pos;
	std::string uri;

	if (locationName == "/\0")
		return requestedUri;
	locationName = removeLastChar(locationName);
	pos = requestedUri.find(locationName);
	if (pos == std::string::npos)
		return requestedUri;
	uri = requestedUri.substr(pos + locationName.length());
	return uri;
}

std::string getExtension(const std::string &file)
{
	std::string tmp;
	std::ifstream 	inputFile("./Response/mime.types");
	size_t pos;

	if (!inputFile.is_open())
	{
		std::cout << "Error: opening Configuration file failed" << std::endl;
		exit(127);	
	}
	while(std::getline(inputFile, tmp))
	{
		pos = tmp.find(file);
		if (pos != std::string::npos)
			return (trim(tmp.substr(pos + file.length()), " ;,"));
	}
	return ("");
}

int Response::processPostResponse()
{
	char buff[1024];

	memset(buff, '\0', 1023);
	if (method != "POST")
		return (postState = END, 1);
	infile.read(buff, 1023);
	if (infile)
		outfile.write(buff, 1023);
	else
		outfile.write(buff, infile.gcount());
	if (infile.eof()) 
	{
		infile.close();
		outfile.close();
		remove(responseBody.c_str());
		postState = END;
		return 1;
	}
	return 0;
}

/*
 * @Description : Post method, create a file that contains the content of the 
 *				  request body the URI in this method has no effect
 * @param none 
 * 
 * @return void
 *
 */

void Response::Post()
{
	std::string pathname;
	std::string path_dir;

	if (postState != PROCESSING)
		return;
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
		pathname = path_dir + '/' + generateFileName() + fileExtension;
	}
	else if (location->root.length())
	{
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
		pathname = path_dir + '/' + generateFileName() + fileExtension;
	}
	else
	{
		status_code = "403";
		send_response();
		return;
	}
	infile.open(responseBody.c_str(), std::ifstream::binary);
	outfile.open(pathname.c_str(), std::ofstream::binary);
	postState = SENDING;
	status_code = "201";
	processPostResponse();

	// i think in case of a regular file that can pass throught CGI we need to call GET method (arabic : dakchi li galina youssef l2ostora)
}

int c = 0;
void Response::send_errorResponse()
{
	std::map<std::string, std::string> error_map;
	error_map["201"] = "error_pages/201.html";
	error_map["204"] = "error_pages/204.html";
	error_map["400"] = "error_pages/400.html";
	error_map["403"] = "error_pages/403.html";
	error_map["404"] = "error_pages/404.html";
	error_map["405"] = "error_pages/405.html";
	error_map["408"] = "error_pages/408.html";
	error_map["413"] = "error_pages/413.html";
	error_map["414"] = "error_pages/414.html";
	error_map["415"] = "error_pages/415.html";
	error_map["500"] = "error_pages/500.html";
	error_map["501"] = "error_pages/501.html";
	error_map["504"] = "error_pages/504.html";
	error_map["505"] = "error_pages/505.html";

	size_t found;
	std::string key, value;
	if (getContentLenght(cgiOut) != "0")
	{
		std::ifstream file(cgiOut.c_str());
		std::string line;
		response += http_v + " ";
		while (std::getline(file, line))
		{
			if (line == "\r")
				break;
			found = line.find(":");
			key = trim(line.substr(0, found));
			value = trim(line.substr(found + 1));
			if (key  == "Status")
			{
				status_code = value.substr(0, 3);
				std::cout << status_code << "\n";
				response += value + "\n";
			}
			else
				response += line + "\n";
		}
		response += "\r\n";
		std::cout << response << "\n";
	}
	else
	{
		response += http_v + " " + status_code + " " + getMessage(status_code) + "\r\n";
		response += "Content-Type: text/html\r\n\r\n";
	}

	write(socket, response.c_str(), response.length());
	if (method != "HEAD")
	{
		std::ifstream file(error_map[status_code].c_str());
		std::string line;

		while (std::getline(file, line))
			write(socket, line.c_str(), line.size());
	}
}

std::string to_String(long long num) {
	std::ostringstream oss;
	oss << num;
	return oss.str();
}
