#include "Client.hpp"
#include "Request.hpp"
#include "Worker.hpp"
#include <cstddef>
#include <cstring>
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
	init_dataServer(data);
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
	init_dataServer(obj.dataServer);
	buffer = obj.buffer;
	isHeaderPartDone = obj.isHeaderPartDone;
	state = obj.state;
	c_timer_start = obj.c_timer_start;
	return (*this);
}

void Client::init_dataServer(const std::vector<Server *> &_server)
{
	size_t i = 0;

	while (i < _server.size())
	{
		dataServer.push_back(new Server(*_server[i]));
		i++;
	}
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

void Client::resetTimer()
{
	c_timer_start = time(0);
}
	
/*
 * @description read bytes sended by the client (request) to buffer and parse it
 *
 * @param buf tocken from the client
 * @return int 0 : succes | -1 : error
 */
long count = 0;
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
			response->http_v = request.getHttpVersion();
			response->method = request.getMethodName();
			response->path = request.getPath();
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
			count += buffer.size();
			if (request.addBody(buffer) == -1)
			{
				if (request.getMethodName() == "POST")
					response->responseBody = request.getBodyFile();
				return setState(WRITE);
			}
			if (request.getBodyLength() < 0)
			{
				std::cout << "Lengh = "<< request.getBodyLength() << "\n";
				return setState(ERROR);
			}
			std::cout << count << "\n";
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
}

Client::~Client() {
	
	size_t i = 0;
	std::cout << "Drope Client\n";
	std::vector<Server *>::iterator it = dataServer.begin();
	while (i < dataServer.size()) {
		dataServer.erase(it);
		delete it[i];
		it++;
		i++;
	}
	delete response;
}
