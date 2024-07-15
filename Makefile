# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: anaji <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/23 11:42:24 by soulang           #+#    #+#              #
#    Updated: 2024/07/15 11:45:31 by anaji            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = Webserv
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -g #-fsanitize=address -g3 #-fstandalone-debug
CXX = c++

SRCS = main.cpp Parser/parser.cpp Parser/server.cpp \
	Parser/location.cpp Parser/Sed/SedIsLoser.cpp \
	Response/Response.cpp  Response/Get.cpp Response/Delete.cpp \
	Response/Cgi.cpp Response/Post.cpp Request/Request.cpp \
	Request/Worker.cpp Request/Client.cpp Request/ServerManager.cpp \
	Request/utils.cpp

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
