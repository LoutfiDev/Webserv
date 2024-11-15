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
#define NOTHING 0

// #define HEADERISDONE 1

class Worker {
	public:
		Worker();
		Worker(const Worker &);
		Worker &operator=(const Worker &);

		void showClients();

		void setServerNames(std::vector<std::string> &);
		std::vector<std::string> getServerNames();

		void add(int, std::vector<Server *> &);
		int serve(int);
		void initResponse(int);
		void checkClientTimeout();

		void dropClientConnection(std::vector<Client *>::iterator );

		~Worker();
	
		//utils functions no need to be public
	private:
		bool writeToClient(std::vector<Client *>::iterator);
		int readFromClient(int, std::vector<Client *>::iterator);

	private:
		std::vector<Client *> clients;
		std::vector<Client *> old_clients;

};


#endif //!WRKR
