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

void Response::send_errorResponse()
{
	int wrt;
	size_t found;
	std::string key, value;
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
				response += value + "\n";
			}
			else
				response += line + "\n";
		}
		response += "\r\n";
	}
	else
	{
		response += http_v + " " + status_code + " " + getMessage(status_code) + "\r\n";
		response += "Content-Type: text/html\r\n\r\n";
	}

	wrt = write(socket, response.c_str(), response.length());
	if (wrt == -1)
		return;
	if (method != "HEAD")
	{
		std::ifstream file(error_map[status_code].c_str());
		std::string line;

		while (std::getline(file, line))
		{
			wrt = write(socket, line.c_str(), line.size());
			if (wrt == -1)
				return ;
		}
	}
}

std::string to_String(long long num) {
	std::ostringstream oss;
	oss << num;
	return oss.str();
}
