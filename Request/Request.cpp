#include "Request.hpp"
#include "utils.hpp"
#include <climits>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <fcntl.h>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <istream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

Request::Request() {
	had_request_line = false;
	request_code = 0;
	bodyLength = 0;
	response_code = "200";
	bodyCount = 0;
	transferEncodingCheck = false;
	contentLengthCheck = false;
	contentTypeCheck = false;
	chunk_length = 0;
	host = "";
	cookie = "";
	query_string = "";
	requestedServer = NULL;
	requested_location = NULL;
	tmp_body_file_extension = "";
	isSet = false;
}

Request::Request(const Request &obj)
{
	requested_location = new Location();
	requestedServer = new Server();
	*this = obj;
}

Request &Request::operator=(const Request &obj) {
	if (this == &obj)
		return (*this);
	isSet = obj.isSet;
	request_code = obj.request_code;
	response_code = obj.response_code;
	had_request_line = obj.had_request_line;
	headers = obj.headers;
	method_name = obj.method_name;
	path = obj.path;
	http_version = obj.http_version;
	body = obj.body;
	bodyLength = obj.bodyLength;
	bodyLength_CPY = obj.bodyLength_CPY;
	chunk_length = obj.chunk_length;
	tmp_body = obj.tmp_body;
	bodyCount = obj.bodyCount;
	host = obj.host;
	query_string = obj.query_string;
	cookie = obj.cookie;

	transferEncodingCheck = obj.transferEncodingCheck;
	contentLengthCheck = obj.contentLengthCheck;
	contentTypeCheck = obj.contentTypeCheck;

	*requested_location = *obj.requested_location;
	*requestedServer = *obj.requestedServer;
	tmp_body_file_extension = obj.tmp_body_file_extension;
	return (*this);
}

int Request::getRequestCode() const { return request_code; }

void Request::setRequestCode(int code) { request_code = code; }

std::string Request::getResponseCode() const { return response_code; }

void Request::setResponseCode(std::string code) {
	std::istringstream num(code);

	num >> request_code;
	response_code = code;
}

bool Request::getIsSet() const { return isSet; }

std::string Request::getMethodName() const { return method_name; }

std::string Request::getPath() const { return path; }

std::string Request::getResponseUri() const { return response_uri; }

void Request::setPath(std::string uri) {
	size_t pos;
	std::string tmp;

	while ((pos = uri.find("//")) != std::string::npos) {
		tmp = uri.substr(0, pos);
		uri = uri.substr(pos + 1, uri.length());
		tmp += uri;
		uri = tmp;
	}
	path = uri;
}

std::string Request::getHttpVersion() const { return http_version; }

std::string Request::getBodyFile() const { return tmp_body_file_name; }

size_t Request::getBodyCount() const { return bodyCount; }

long long Request::getBodyLength() {
	if (bodyLength < 0)
		setResponseCode("400");
	return bodyLength;
}

Location *Request::getRequestedLocation() { return requested_location; }

Server *Request::getRequestedServer() { return requestedServer; }

const std::string &Request::getHost() {
	if (host.length() == 0)
		setResponseCode("400");
	return host;
}

std::string Request::getFileExtension() const { return tmp_body_file_extension;}

std::string Request::getCookie() const { return cookie; }

std::string Request::getQueryString() const { return query_string; }

void Request::setChunkLength(std::string token) {
	std::istringstream num(token);

	num >> std::hex >> chunk_length;
	if (chunk_length == 0)
		chunk_length = -1;
	else if (chunk_length < 0)
		chunk_length = -2;
}

void Request::closeTmpBody() { tmp_body_file.close(); }

void Request::setBodyLength(std::string &number) {
	char *s;

	bodyLength = strtod(number.c_str(), &s);
	if (std::strlen(s) != 0 || bodyLength > LLONG_MAX - 1 || bodyLength < 0) {
		bodyLength = -1;
		setResponseCode("400");
	}
	bodyLength_CPY = bodyLength;
}


void Request::setRequestedServer(std::vector<Server *> servers) {
	if (servers.size())
		requestedServer = servers[0];
	for (size_t i = 0; i < servers.size(); i++) {
		if (servers[i]->host == host) {
			requestedServer = servers[i];
			break;
		}
	}
}

void Request::setFileExtension(std::string &file)
{
	bool fnd = 0;
	std::string tmp, tmp1;
	std::ifstream 	inputFile("Response/mime.types");
	while(std::getline(inputFile, tmp))
	{
		if (tmp.find(file) != std::string::npos)
		{
			std::stringstream ss(tmp);
			while(std::getline(ss, tmp1, ' '))
			{
				if (!tmp1.size())
					continue;
				if (tmp1 == file && !fnd)
					fnd = true;
				else
				{
					tmp_body_file_extension = trim(tmp1, " \t\r\n;,:");
					tmp_body_file_extension.insert(0, ".");
					inputFile.close();
					return;
				}
			}
		}
	}
	inputFile.close();
	tmp_body_file_extension = "";
}

void Request::setRequestedLocation() {
	std::string uri;
	std::map<std::string, Location *> *l = &requestedServer->locations;
	std::map<std::string, Location *>::iterator beg;
	std::string finale_path;
	std::string location_name;
	size_t query_pos;

	uri = path;
	for (beg = l->begin(); beg != l->end(); beg++) {
		if (uri.compare(0, beg->first.length(), beg->first) == 0) {
			requested_location = new Location(*beg->second);
			location_name = beg->first;
		}
	}
	if (location_name.length() == 0) {
		setResponseCode("400");
		return;
	}
	uri = getUri(path, location_name);
	finale_path = removeLastChar(requested_location->root) + uri;
	response_uri = path;
	query_pos = finale_path.find("?");
	if (query_pos != std::string::npos) {
		query_string = finale_path.substr(query_pos + 1);
		finale_path = finale_path.substr(0, query_pos);
	}
	path = finale_path;
}

/*
 * @description the check for my request line
 *
 * @param vector of strings the fields of my request line after the split
 * @return void
 */

void Request::checkRequestLine(std::vector<std::string> &attrs) {
	isSet = true;
	if (attrs.size() != 3) {
		setResponseCode("400");
		return;
	}
	method_name = attrs[0];
	setPath(attrs[1]);
	http_version = attrs[2];

	if (method_name != "GET" && method_name != "POST" && method_name != "DELETE")
	{
		std::cout << method_name<< " i set\n";
		setResponseCode("405");
	}
	else if (http_version.compare("HTTP/1.1"))
		setResponseCode("505");
}

/*
 * @description parse the first line
 *
 * @param tocken tipycally a line with 'METHOD PATY HTTP_VERSIOn'
 * @return void
 */

void Request::processRequestLine(std::string token) {
	std::stringstream strm(token);
	std::vector<std::string> strs;
	std::string chunk;

	while (getline(strm, chunk, ' '))
		strs.push_back(chunk);
	had_request_line = true;
	checkRequestLine(strs);
}

/*
 * @description get each line and parse it
 *
 * @param token the line (end's with \r\n) that i read from the client
 * @return int 0 : succes | 1 : if host haeder is exist
 */

int Request::addHeader(std::string token) {
	size_t found;
	std::string key, value;

	if (!had_request_line)
		processRequestLine(token);
	else {
		found = token.find(":");
		if (found == std::string::npos) {
			setResponseCode("400");
			return 0;
		}
		key = trim(token.substr(0, found));
		value = trim(token.substr(found + 1));
		if (key.length() == 0 || value.length() == 0) {
			setResponseCode("400");
			return 0;
		}
		if (key == "Cookie")
			cookie = value;
		// toLower(key);
		// toLower(value);
		headers[key] = value;
		if (key == "Content-Length")
			setBodyLength(value);
		if (key == "Content-Type")
			setFileExtension(value);
		else if (key == "Host") {
			host = value;
			return HOST_EXIST;
		}
	}
	return 0;
}

int Request::ignoreBody(std::string &token) {
	try {
		if (!transferEncodingCheck) {

			getTransferEncoding();
			if (request_code)
				return request_code;
			transferEncodingCheck = true;
		}
		if (ignoreTransferEncodingBody(token) == -1)
			return -1;
	} catch (int state) {
		if (state == -1)
			return 0;
		try {
			if (!contentLengthCheck) {
				getContentLength();
				contentLengthCheck = true;
			}
			bodyLength_CPY -= token.size();
			bodyCount += token.size();
			if (bodyLength_CPY <= 0)
				return -1;
		} catch (int) {
			return -1;
		}
	}
	return 0;
}

/*
 * @Description : read that buffer into the body attribute and parse it
 * @param token string buffer from client
 *
 * @return int 0:no error in token | 1:reading is done | -1: error
 *
 */

int Request::addBody(std::string &token) {
	if (method_name != "POST")
		return ignoreBody(token);
	if (!tmp_body_file.is_open()) {
		tmp_body_file_name = "./" + generateFileName() + tmp_body_file_extension;
		tmp_body_file.open(tmp_body_file_name.c_str(), std::istream::binary);
	}
	try {
		if (!transferEncodingCheck) {
			getTransferEncoding();
			if (request_code)
				return request_code;
			transferEncodingCheck = true;
		}
		if (readTransferEncodingBody(token) == -1) {
			tmp_body_file.close();
			return -1;
		}
	} catch (int state) {
		if (state == -1)
			return 0;
		try {
			if (!contentLengthCheck)
				getContentLength();
			contentLengthCheck = true;
			tmp_body_file.write(token.c_str(), token.size());
			bodyLength_CPY -= token.size();
			bodyCount += token.size();
			if (bodyLength_CPY <= 0)
				return (tmp_body_file.close(), -1);
		} catch (int) {
			tmp_body_file.close();
			return -1;
		}
	}
	return 0;
}

void Request::showHeaders() {
	std::map<std::string, std::string>::iterator beg = headers.begin();

	std::cout << "Method = " << method_name << "| location = ";
	std::cout << path << "| HTTP Version = " << http_version << "\n";
	while (beg != headers.end()) {
		std::cout << beg->first << " => " << beg->second << "\n";
		beg++;
	}
}

/*
 * @Description : read and parse body (i think) with the Transfer-Encoding
 *				  is set to "chunked"
 * @param token string the token that i read from Client
 *
 * @return int 0: no error found | 1: reading is done
 *
 */

int Request::readTransferEncodingBody(std::string token) {
	size_t line;
	tmp_body += token;

	while (tmp_body.length()) {
		line = tmp_body.find("\r\n");
		if (line == std::string::npos)
			return 0;
		if (line == 0) {
			tmp_body = tmp_body.substr(2);
			continue;
		}
		if (chunk_length == 0) {
			setChunkLength(tmp_body.substr(0, line));
			if (chunk_length == -1) {
				tmp_body_file << "\r\n";
				return -1;
			}
			if (chunk_length == -2) {
				setResponseCode("408");
				return -1;
			}
			tmp_body = tmp_body.substr(line + 2);
		} else {
			if (chunk_length > (int)tmp_body.length()) {
				tmp_body_file.write(tmp_body.c_str(), tmp_body.length());
				chunk_length -= tmp_body.length();
				tmp_body.clear();
			} else {
				tmp_body_file.write(tmp_body.substr(0, chunk_length).c_str(),
						tmp_body.substr(0, chunk_length).length());
				tmp_body = tmp_body.substr(chunk_length);
				chunk_length = 0;
				tmp_body_file << "\r\n";
			}
			bodyCount += tmp_body.length();
		}
	}
	return 0;
}

int Request::ignoreTransferEncodingBody(std::string token) {
	size_t line;
	tmp_body += token;

	while (tmp_body.length()) {
		line = tmp_body.find("\r\n");
		if (line == std::string::npos)
			return 0;
		if (line == 0) {
			tmp_body = tmp_body.substr(2);
			continue;
		}
		if (chunk_length == 0) {
			setChunkLength(tmp_body.substr(0, line));
			if (chunk_length == -1)
				return -1;
			if (chunk_length == -2) {
				setResponseCode("408");
				return -1;
			}
			tmp_body = tmp_body.substr(line + 2);
		} else {
			if (chunk_length > (int)tmp_body.length()) {
				chunk_length -= tmp_body.length();
				tmp_body.clear();
			} else {
				tmp_body = tmp_body.substr(chunk_length);
				chunk_length = 0;
			}
			bodyCount += tmp_body.length();
		}
	}
	return 0;
}
/*
 * @Description : check the type of that body and performe any needed cleanup
 * @param void
 *
 * @return void
 *
 */

void Request::parseBody() {
	std::string type;
	try {
		if (contentTypeCheck == false) {
			type = getContentType();
			contentTypeCheck = true;
		}
		if (type.find("multipart/form-data; boundary=") != std::string::npos)
			removeBoundary(type);
	} catch (int) {
	}
}

void Request::removeBoundary(std::string boundary) {
	size_t pos = boundary.find('=');

	if (pos == std::string::npos)
		return;
	boundary = boundary.substr(pos + 1);
	boundary += "\r\n";
	tmp_body = body;
	body = "";
	while (tmp_body.length()) {
		pos = tmp_body.find(boundary);
		if (pos == std::string::npos)
			break;
		if (pos == 0)
			tmp_body.erase(0, boundary.length());
		else {
			body += tmp_body.substr(0, pos);
			tmp_body.erase(0, pos);
		}
	}
	std::cout << '<' << body << '>';
}

/*
 * @Description : POST getters for Content-Length and Content-Type
 * @param void
 *
 * @return string if there is that field in my headers map or throw a NOTEXIST
 *
 */

const std::string &Request::getContentLength() {
	std::map<std::string, std::string>::iterator value;

	value = headers.find("Content-Length");
	if (value == headers.end())
		throw NOTEXIST;
	return (value->second);
}

const std::string Request::getContentType() {
	std::map<std::string, std::string>::iterator value;

	value = headers.find("Content-Type");
	if (value == headers.end())
		return "";
	return (value->second);
}

const std::string &Request::getTransferEncoding() {
	std::map<std::string, std::string>::iterator value;

	value = headers.find("Transfer-Encoding");
	if (value == headers.end())
		throw NOTEXIST;
	try {
		getContentLength();
		setResponseCode("400");
	} catch (int) {
		if (value->second != "chunked") {
			setResponseCode("501");
			throw -1;
		}
	}
	return (value->second);
}

Request::~Request() {
	std::cout << "Request Distructor \n";
	if (requested_location)
		delete requested_location;
	if (requestedServer)
		delete requestedServer;
	tmp_body_file.close();
}
