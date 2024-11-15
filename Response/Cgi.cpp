/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaji <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 10:10:08 by soulang           #+#    #+#             */
/*   Updated: 2024/07/20 14:32:01 by anaji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include <cstdlib>
#include <iostream>

void Response::formEnv( void )
{
	env = new char*[9];
	memset(env, 0, (9 * sizeof(char *)));
	
	env[0] = new char[std::string(std::string("REQUEST_METHOD") + "=" + method).size() + 1];
	strcpy(env[0], std::string(std::string("REQUEST_METHOD") + "=" + method).c_str());
	
	env[1] = new char[std::string(std::string("QUERY_STRING") + "=" + query).size() + 1];
	strcpy(env[1], std::string(std::string("QUERY_STRING") + "=" + query).c_str());
	
	env[2] = new char[std::string(std::string("REDIRECT_STATUS") + "=" + "200").size() + 2];
	strcpy(env[2], std::string(std::string("REDIRECT_STATUS") + "=" + "200").c_str());
	
	env[3] = new char[std::string(std::string("SCRIPT_FILENAME") + "=" + cgiFile).size() + 1];
	strcpy(env[3], std::string(std::string("SCRIPT_FILENAME") + "=" + cgiFile).c_str());
	
	env[4] = new char[std::string(std::string("HTTP_COOKIE") + "=" + http_cookie).size() + 1];
	strcpy(env[4], std::string(std::string("HTTP_COOKIE") + "=" + http_cookie).c_str());
	
	if (method == "POST")
	{
		env[5] = new char[std::string(std::string("CONTENT_TYPE") + "=" + contentType).size() + 1];
		strcpy(env[5], std::string(std::string("CONTENT_TYPE") + "=" + contentType).c_str());
		
		env[6] = new char[std::string(std::string("CONTENT_LENGTH") + "=" + getContentLenght(responseBody)).size() + 1];
		strcpy(env[6], std::string(std::string("CONTENT_LENGTH") + "=" + getContentLenght(responseBody)).c_str());
	}
	
}

int Response::isValid( void )
{
	std::stringstream ss(path);
	while (getline(ss, extension, '.'))
		continue;
	extension.insert (0, 1, '.');
	std::map<std::string, std::string>::iterator it = location->cgi.begin();
	for (; it != location->cgi.end(); ++it)
	{
		if (extension == it->first)
		{
			cgiPath = it->second;
			return (0);
		}
	}
	return 1;
}

int Response::is_cgi()
{
	DIR *directory;
	
	if ((directory = opendir(path.c_str())))
	{
		closedir(directory);
		if (location->index.size())
		{
			std::string tmp;
			std::vector<std::string>::iterator it = location->index.begin();
			for (; it != location->index.end(); ++it)
			{
				tmp = path + *it;
				if (access(tmp.c_str(), F_OK) == 0)
				{
					if (access(tmp.c_str(), W_OK) != 0)
						return 1;
					else
					{
						path.append(*it);
						if (isValid())
							return 1;
						return 0;
					}
				}
			}
			return 1;
		}
		else
			return 1;
	}
	else
	{
		if (access(path.c_str(), F_OK) == 0)
		{
			if (access(path.c_str(), W_OK) != 0 || isValid())
				return 1;			
		}
		else
			return 1;
	}
	return 0;
}
void Response::resetTimer()
{
	timespan = time(0);
}

bool Response::istimeOut()
{
	if (time(0) - timespan > CGITIMEOUT)
		return true;
	return false;
}

std::string getScriptName(std::string str)
{
	int len = (int)str.size() - 1;
	while (len >= 0)
	{
		if (str[len] == '/')
			break;
		len--;
	}
	return (str.substr(len + 1));
}

int Response::execute_cgi( void )
{
	if (location && location->cgi.size() && !is_cgi())
	{
		cgiFile = getScriptName(path);
		if (STAGE == EXEC_CGI)
		{
			resetTimer();
			cgiOut = generateFileName();
			cgiOut.append(".html");
			cgiErr = generateFileName();
			if ((pid = fork()) == 0)
			{
				argv = new char*[3];
				argv[0] = new char[cgiPath.size() + 1];
				strcpy(argv[0], cgiPath.c_str());
				argv[1] = new char[cgiFile.size() + 1];
				strcpy(argv[1], cgiFile.c_str());
				argv[2] = NULL;
				formEnv();
				out = freopen (cgiOut.c_str(),"w",stdout);
				err = freopen (cgiErr.c_str(),"w",stderr);
				if (method == "POST")
					in = freopen (responseBody.c_str(),"r",stdin);
				if (chdir((location->root).c_str()) == -1)
					kill(getpid(), SIGKILL);
				if (execve(argv[0], argv, env) == -1)
					kill(getpid(), SIGKILL);
			}
			STAGE += 1;
		}
		waitpid(pid, &status, WNOHANG);
		if (status != -1)
		{
			if (WIFSIGNALED(status))
			{
				status_code = "500";
				STAGE += 1;
				return 2;
			}
			STAGE += 1;
		}
		else if (istimeOut())
		{
			kill(pid, SIGKILL);
			waitpid(pid, NULL, 0);
			status_code = "504";
			STAGE += 1;
			return 2;
		}
	}
	else
		STAGE = CGI_PROCESSING + 1;
	return 1;
}
