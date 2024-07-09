# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: soulang <soulang@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/23 11:42:24 by soulang           #+#    #+#              #
#    Updated: 2024/07/09 16:36:00 by soulang          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = Webserv
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -g -fsanitize=address -g3 #-fstandalone-debug
CXX = c++

SRCS = main.cpp Parser/parser.cpp Parser/server.cpp \
	Parser/location.cpp Parser/Sed/SedIsLoser.cpp \
	Response/Response.cpp  Response/Get.cpp Response/Delete.cpp \
	Response/Cgi.cpp AI_part/Request.cpp \
	AI_part/Worker.cpp AI_part/Client.cpp AI_part/ServerManager.cpp \
	AI_part/utils.cpp

OBJS = $(SRCS:.cpp=.o)

all : $(NAME)

$(NAME) : $(OBJS) 
	$(CXX) $(CPPFLAGS) $(OBJS) -o $(NAME)

clean :
	-rm -f $(OBJS)

fclean : clean
	-rm -f $(NAME)

re : fclean all

.PHONY: all clean fclean re
