#include "Request.hpp"
#include "utils.hpp"
#include <climits>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <iomanip>
#include <ios>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/types.h>
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
	host =  "";
}

Request::Request(const Request& obj) {
	*this = obj;
}

Request &Request::operator=(const Request& obj)
{
	if (this == &obj)
		return (*this);
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

	transferEncodingCheck = obj.transferEncodingCheck;
	contentLengthCheck = obj.contentLengthCheck;
	contentTypeCheck = obj.contentTypeCheck;

	requested_location = obj.requested_location;
	requestedServer = obj.requestedServer;
	return (*this);
}

int Request::getRequestCode() const
{
	return request_code;
}

std::string Request::getRsponseCode() const
{
	return response_code;
}

std::string  Request::getMethodName() const
{
	return method_name;
}

std::string  Request::getPath() const
{
	return path;
}

std::string  Request::getHttpVersion() const
{
	return http_version;
}

std::string Request::getBody() const
{
	return body;
}

int Request::getBodyCount() const
{
	return bodyCount;
}

int Request::getBodyLength()
{
	return bodyLength;
}

Location &Request::getRequestedLocation()
{
	return requested_location;
}

const std::string &Request::getHost()
{
	return host;
}

void Request::setChunkLength(std::string token)
{
	std::istringstream num(token);
	
	num >> std::hex >> chunk_length;
	if (chunk_length == 0)
		chunk_length = -1;

}

void Request::setBodyLength(std::string &number)
{
	char *s;

	bodyLength = strtod(number.c_str(), &s);
	if (std::strlen(s) != 0 || bodyLength > LLONG_MAX - 1 || bodyLength < 0)
	{
		bodyLength = -1;
		request_code = 400;
		response_code = "400";
	}
	bodyLength_CPY = bodyLength;
}

void Request::setRequestedServer(std::vector<Server *> servers)
{
	std::cout << servers.size() <<"\n";
	requestedServer = *servers[0];
	for (size_t i = 0; i < servers.size(); i++) {
		if (servers[i]->host == host)
		{
			requestedServer = *servers[i];
			return;
		}
	}
}

void Request::setRequestedLocation(std::string uri)
{
	std::map<std::string, Location *> l = requestedServer.locations;
	Location *test = NULL;
	std::map<std::string, Location *>::iterator beg;

	beg = l.find(uri);
	if (beg != l.end())
	{
		requested_location = *beg->second;
		return;
	}
	beg = l.begin();
	l[uri] = test;
	requested_location = *(beg->second);
	while (beg != l.end())
	{
		if (beg->first == uri)
		{
			requested_location = *(--beg)->second;
			return;
		}
		beg++;
	}
}

/*
 * @description the check for my request line
 *
 * @param vector of strings the fields of my request line after the split
 * @return void
 */

void Request::checkRequestLine(std::vector<std::string> &attrs)
{
	if (attrs.size() != 3)
	{
		// std::cout << attrs[0] << "\n";
		response_code = "400";
		request_code = 400;
		return;
	}
	method_name = attrs[0];
	path = attrs[1];
	http_version = attrs[2];

	if (method_name != "GET" && method_name != "POST" && method_name != "DELETE")
	{
		response_code = "405";
		request_code = 405;
	}
	else if (http_version != "HTTP/1.1")
	{
		response_code = "505";
		request_code = 505;
	}
}

/*
 * @description parse the first line 
 *
 * @param tocken tipycally a line with 'METHOD PATY HTTP_VERSIOn'
 * @return 0 : succes | 1 : bad request name | 2 : bad http version
 */

void Request::processRequestLine(std::string token)
{
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
 * @return int 0 : succes | 1 : error on the request so end the parsing here
 */

int Request::addHeader(std::string token)
{
	size_t found;
	std::string key, value;

	if (!had_request_line)
		processRequestLine(token);
	else 
	{
		found = token.find(":");
		if (found == std::string::npos)
		{
			response_code = "400";
			request_code = 400;
			return 0;
		}
		key = trim(token.substr(0, found));
		value = trim(token.substr(found + 1));
		if (key.length() == 0 || value.length() == 0)
		{
			response_code = "400";
			request_code = 400;
			return 0;
		}
		toLower(key);
		toLower(value);
		headers[key] = value;
		if (key == "content-length")
			setBodyLength(value);
		if (key == "host")
		{
			host = value;
			return HOST_EXIST;
		}
	}
	return 0;
}

int Request::addBody(std::string token)
{
	try{
		if (!transferEncodingCheck)
		{
			getTransferEncoding();
			transferEncodingCheck = true;
		}
		if (readTransferEncodingBody(token) == -1)
			return -1;
	}
	catch (int state)
	{
		if (state == -1)
			return 0;
		try
		{
			if (!contentLengthCheck)
			{
				getContentLength();
				contentLengthCheck = true;
			}
			body += token;
			bodyLength_CPY -= token.length();
			bodyCount += token.length();
			if (bodyLength_CPY <= 0)
				return -1;
		}
		catch (int)
		{
			return -1;
		}
	}
	return 0;
}

void Request::showHeaders()
{
	std::map<std::string, std::string>::iterator beg = headers.begin();

	std::cout << "Method = " << method_name << "| location = " ;
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
 * @return void
 *
 */

int Request::readTransferEncodingBody(std::string token)
{
	size_t line;
	tmp_body += token;
	std::cout << token << "\n=> "<< token.length() << "\n";

	while (tmp_body.length())
	{
		line = tmp_body.find("\r\n");
		if (line == std::string::npos)
			return 0;
		if (line == 0)
		{
			tmp_body = tmp_body.substr(2);
			continue;
		}
		if (chunk_length == 0)
		{
			setChunkLength(tmp_body.substr(0, line));
			if (chunk_length == -1)
			{
				body += "\r\n";
				return -1;
			}
			tmp_body = tmp_body.substr(line + 2);
		}
		else
		{
			if (chunk_length > (int)tmp_body.length())
			{
				body += tmp_body;
				chunk_length -= tmp_body.length();
				tmp_body.clear();
			}
			else
			{
				body += tmp_body.substr(0, chunk_length);
				tmp_body = tmp_body.substr(chunk_length);
				chunk_length = 0;
				body += "\r\n";
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

void Request::parseBody()
{
	std::string type;
	try
	{
		if (contentTypeCheck == false)
		{
			type = getContentType();
			contentTypeCheck = true;
		}
		if (type.find("multipart/form-data; boundary=") != std::string::npos)
			removeBoundary(type);
	}
	catch (int) {
	}
}

void Request::removeBoundary(std::string boundary)
{
	size_t pos = boundary.find('=');

	if (pos == std::string::npos)
		return;
	boundary = boundary.substr(pos + 1);
	boundary += "\r\n";
	tmp_body = body;
	body = "";
	while (tmp_body.length())
	{
		pos = tmp_body.find(boundary);
		if (pos == std::string::npos)
			break;
		if (pos == 0)
			tmp_body.erase(0, boundary.length());
		else
		{
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

const std::string &Request::getContentLength()
{
	std::map<std::string, std::string>::iterator value;

	value = headers.find("Content-Length");
	if (value == headers.end())
		throw NOTEXIST;
	return (value->second);
}

const std::string &Request::getContentType()
{
	std::map<std::string, std::string>::iterator value;

	value = headers.find("content-type");
	if (value == headers.end())
		throw NOTEXIST;
	return (value->second);
}

const std::string &Request::getTransferEncoding()
{
	std::map<std::string, std::string>::iterator value;

	value = headers.find("transfer-encoding");
	if (value == headers.end())
		throw NOTEXIST;
	try {
		getContentLength();
		response_code = "400";
		request_code = 400;
	}
	catch (int) {
		if (value->second != "chunked")
		{
			response_code = "501";
			request_code = 501;
			throw -1;
		}
	}
	return (value->second);
}

Request::~Request() {
}
