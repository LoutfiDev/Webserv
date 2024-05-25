#ifndef RQST
#define RQST

#include <cstddef>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "utils.hpp"
#include <climits>

#include "../Parser/location.hpp"

#define NOTEXIST 1
#define UNKNOWN_TRANSFER 2

class Client;
class Request {
	public:
		Request();
		Request(const Request &);
		Request &operator=(const Request &);

		int getRequestCode() const;
		std::string getRsponseCode() const;

		std::string& getRequestLine() const;
		std::map<std::string, std::string>& getHeaders() const;

		std::string getMethodName() const;
		std::string getPath() const;
		std::string getHttpVersion() const;

		int getBodyLength();
		std::string getBody() const;

		Location &getRequestedLocation();
		void setRequestedLocation(std::string &); // the location will sets after
												  // the paresing of the request line

		void addHeader(std::string );
		int  addBody(std::string );

		void showHeaders();
		
		void processRequestLine(std::string);
		void checkRequestLine(std::vector<std::string>&);

		void setChunkLength(std::string);
		
		void parseBody();
		void removeBoundary(std::string);
		~Request();

		// getters for the POST method
	public:
		const std::string &getContentLength();
		const std::string &getTransferEncoding();
		const std::string &getContentType();
		int getBodyCount() const;

	private:
		void setBodyLength(std::string &);
		int readTransferEncodingBody(std::string);

	private:
		int request_code;			// the code that the parsing return
		std::string response_code;	// status code for the response
		bool had_request_line;
		std::map<std::string, std::string> headers;
		std::map<int, int>test;
		std::string method_name;
		std::string path;
		std::string http_version;
		std::string body;
		long long bodyLength;
		long long bodyLength_CPY;
		int chunk_length;
		std::string tmp_body;


		size_t bodyCount;				// to check if it is more then max_size
									// on config file
		// to performe only the check of these attribute only once
	private:
		bool transferEncodingCheck;
		bool contentLengthCheck;
		bool contentTypeCheck;
	
	// attribute for the request location 
	private:
		Location requested_location;

};


#endif //!RQST
