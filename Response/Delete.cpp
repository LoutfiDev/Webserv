/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soulang <soulang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 10:10:08 by soulang           #+#    #+#             */
/*   Updated: 2024/07/03 10:13:02 by soulang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

void Response::Delete_folder(std::string path)
{
	DIR *directory;
	directory=opendir(path.c_str());
	
	struct dirent *dent;
	while((dent=readdir(directory)) && status_code == "200")
	{
		struct stat st_buf;
		stat (std::string(path + "/" + dent->d_name).c_str(), &st_buf);
		if (std::string(dent->d_name) == ".." || std::string(dent->d_name) == ".")
			continue;
		if (S_ISDIR(st_buf.st_mode))
		{
			Delete_folder(path + "/" + dent->d_name);
			if ((remove(std::string(path + "/" + dent->d_name).c_str())) != 0)
				status_code = "403";
		}
		else if (S_ISREG(st_buf.st_mode)) 
		{
			if ((remove(std::string(path + "/" + dent->d_name).c_str())) != 0)
				status_code = "403";
		}
	}
	closedir(directory);
}

void Response::Delete() 
{ 
	DIR *directory;

	if((directory=opendir(path.c_str())))
	{
		Delete_folder(path);
		closedir(directory);
		if (status_code == "200")
		{
			if ((remove(path.c_str())) != 0)
				status_code = "403";
			else
				status_code = "204";
		}
	}
	else
	{
		if (access(path.c_str(), F_OK) == 0)
		{
			if ((remove(path.c_str())) != 0)
				status_code = "403";
			else 
				status_code = "204";
		}
		else
			status_code = "404";
	}
}