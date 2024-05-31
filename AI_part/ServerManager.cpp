#include "ServerManager.hpp"
#include "Worker.hpp"
#include "utils.hpp"
#include <cerrno>
#include <cstddef>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <unistd.h>
#include <utility>
#include <vector>


ServerManager::ServerManager() {
}

ServerManager::ServerManager(const ServerManager& obj) {
	(void)obj;
}

ServerManager &ServerManager::operator=(const ServerManager& obj)
{
	if (this == &obj)
		return (*this);
	return (*this);
}

int ServerManager::getSocketFd()
{
	return tmp_socket_fd;
}

int ServerManager::getEpollFd()
{
	return epoll_fd;
}

/*
 * @description initiate my socket_fd vector with a each server having its 
 * own socket FD based on the "port" then bind that socket fd
 *
 * @param name Type and description of the parameter.
 * @return Type and description of the returned value.
 */

void ServerManager::init_server(const char *port)
{
	struct addrinfo hints;
	struct addrinfo *servinfo;
	struct addrinfo *st_res;
	int res;
	int reuse = 1;

	memset(&hints, 0, sizeof hints); // make sure the struct is empty
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	res = getaddrinfo(NULL, port, &hints, &servinfo);
	if (res == -1)
		handleError("getaddrinfo", errno);
	for (st_res = servinfo; st_res != NULL; st_res = st_res->ai_next) {
		tmp_socket_fd = socket(st_res->ai_family, st_res->ai_socktype, st_res->ai_protocol);
		if (tmp_socket_fd == -1)
			continue ;
		res = bind(tmp_socket_fd, st_res->ai_addr, st_res->ai_addrlen);
		if (res == 0)
			break ;
	}
	if (st_res == NULL)
	{
		freeaddrinfo(servinfo);
		close(tmp_socket_fd);
		throw SOCKET_NOT_AVAILABLE;
	}
	freeaddrinfo(servinfo);
	res = setsockopt(tmp_socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
	if (res == -1)
		handleError("setsockopt", errno);
	fcntl(tmp_socket_fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	makeListen();
	std::cout << "Listening on port: " << port << "....\n";
}

/*
 * @description mark my socket_fd vector as a listening socket
 *
 * @param void 
 * @return void
 */

void ServerManager::makeListen()
{
	int listen_res;
	listen_res = listen(tmp_socket_fd, NUMCONNECTION);
	if (listen_res == -1)
	{
		close(tmp_socket_fd);
		handleError("listen", errno);
	}
}

/*
 * @description initilate epoll instance 
 *
 * @param void  
 * @return void
 */

void ServerManager::init_epoll()
{
	int epoll_ctl_check;
	struct epoll_event ev;
	std::map<int, std::vector<Server *> >::iterator beg = nginx.begin();

	epoll_fd = epoll_create(1);
	if (epoll_fd == -1)
	{
		this->~ServerManager();	
		handleError("epoll_create", errno);
	}

	while (beg != nginx.end())
	{
		ev.events = EPOLLIN;
		ev.data.fd = beg->first;
		epoll_ctl_check = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, beg->first, &ev);
		if (epoll_ctl_check == -1)
		{
			close(epoll_fd);
			this->~ServerManager();
			handleError("epoll_ctl", errno);
		}
		beg++;
	}
}

bool ServerManager::isNewConnection(int fd)
{
	std::map<int, std::vector<Server *> >::iterator beg = nginx.begin();
	int newClient;
	struct sockaddr_in accept_addr;
	socklen_t accept_len;
	struct epoll_event ev;

	accept_len = sizeof(accept_addr);
	for (; beg != nginx.end(); beg++) {

		if (fd == beg->first)
		{
			std::cout << "New Client\n";
			newClient = accept(beg->first, (struct sockaddr *)&accept_addr, &accept_len);
			if (newClient == -1)
			{
				this->~ServerManager();
				handleError("accept", errno);
			}
			fcntl(newClient, F_SETFL, O_NONBLOCK);
			ev.events = EPOLLIN;
			ev.data.fd = newClient;
			epoll_ctl(epoll_fd, EPOLL_CTL_ADD, newClient, &ev);
			worker.add(newClient, nginx[beg->first]);
			return true;
		}
	}
	return false;
}

/*
 * @description create the multiplixing loop using epoll()
 *
 * @param void
 * @return void
 */

void ServerManager::multiplixer()
{
	struct epoll_event epl_evt[NUMCONNECTION], ev;
	int num_event;
	int state;

	while (1)
	{
		num_event = epoll_wait(epoll_fd, epl_evt, NUMCONNECTION, -1);
		if (num_event == -1)
		{
			this->~ServerManager();
			handleError("epoll_event", errno);
		}
		for (int i = 0; i < num_event; i++)
		{
			if (!isNewConnection(epl_evt[i].data.fd))
			{
				if (epl_evt[i].events & EPOLLIN)
				{
					state = worker.serve(epl_evt[i].data.fd);
					if (state == CONNECTIONCLOSED)
					{
						std::cerr << "error while reading request\n";
						close(epl_evt[i].data.fd);
					}
					else if (state == READINGISDONE  || state == ERRORINREADING)
					{
						worker.setClientResponse(epl_evt[i].data.fd);
						ev.data.fd = epl_evt[i].data.fd;
						ev.events = EPOLLOUT;
						epoll_ctl(epoll_fd, EPOLL_CTL_MOD, epl_evt[i].data.fd, &ev);
						continue;
					}

				}
				else if (epl_evt[i].events & EPOLLOUT)
				{
					worker.serve(epl_evt[i].data.fd);
				}
			}
		}
	}
}


void ServerManager::show()
{
	std::map<int, std::vector<Server *> >::iterator beg = nginx.begin();
	while (beg != nginx.end()) {
		std::cout << beg->first << "=> ";
		std::vector<Server *> ser = beg->second; 
		for (size_t i = 0; i < ser.size(); i++) {
			std::cout << " - ";
			for (size_t j = 0; j < ser[i]->server_names.size(); j++) {
				std::cout << ser[i]->server_names[j] << "\t";
			}
		}
		std::cout << "\n";
		beg++;
	}
}

bool findPort(std::vector<Server *> workrs, std::string port)
{
	for (size_t i = 0; i < workrs.size(); i++) {
		if (workrs[i]->port == port)
			return true;
	}
	return false;
}


void ServerManager::insert_alredy_known_server(Server server)
{
	std::map<int, std::vector<Server *> >::iterator beg = nginx.begin();

	while (beg != nginx.end())
	{
		if (findPort(beg->second, server.port))
			beg->second.push_back(new Server(server));
		beg++;
	}
}

void ServerManager::start(Parser &config)
{
	std::vector<Server *> servers;

	servers = config.get_servers();
	for (size_t i = 0; i < servers.size(); i++) {
		try {
			init_server(servers[i]->port.c_str());
			nginx[tmp_socket_fd].push_back(new Server(*servers[i]));
		}
		catch (int errorCode) {
			if (errorCode == SOCKET_NOT_AVAILABLE)
				insert_alredy_known_server(*servers[i]);
		}
	}
	init_epoll();
	multiplixer();
}

ServerManager::~ServerManager() {
	std::map<int, std::vector<Server *> >::iterator beg = nginx.begin();

	while (beg != nginx.end()) {
		close(beg->first);
		int j = 0;
		for (std::vector<Server *>::iterator i = beg->second.begin(); i < beg->second.end(); i++) {
			delete beg->second[j];
			beg->second.erase(i);
			j++;
		}
		beg++;	
	}
}
