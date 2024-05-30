#include "Worker.hpp"
#include "Client.hpp"
#include "utils.hpp"
#include <cerrno>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <vector>

#include "../Response/Response.hpp"

/*
 * @Description : read request (header + body) from client
 * @param 1 fd int the fd to read from
 * @param 2 _clients vector to see which client to read his data and parse it
 * 
 * @return int 0 = raed is done | -1 = error
 *
 */
int Worker::readFromClient(int fd)
{
	int request_res;
	int read_size;
	char buf[READBUFFER];

	for (size_t i = 0; i < clients.size(); i++)
	{
		if (clients[i]->getFd() == fd)
		{
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
				continue;
			request_res = clients[i]->readBuffer(buf);
			if (request_res == -1)
			{
				std::cout << "error in the request\n";
				return ERRORINREADING;
			}
			if (request_res == 0)
				return READINGISDONE;
			break ;
		}
	}
	return 0;
}

/*
 * @Description : write to the response client fd
 * @param fd int client to write to
 * @param _clients vector to see which one to write to
 * 
 * @return vector::iterator the client that gets his response so that i can
 *							close the connection and free the resources
 *
 */

std::vector<Client *>::iterator Worker::writeToClient(int fd)
{
	std::vector<Client *>::iterator c_beg = clients.begin();
	// static int status = 0;
	int read;
	// static int len = 60;
	// static int offset = 0;

	while (c_beg != clients.end())
	{
		if ((*c_beg)->getFd() == fd)
		{
			while ((read = (*c_beg)->getResponse()->send_response()) != -1);

			if(read == -1)
			{
				if (errno == 11)
					return c_beg;
				std::cerr << "Client has closed the Connection\n";
				return c_beg;
			}
			return c_beg;
			
			// while (1) ;
			// (*c_beg)->getResponse()->Post();	
			// if (!status)	
			// 	(*c_beg)->showrequest();
			// char resp[61] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello, world!\r\n";
			// read = write(fd, resp , strlen(resp));
			// status += read;
			// return c_beg;
			// if (status == -1)
			// {
			// 	std::cerr << "Client has closed the Connection\n";
			// 	return c_beg;
			// }
			// std::cout << status << "\n";
			// if (status == 61)
			// {
			// 	std::cout << "status is maxed" << status << "\n";
			// 	return c_beg;
			// }
			// if (read == 0)
			// 	return c_beg;
			// break;
		}
		c_beg++;
	}
	return clients.end();
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

int Worker::serve(int fd, int state)
{
	std::vector<Client *>::iterator cli;

	if (state == READ)
		return readFromClient(fd);
	else
	{
		cli = writeToClient(fd);
		if (cli != clients.end())
			dropClientConnection(cli);
		return 0;
	}
}

void Worker::showClients()
{
	for (size_t i = 0; i < clients.size(); i++)
	{
		clients[i]->showrequest();
	}
}

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

Worker::~Worker() {
}
