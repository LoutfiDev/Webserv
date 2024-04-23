/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soulang <soulang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 10:17:20 by soulang           #+#    #+#             */
/*   Updated: 2024/04/23 11:47:22 by soulang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <iostream>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <sstream>
#include <cstdlib>

#include "parser.hpp"

class Location
{
	private:
		
	public:
		Location();
		Location(const Location& copy);
		Location& operator=(const Location& src);
		~Location();
};

#endif