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

		void init_dataServer(const std::vector<Server *> &);

		Request &getRequest();
		void setRequest(const Request &);

		Response *getResponse() const;
		void setResponse(Response *);

		int getState() const;
		void setState(int);

		void  resetTimer();

		int getFd() const;
		std::vector<Server *> getDataServer();

		void readBuffer(char *, int);
		void showrequest();

		bool istimeOut();
		// bool getIgnoreTimer() const;
		void setIgnoreTimer(bool);

		~Client();

	private:
		void setResponseAttribut();
		
	private:
		int requestLine;
		int fd;
		int state;
		Request request;
		Response *response;
		std::string buffer;

		int isHeaderPartDone;   // to check if the haeders id done
		// bool canParseBody;		// if method is POST then go and parse body else don't 
		// int bodyLength;
		std::vector<Server *> dataServer;

	private:
		time_t c_timer_start;
		bool ignoretimer;



};

#endif //!CLI
