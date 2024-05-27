#include "Client.hpp"
#include "Request.hpp"
#include <cstddef>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

Client::Client(int _fd, std::vector<Server *> data) {
	fd = _fd;
	std::cout << data.size() <<"\n";
	for (size_t i = 0; i < data.size(); i++) {
		dataServer.push_back(data[i]);
	}
	std::cout << "size dataServer => " << dataServer.size() <<"\n";
	isHeaderPartDone = 0;
}

Client::Client(const Client& obj) {
	*this = obj;	
}

Client &Client::operator=(const Client& obj)
{
	if (this == &obj)
		return (*this);
	fd = obj.fd;
	request = obj.request;
	response = obj.response;
	buffer = obj.buffer;
	isHeaderPartDone = obj.isHeaderPartDone;
	return (*this);
}

const Request &Client::getRequest() const
{
	return (request);
}

void Client::setRequest(Request &rq)
{
	request = rq;
}

int Client::getFd() const
{
	return fd;
}

std::vector<Server *> Client::getDataServer()
{
	return dataServer;
}

/*
 * @description read bytes sended by the client (request) to buffer and parse it
 *
 * @param buf tocken from the client
 * @return void
 */

int Client::readBuffer(char *buf)
{
	buffer += buf;
	size_t found = 0;
	while (buffer.size())
	{
		found = buffer.find("\r\n");
		if (found == 0)
		{
			buffer = buffer.substr(2);
			isHeaderPartDone++;
		}
		if (isHeaderPartDone == 0)
		{
			if (found == std::string::npos)
				break ;
			if (request.addHeader(buffer.substr(0, found)) == HOST_EXIST)
			{
				request.setRequestedServer(dataServer);
				request.setRequestedLocation(request.getPath());
			}
			buffer = buffer.substr(found + 2);
		}
		else
		{
			if (request.getHost().length() == 0)
				return 400;
			if (request.addBody(buffer) == -1)
			{
				std::cout << request.getBodyCount() << "\n";
				return -1;
			}
			if (request.getBodyLength() < 0)
			{
				std::cout << "Lengh = "<< request.getBodyLength() << "\n";
				return 402;
			}
			buffer.clear();
		}
		// std::cout << "RQ => " << request.getRequestCode() <<"\n";
		if (request.getRequestCode())
		{
			std::cerr << "pass to the response with <" << request.getRequestCode() << ">\n";
			return request.getRequestCode();
		}
	}
	// if (isHeaderPartDone == 1)
		// std::cout << "isheaderdone = " << isHeaderPartDone << "\n";
	return 0;
}

void Client::showrequest()
{
	request.showHeaders();
	std::cout << "\n*************************BODY**************************\n";
	std::cout << request.getBody();
}

Client::~Client() {
}
