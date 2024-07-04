/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soulang <soulang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 10:10:08 by soulang           #+#    #+#             */
/*   Updated: 2024/07/03 10:11:56 by soulang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

void Response::Get() {
	DIR *directory;
	if (location->redirection.size())
	{
		std::map<std::string, std::string>::iterator it = location->redirection.begin();
		status_code = it->first;
	}
	else if ((directory = opendir(path.c_str())))
	{
		if (uri[uri.size() - 1] != '/')
		{
			uri.append("/");
			status_code = "301";
		}
		else
		{
			if (location->index.size())
			{
				std::string tmp;
				std::vector<std::string>::iterator it = location->index.begin();
				for (; it != location->index.end(); ++it)
				{
					tmp = path + *it;
					if (access(tmp.c_str(), F_OK) == 0)
					{
						path.append(*it);
						if (access(path.c_str(), R_OK) != 0)
							status_code = "403";
						closedir(directory);
						return ;
					}
				}
				status_code = "403";
			}
			else if (location->autoindex)
			{
				std::string tmp(path + "index.html");
				if (access(tmp.c_str(), F_OK) == 0)
				{
					if (access(tmp.c_str(), R_OK) != 0)
						status_code = "403";
					else
						path = tmp;
				}
			}
			else
				status_code = "403";
		}
		closedir(directory);
	}
	else
	{
		if (access(path.c_str(), F_OK) == 0)
		{
			if (access(path.c_str(), R_OK) != 0)
				status_code = "403";
		}
		else
			status_code = "404";
	}
}
