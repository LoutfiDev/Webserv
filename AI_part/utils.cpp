#include "utils.hpp"
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>


void handleError(const char *errorName, int errorCode)
{
	perror(errorName);
	std::exit(errorCode);
}

std::string trim(const std::string& str)
{
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");

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
	long num = rand();
	return num;
}

char randomChar(int old)
{
	long character = rand() * old;
	char charset[54] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ\0";
	character *= rand();
	if (character < 0)
		character *= -1;
	character %= 54;
	return (charset[character]);
}

std::string fileName()
{
	srand((unsigned) time(NULL));
	std::string name;
	int len = generateNum() % 20 + 10;
	for (int i = 1; i <= len; i++) {
		name.append(1, randomChar(i));
	}
	name.insert(0, "gen_");
	return name;	
}

/*
 * @Description : Post method
 * @param none 
 * 
 * @return string the response so that i can read it and send it to the Client
 *
 */

// std::string Post(Client client)
// {
// 	Request req;
// 	std::string post_response;
// 	std::fstream file;
//
// 	req = client.getRequest();
//
// 	if (req.getBodyLength() > 0)
// 	{
// 		post_response = req.getBody();
// 	}
// 	return "";
// }
