/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soulang <soulang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 08:53:41 by soulang           #+#    #+#             */
/*   Updated: 2024/05/15 11:41:14 by soulang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfParser/parser.hpp"


int main (int ac, char **av)
{
	if (ac == 2)
	{
		std::string fileName = av[1];
		Parser parser(fileName);
	}
	else 
		std::cout << av[0] << \
			" accept 1 param (Configuration file) only!" << std::endl;
	return (0);
}