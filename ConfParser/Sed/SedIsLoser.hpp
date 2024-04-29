/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SedIsLoser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soulang <soulang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 12:01:18 by yloutfi           #+#    #+#             */
/*   Updated: 2024/04/27 10:14:29 by soulang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SEDISLOSER_H__
#define __SEDISLOSER_H__

#include <iostream>
#include <cstring>
#include <fstream>

class Sed
{
	private:
		Sed();
	public:
		static void execute (std::string _fileName, std::string _s1, std::string s2, std::string _s3);
		~Sed();
};


#endif