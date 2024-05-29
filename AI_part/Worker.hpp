#ifndef WRKR
#define WRKR

#include <string>
#include <vector>
#include <sys/epoll.h>

#include "Client.hpp"

#define NUMCONNECTION 10

// macros for reading from the client fd
#define CONNECTIONCLOSED -1		// connection closed
#define ERRORINREADING 1		// error on the data that comes from the user
#define READINGISDONE 2			// no error found

// #define HEADERISDONE 1

#define READBUFFER 1024

#define READ 0
#define WRITE 1

class Worker {
	public:
		Worker();
		Worker(const Worker &);
		Worker &operator=(const Worker &);

		void showClients();

		void setServerNames(std::vector<std::string> &);
		std::vector<std::string> getServerNames();

		void add(int, std::vector<Server *> &);
		int serve(int, int);

		std::vector<Client *>::iterator writeToClient(int fd);
		int readFromClient(int fd);

		void dropClientConnection(std::vector<Client *>::iterator );
		~Worker();

	private:
		std::vector<Client *> clients;

};


#endif //!WRKR
