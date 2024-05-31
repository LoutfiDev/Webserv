#include "Worker.hpp"
#include "Client.hpp"
#include "utils.hpp"
#include <cerrno>
#include <climits>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <vector>

#include "../Response/Response.hpp"

Worker::Worker() {

}

Worker::Worker(const Worker& obj) {
	*this = obj;
}

Worker &Worker::operator=(const Worker& obj)
{
	if (this == &obj)
		return (*this);
	clients = obj.clients;
	return (*this);
}
/*
 * @Description : read request (header + body) from client
 * @param fd int the fd to read from
 * @param client vector item :client to read his data and parse it
 * 
 * @return int state of the read (done, error, not yet)
 *
 */
int Worker::readFromClient(int fd, std::vector<Client *>::iterator client)
{
	int read_size;
	char buf[READBUFFER];

	std::memset(buf, '\0', READBUFFER - 1);
	read_size = read(fd, buf, READBUFFER - 2);
	buf[READBUFFER - 1] = '\0';
	if (read_size == -1)
	{
		std::cerr << "Connection Closed by peer\n";
		close(fd);
		return CONNECTIONCLOSED;
	}
	if (read_size == 0)
		return READINGISDONE;
	(*client)->readBuffer(buf);
	if ((*client)->getState() == ERROR)
	{
		std::cout << "error in the request\n";
		return ERRORINREADING;
	}
	if ((*client)->getState() == WRITE)
		return READINGISDONE; // move to write data to that client fd
	return NOTHING;
}

/*
 * @Description : write to the response client fd
 * @param fd int client to write to
 * @param client the actual client write to
 * 
 * @return true: write is done | false: there is data to write
 *
 */

bool Worker::writeToClient(std::vector<Client *>::iterator client)
{
	int read;
	read = 0;

	if ((*client)->getState() == ERROR)
	{
		(*client)->getResponse()->send_errorResponse();
		return true;
	}
	while ((read = (*client)->getResponse()->send_response())!= -1);
	if(read == -1)
	{
		return true;
	}
	return false;
}

void Worker::dropClientConnection(std::vector<Client *>::iterator client)
{
	std::cout << "Dropping Client\n";
	close((*client)->getFd());
	clients.erase(client);
}

void Worker::add(int connection, std::vector<Server *> &prerquisite)
{
	clients.push_back(new Client(connection, prerquisite));
}

int Worker::serve(int fd)
{
	std::vector<Client *>::iterator cli = clients.begin();

	while (cli != clients.end())
	{
		if ((*cli)->getFd() == fd)
		{
			if ((*cli)->getState() == READ)
				return readFromClient(fd, cli);
			else if ((*cli)->getState() == WRITE)
			{
				if (writeToClient(cli) == true)
				{
					dropClientConnection(cli);
					break;
				}
				break;
			}
			else
			{
				(*cli)->getResponse()->send_errorResponse();
				dropClientConnection(cli);
				break;
			}
		}
		cli ++;
	}
	return NOTHING;
}

void Worker::setClientResponse(int clientFd)
{
	for (size_t i = 0; i < clients.size(); i++) {
		if (clients[i]->getFd() == clientFd)
		{
			clients[i]->getResponse()->location = clients[i]->getRequest().getRequestedLocation();
			clients[i]->getResponse()->server = clients[i]->getRequest().getRequestedServer();
			clients[i]->getResponse()->method = clients[i]->getRequest().getMethodName();
			clients[i]->getResponse()->http_v = clients[i]->getRequest().getHttpVersion();
			clients[i]->getResponse()->status_code = clients[i]->getRequest().getResponseCode();
			clients[i]->getResponse()->path = clients[i]->getRequest().getPath();
			if (clients[i]->getState() == WRITE)
				clients[i]->getResponse()->pick_method(clients[i]->getResponse()->location);
		}
	}
}

void Worker::showClients()
{
	for (size_t i = 0; i < clients.size(); i++)
		clients[i]->showrequest();
}

Worker::~Worker() {
	std::vector<Client *>::iterator it = clients.begin();

	while (it != clients.end())
	{
		delete *it;
		clients.erase(it);
		it++;
	}
}
