#ifndef CLI
#define CLI

#include "Request.hpp"
#include "../Response/Response.hpp"
#include <sstream>
#include <string>
#include <vector>

#define HEADERISDONE 1

#define READ 0
#define WRITE 1
#define ERROR 2

#define TIMEOUT 3

class Client {
	public:
		Client(int, std::vector<Server *>);
		Client(const Client &);
		Client &operator=(const Client &);

		Request &getRequest();
		void setRequest(const Request &);

		Response *getResponse() const;
		void setResponse(Response *);

		int getState() const;
		void setState(int);

		int getFd() const;
		std::vector<Server *> getDataServer();

		void readBuffer(char *);
		void showrequest();

		bool istimeOut();

		~Client();
		
	private:
		int fd;
		int state;
		Request request;
		Response *response;
		std::string buffer;
		std::string leftOver;	//this attribute for the remaining of the 
								//request see readToBuffer()
		int isHeaderPartDone;   // to check if the haeders id done
		// bool canParseBody;		// if method is POST then go and parse body else don't 
		// int bodyLength;
		std::vector<Server *> dataServer;
		time_t c_timer_start;



};

#endif //!CLI
