/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soulang <soulang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 10:20:30 by soulang           #+#    #+#             */
/*   Updated: 2024/04/23 10:20:43 by soulang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "location.hpp"

Location::Location()
{
}

Location::Location(const Location& copy)
{
	(void)copy;
}

Location& Location::operator=(const Location& src)
{
	(void)src;
	return (*this);
}

Location::~Location()
{
}