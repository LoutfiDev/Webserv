#ifndef MANAGER_SERVER_HPP
#define MANAGER_SERVER_HPP

#include <map>
#include <vector>
#include <sys/epoll.h>
#include <asm-generic/socket.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

#include "Worker.hpp"
#include "utils.hpp"
#include "../Parser/parser.hpp"

#define SOCKET_NOT_AVAILABLE 1

class ServerManager {
	public:
		ServerManager();
		ServerManager(const ServerManager &);
		ServerManager &operator=(const ServerManager &);
		
		void init_server(const char *);
		void makeListen();
		void init_epoll();
		void multiplixer();

		int getSocketFd();
		int getEpollFd();

		void start(Parser &);
		void show();

		bool isNewConnection(int);

		void insert_alredy_known_server(Server );
		void signal_handler();

		~ServerManager();

	private:
		int tmp_socket_fd;
		int epoll_fd;
		std::map<int, std::vector<Server *> > nginx;
		Worker worker;

};

#endif //!MANAGER_SERVER_HPP
