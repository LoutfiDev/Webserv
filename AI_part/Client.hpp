#ifndef CLI
#define CLI

#include "Request.hpp"
#include "../Response/Response.hpp"
#include <sstream>
#include <string>

#define HEADERISDONE 1

class Client {
	public:
		Client(int);
		Client(const Client &);
		Client &operator=(const Client &);

		const Request &getRequest() const;
		void setRequest(Request &);
		int getFd() const;

		int readBuffer(char *);
		void showrequest();

		~Client();

	private:
		int fd;
		Request request;
		Response response;
		std::string buffer;
		std::string leftOver;	//this attribute for the remaining of the 
								//request see readToBuffer()
		int isHeaderPartDone;   // to check if the haeders id done
		// bool canParseBody;		// if method is POST then go and parse body else don't 
		// int bodyLength;

};

#endif //!CLI