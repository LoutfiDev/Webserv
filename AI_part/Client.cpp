#include "Client.hpp"
#include "Request.hpp"
#include "Worker.hpp"
#include <cstddef>
#include <ctime>
#include <exception>
#include <iostream>
#include <iterator>
#include <string>
#include <unistd.h>
#include <vector>

Client::Client(int _fd, std::vector<Server *> data)
{
	response = new Response();
	fd = _fd;
	dataServer = data;
	isHeaderPartDone = 0;
	response->socket = _fd;
	state = READ;
	c_timer_start = time(0);
	std::cout << "new Client Created\n";
}

Client::Client(const Client& obj) {
	response = new Response();
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
	state = obj.state;
	c_timer_start = obj.c_timer_start;
	return (*this);
}

Request &Client::getRequest()
{
	return (request);
}

Response *Client::getResponse() const
{
	return response;
}

void Client::setRequest(const Request &rq)
{
	request = rq;
}

void Client::setResponse(Response *res)
{
	response = res;
}

int Client::getFd() const
{
	return fd;
}

std::vector<Server *> Client::getDataServer()
{
	return dataServer;
}

int Client::getState() const
{
	return state;
}

void Client::setState(int _state)
{
	response->status_code = request.getResponseCode();
	state = _state;
}
	
/*
 * @description read bytes sended by the client (request) to buffer and parse it
 *
 * @param buf tocken from the client
 * @return int 0 : succes | -1 : error
 */

void Client::readBuffer(char *buf)
{
	buffer += buf;
	size_t found = -1;
	c_timer_start = time(0);
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
				request.setRequestedLocation();
			}
			buffer = buffer.substr(found + 2);
		}
		else
		{
			if (request.addBody(buffer) == -1)
			{
				response->responseBody = request.getBody();
				return setState(WRITE);
			}
			if (request.getBodyLength() < 0)
			{
				std::cout << "Lengh = "<< request.getBodyLength() << "\n";
				return setState(ERROR);
			}
			buffer.clear();
		}
		if (request.getRequestCode())
		{
			std::cout << "pass to the response with <" << request.getRequestCode() << ">\n";
				return setState(ERROR);
		}
	}
	return setState(READ);
}

bool Client::istimeOut()
{
	if (time(0) - c_timer_start > TIMEOUT)
	{
		response->status_code = "408";
		return true;
	}
	return false;
}

void Client::showrequest()
{
	request.showHeaders();
	std::cout << "\n*************************BODY**************************\n";
	std::cout << request.getBody();
}


Client::~Client() {
	
	std::vector<Server *>::iterator it = dataServer.begin();
	while (it != dataServer.end()) {
		delete *it;
		dataServer.erase(it);
		it++;
	}
	delete response;


}
