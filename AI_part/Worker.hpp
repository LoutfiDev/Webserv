#ifndef WRKR
#define WRKR

#include <vector>
#include "Client.hpp"

#define NUMCONNECTION 10

// macros for reading from the client fd
#define READINGISDONE 2			//read return -1 with errno set to 11(EAGAIN)
#define ERRORINREADING 1		// error on the data that comes from the user

#define HEADERISDONE 1

#define READBUFFER 10000

class Worker {
	public:
		Worker(const char *);
		Worker(const Worker &);
		Worker &operator=(const Worker &);

		void init_server(const char *);
		void multiplixer();
		void makeListen();
		void init_epoll();
		void showClients();

		int getSocketFd();
		int getEpollFd();

		void dropClientConnection(std::vector<Client>::iterator );
		~Worker();

	private:
		int socket_fd;
		int epoll_fd;
		// int server_port;
		std::vector<Client> clients;

};


#endif //!WRKR
