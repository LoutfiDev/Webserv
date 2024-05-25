#include "Worker.hpp"
#include "Client.hpp"
#include "utils.hpp"
#include <asm-generic/socket.h>
#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <netdb.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <cstring>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <vector>

#include "../Response/Response.hpp"

/*
 * @description initiate my socket_fd vector with a each server having its 
 * own socket FD based on the "port" then bind that socket fd
 *
 * @param name Type and description of the parameter.
 * @return Type and description of the returned value.
 */

void Worker::init_server(const char *port)
{
	struct addrinfo hints;
	struct addrinfo *servinfo;
	struct addrinfo *st_res;
	int res;
	// int reuse = 1;

	memset(&hints, 0, sizeof hints); // make sure the struct is empty
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	res = getaddrinfo(NULL, port, &hints, &servinfo);
	if (res == -1)
		handleError("getaddrinfo", errno);
	for (st_res = servinfo; st_res != NULL; st_res = st_res->ai_next) {
		socket_fd = socket(st_res->ai_family, st_res->ai_socktype, st_res->ai_protocol);
		if (socket_fd == -1)
			continue ;
		res = bind(socket_fd, st_res->ai_addr, st_res->ai_addrlen);
		if (res == 0)
			break ;
	}
	if (st_res == NULL)
	{
		std::cerr << "could not create a socket\n";
		std::exit(-1);
	}
	freeaddrinfo(servinfo);
	// res = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
	// if (res == -1)
	// 	handleError("setsockopt", errno);
	fcntl(socket_fd, F_SETFL, O_NONBLOCK);
	std::cout << "Listening on port: " << port << "\n";
}


/*
 * @description mark my socket_fd vector as a listening socket
 *
 * @param void 
 * @return void
 */

void Worker::makeListen()
{
	int listen_res;
	listen_res = listen(socket_fd, NUMCONNECTION);
	if (listen_res == -1)
	{
		close(socket_fd);
		handleError("listen", errno);
	}
}

/*
 * @description initilate epoll instance 
 *
 * @param void  
 * @return void
 */

void Worker::init_epoll()
{
	int epoll_ctl_check;
	struct epoll_event ev;

	ev.events = EPOLLIN;
	ev.data.fd = socket_fd;

	epoll_fd = epoll_create(1);
	if (epoll_fd == -1)
	{
		close(socket_fd);
		handleError("epoll_create", errno);
	}

	epoll_ctl_check = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &ev);
	if (epoll_ctl_check == -1)
	{
		close(epoll_fd);
		close(socket_fd);
		handleError("epoll_ctl", errno);
	}
}

/*
 * @Description : read request (header + body) from client
 * @param 1 fd int the fd to read from
 * @param 2 _clients vector to see which client to read his data and parse it
 * 
 * @return int 0 = raed is done | -1 = error
 *
 */
int readFromClient(int fd, std::vector<Client> &_clients)
{
	int request_res;
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i].getFd() == fd)
		{
			int read_size;
			char buf[READBUFFER];
			std::memset(buf, '\0', READBUFFER - 1);
			read_size = read(fd, buf, READBUFFER - 2);
			buf[READBUFFER - 1] = '\0';
			if (read_size == -1)
			{
				std::cerr << "Connection is Closed by peer\n";
				close(fd);
				return -1;
			}
			if (read_size == 0)
				return (std::cout << "request is done\n", 2);
			request_res = _clients[i].readBuffer(buf);
			// std::cout << "request_res => " << request_res << "\n";
			if (request_res > 0)
			{
				std::cout << "error in the request\n";
				return 0;
			}
			if (request_res == -1)
				return 2;
		}
	}
	return 3;
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

std::vector<Client>::iterator writeToClient(int fd, std::vector<Client> &_clients)
{
	std::vector<Client>::iterator c_beg = _clients.begin();
	int status;
	// static int len = 60;
	// static int offset = 0;

	while (c_beg != _clients.end())
	{
		if (c_beg->getFd() == fd)
		{
			// c_beg->showrequest();
			char resp[61] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello, world!\r\n";
			status = write(fd, resp , std::strlen(resp));
			if (status == -1)
			{
				std::cerr << "Client has closed the Connection\n";
				return c_beg;
			}
			return c_beg;
			// if (status)
			// 	break;
			// if (status == 0)
			// 	return c_beg;
		}
		c_beg++;
	}
	return _clients.end();
}

void Worker::dropClientConnection(std::vector<Client>::iterator client)
{
	if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client->getFd(), NULL) == -1)
		std::cout << "cannot remove client\n";
	close(client->getFd());
	clients.erase(client);
}

/*
 * @description create the multiplixing loop using epoll()
 *
 * @param void
 * @return void
 */

void Worker::multiplixer()
{
	int client_fd;
	struct sockaddr_in accept_addr;
	socklen_t accept_len;
	struct epoll_event epl_evt[NUMCONNECTION], ev;
	int num_client;

	accept_len = sizeof(accept_addr);
	while (1)
	{
		num_client = epoll_wait(epoll_fd, epl_evt, NUMCONNECTION, -1);
		if (num_client == -1)
		{
			close(epoll_fd);
			close(socket_fd);
			handleError("epoll_event", errno);
		}
		for (int i = 0; i < num_client; i++)
		{
			if (epl_evt[i].data.fd == socket_fd)
			{
				std::cout << "New Client\n";
				client_fd = accept(socket_fd, (struct sockaddr *)&accept_addr, &accept_len);
				if (client_fd == -1)
				{
					close(socket_fd);
					close(epoll_fd);
					handleError("accept", errno);
				}
				fcntl(client_fd, F_SETFL, O_NONBLOCK);
				ev.events = EPOLLIN;
				ev.data.fd = client_fd;
				epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev);
				clients.push_back(Client(client_fd));
			}
			else
			{
				// std::cout << "fd : " << client_fd << "\n";
				int test = 0;
				std::vector<Client>::iterator cli;
				if (epl_evt[i].events & EPOLLIN)
				{
					// std::cout << "IN\n";
					test = readFromClient(epl_evt[i].data.fd, clients);
					// std::cout << "test = " << test << "\n";
					if (test == ERRORINREADING)
					{
						std::cerr << "error while reading request\n";
						close(epl_evt[i].data.fd);
					}
					if (test == READINGISDONE || test == 0)
					{
						// std::cout << "OK\n";
						ev.data.fd = epl_evt[i].data.fd;
						ev.events = EPOLLOUT;
						epoll_ctl(epoll_fd, EPOLL_CTL_MOD, epl_evt[i].data.fd, &ev);
						continue;
					}

				}
				else if (epl_evt[i].events & EPOLLOUT)
				{
					cli = writeToClient(epl_evt[i].data.fd, clients);
					if (cli != clients.end())
						dropClientConnection(cli);
				}
				else
					std::cout << "i Dont Know\n";
				// epoll_ctl(epl_connection, EPOLL_CTL_DEL, client_fd, NULL);
				//
				// close(client_fd);
			}
		}
	}
}

int Worker::getSocketFd()
{
	return socket_fd;
}

int Worker::getEpollFd()
{
	return epoll_fd;
}

void Worker::showClients()
{
	for (size_t i = 0; i < clients.size(); i++)
	{
		clients[i].showrequest();
	}
}

Worker::Worker(const char *port) {
	init_server(port);
	makeListen();
	init_epoll();
	multiplixer();
}

Worker::Worker(const Worker& obj) {
	(void)obj;
}

Worker &Worker::operator=(const Worker& obj)
{
	if (this == &obj)
		return (*this);
	return (*this);
}

Worker::~Worker() {
}
