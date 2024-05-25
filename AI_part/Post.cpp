
#include "utils.hpp"
#include "Request.hpp"
#include "Client.hpp"
#include <iostream>
#include <fstream>

/*
 * @Description : Post method
 * @param none 
 * 
 * @return string the response so that i can read it and send it to the Client
 *
 */

std::string Post(Client client)
{
	Request req;
	std::string post_response;
	std::fstream file(fileName().c_str());

	req = client.getRequest();

	if (req.getBodyLength() > 0)
	{
		file << req.getBody();
	}
	return "201";
}
