#ifndef CLI
#define CLI

#include "Request.hpp"
#include "../Response/Response.hpp"
#include <sstream>
#include <string>
#include <vector>

#define HEADERISDONE 1

class Client {
	public:
		Client(int, std::vector<Server *>);
		Client(const Client &);
		Client &operator=(const Client &);

		Request getRequest() const;
		void setRequest(const Request &);

		Response *getResponse() const;
		void setResponse(Response *);

		int getFd() const;
		std::vector<Server *> getDataServer();

		int readBuffer(char *);
		void showrequest();

		~Client();

		Response response;
		
	private:
		int fd;
		Request request;
		Response *response;
		std::string buffer;
		std::string leftOver;	//this attribute for the remaining of the 
								//request see readToBuffer()
		int isHeaderPartDone;   // to check if the haeders id done
		// bool canParseBody;		// if method is POST then go and parse body else don't 
		// int bodyLength;
		std::vector<Server *> dataServer;



};

#endif //!CLI
