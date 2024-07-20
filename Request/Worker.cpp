#include "Worker.hpp"
#include "Client.hpp"
#include "utils.hpp"
#include <cerrno>
#include <climits>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <cstring>
#include <sys/epoll.h>
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
	int READBUFFER = 1024;

	(*client)->resetTimer();
	char buf[READBUFFER];
	std::memset(buf, '\0', READBUFFER);
	read_size = read(fd, buf, READBUFFER - 1);
	if (read_size == -1)
	{
		std::cerr << "Connection Closed by peer\n";
		delete *client;
		clients.erase(client);
		return CONNECTIONCLOSED;
	}
	(*client)->readBuffer(buf, read_size);
	if (read_size == 0)
		return READINGISDONE;
	if ((*client)->getState() == ERROR)
		return ERRORINREADING;
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
	int response_result;

	(*client)->resetTimer();
	if ((*client)->getState() == ERROR)
	{
		(*client)->getResponse()->send_response();
		if ((*client)->getResponse()->STAGE > BODY_PROCESSING)
			return true;
	}
	if ((*client)->getResponse()->STAGE <= CGI_PROCESSING)
	{
		int error = (*client)->getResponse()->execute_cgi();
		if (error == ERROR)
		{
			(*client)->getResponse()->send_errorResponse();
			return true;
		}
	}
	else
	{
		if ((*client)->getResponse()->STAGE == HEADER_PROCESSING)
			(*client)->getResponse()->pick_method();
		response_result = (*client)->getResponse()->send_response();
		if (response_result == -1)
		{
			(*client)->setIgnoreTimer(true);
			return true;
		}
	}
	return false;
}

void Worker::dropClientConnection(std::vector<Client *>::iterator client)
{
	if ((*client)->getResponse()->in)
		fclose((*client)->getResponse()->in);
	if ((*client)->getResponse()->out)
		fclose((*client)->getResponse()->out);
	if ((*client)->getResponse()->err)
		fclose((*client)->getResponse()->err);
	if ((*client)->getResponse()->dir)
		closedir((*client)->getResponse()->dir);
	if (access((*client)->getResponse()->cgiOut.c_str(), F_OK) == 0)
		remove((*client)->getResponse()->cgiOut.c_str());
	if (access((*client)->getResponse()->cgiErr.c_str(), F_OK) == 0)
		remove((*client)->getResponse()->cgiErr.c_str());
	if (access((*client)->getResponse()->responseBody.c_str(), F_OK) == 0)
		remove((*client)->getResponse()->responseBody.c_str());
	if (access((*client)->getRequest().tmp_body_file_name.c_str(), F_OK) == 0)
		remove((*client)->getRequest().tmp_body_file_name.c_str());
	if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, (*client)->getFd(), NULL) == -1)
		handleError("epoll_ctl", errno);
	close((*client)->getFd());
	delete(*client);
	clients.erase(client);
}

void Worker::add(int connection, std::vector<Server *> &prerquisite)
{
	Client *tmpCli = new Client(connection, prerquisite);
	clients.push_back(tmpCli);
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
				if (writeToClient(cli))
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

void Worker::initResponse(int clientFd)
{
	size_t tmp, max_body_size = 100000000000;
	char *s;

	for (size_t i = 0; i < clients.size(); i++) {
		if (clients[i]->getFd() == clientFd)
		{
			if (clients[i]->getResponse()->status_code != "200")
				return ;
			if (clients[i]->getRequest().getHost().length() == 0)
				return clients[i]->setState(ERROR);
			
			clients[i]->getResponse()->path = clients[i]->getRequest().getPath();
			clients[i]->getResponse()->status_code = clients[i]->getRequest().getResponseCode();
			clients[i]->getResponse()->location = new Location(*clients[i]->getRequest().getRequestedLocation());
			clients[i]->getResponse()->server = clients[i]->getRequest().getRequestedServer();
			clients[i]->getResponse()->uri= clients[i]->getRequest().getResponseUri();
			clients[i]->getResponse()->http_cookie = clients[i]->getRequest().getCookie();
			clients[i]->getResponse()->query = clients[i]->getRequest().getQueryString();
			clients[i]->getResponse()->fileExtension = clients[i]->getRequest().getFileExtension();
			clients[i]->getResponse()->contentType = clients[i]->getRequest().getContentType();

			tmp = strtod(clients[i]->getResponse()->server->max_body_size.c_str(), &s);
			if (tmp)
				max_body_size = tmp;
			if (clients[i]->getRequest().getBodyCount() > max_body_size)
			{
				clients[i]->setState(ERROR);
				clients[i]->getResponse()->status_code = "413";
			}
		}
	}
}

void Worker::showClients()
{
	for (size_t i = 0; i < clients.size(); i++)
		std::cout << clients[i]->getFd() << "\n";
}

void Worker::checkClientTimeout()
{
	std::vector<Client *>::iterator client = clients.begin();

	while (client != clients.end())
	{
		if ((*client)->istimeOut())
		{
			(*client)->getResponse()->send_errorResponse();
			dropClientConnection(client);
			break;
		}
		client++;
	}
}

Worker::~Worker() 
{
	size_t i = 0;
	std::vector<Client *>::iterator it = clients.begin();

	while (i < clients.size())
	{
		delete clients[i];
		clients.erase(it);
		i++;
		it++;
	}
}
