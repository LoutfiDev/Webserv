#include "Response.hpp"
#include "../Request/utils.hpp"

int Response::processPostResponse()
{
	char buff[1024];

	memset(buff, '\0', 1023);
	if (method != "POST")
		return (postState = END, 1);
	infile.read(buff, 1023);
	if (infile)
		outfile.write(buff, 1023);
	else
		outfile.write(buff, infile.gcount());
	if (infile.eof()) 
	{
		infile.close();
		outfile.close();
		remove(responseBody.c_str());
		postState = END;
		return 1;
	}
	return 0;
}

/*
 * @Description : Post method, create a file that contains the content of the 
 *				  request body the URI in this method has no effect
 * @param none 
 * 
 * @return void
 *
 */

void Response::Post()
{
	std::string pathname;
	std::string path_dir;

	if (postState != PROCESSING)
		return;
	if (location->upload_dir.length())
	{
		if (access(location->upload_dir.c_str(), F_OK) == -1)
		{
			status_code = "404";
			send_response();
			return;
		}
		if (access(location->upload_dir.c_str(), W_OK) == -1)
		{
			status_code = "403";
			send_response();
			return;
		}
		path_dir = removeLastChar(location->upload_dir);
		pathname = path_dir + '/' + generateFileName() + fileExtension;
	}
	else if (location->root.length())
	{
		if (access(location->root.c_str(), F_OK) == -1)
		{
			status_code = "404";
			send_response();
			return;
		}
		if (access(location->root.c_str(), W_OK) == -1)
		{
			status_code = "403";
			send_response();
			return;
		}
		path_dir = removeLastChar(location->root);
		pathname = path_dir + '/' + generateFileName() + fileExtension;
	}
	else
	{
		status_code = "403";
		send_response();
		return;
	}
	infile.open(responseBody.c_str(), std::ifstream::binary);
	outfile.open(pathname.c_str(), std::ofstream::binary);
	postState = SENDING;
	status_code = "201";
	processPostResponse();

	// i think in case of a regular file that can pass throught CGI we need to call GET method (arabic : dakchi li galina youssef l2ostora)
}
