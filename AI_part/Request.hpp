#ifndef RQST
#define RQST

#include <cstddef>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
#include "utils.hpp"
#include <climits>

#include "../Parser/location.hpp"

#define NOTEXIST 1
#define UNKNOWN_TRANSFER 2

#define HOST_EXIST 1

class Client;
class Request {
	public:
		Request();
		Request(const Request &);
		Request &operator=(const Request &);

		int getRequestCode() const;
		void setRequestCode(int);

		std::string getResponseCode() const;
		void setResponseCode(std::string);

		std::string& getRequestLine() const;
		std::map<std::string, std::string>& getHeaders() const;

		bool getIsSet() const;
		std::string getMethodName() const;
		std::string getPath() const;
		std::string getResponseUri() const;
		void setPath(std::string);
		std::string getHttpVersion() const;
		std::string getMatchedLocation() const;
		std::string getCookie() const;
		std::string getQueryString() const;

		std::string getSession() const;

		long long getBodyLength();
		std::string getBodyFile() const;

		Location *getRequestedLocation();
		void setRequestedLocation();

		Server *getRequestedServer();
		void setRequestedServer(std::vector<Server *> );

		int addHeader(std::string );
		int  addBody(std::string &);
		int  ignoreBody(std::string &);

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
		const std::string getContentType();
		const std::string &getHost();
		size_t getBodyCount() const;
		void setFileExtension(std::string &);
		std::string getFileExtension() const;

		void closeTmpBody();
		
		std::string tmp_body_file_name;
		std::string tmp_body_file_extension;


	private:
		void setBodyLength(std::string &);
		int readTransferEncodingBody(std::string);
		int ignoreTransferEncodingBody(std::string);

	private:
		int request_code;			// the code that the parsing return
		std::string response_code;	// status code for the response
		bool had_request_line;
		std::map<std::string, std::string> headers;

		bool isSet;
		std::string method_name;
		std::string path;
		std::string response_uri;
		std::string http_version;

		std::string body;
		std::string host;
		long long bodyLength;
		long long bodyLength_CPY;
		int chunk_length;
		std::string tmp_body;
		std::ofstream tmp_body_file;


		size_t bodyCount;				// to check if it is more then max_size
										// on config file

		// to performe only the check of these attribute only once
	private:
		bool transferEncodingCheck;
		bool contentLengthCheck;
		bool contentTypeCheck;
	
	// attribute for the request location 
	private:
		Location *requested_location;
		Server *requestedServer;

	//cgi paraameter
	private:
		std::string cookie;
		std::string query_string;
};


#endif //!RQST
