# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: soulang <soulang@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/23 11:42:24 by soulang           #+#    #+#              #
#    Updated: 2024/06/05 11:05:01 by soulang          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = Webserv
CPPFLAGS = #-Wall -Wextra -Werror -std=c++98 -fsanitize=address -g3
CXX = c++

SRCS = main.cpp Parser/parser.cpp Parser/server.cpp \
	Parser/location.cpp Parser/Sed/SedIsLoser.cpp \
	Response/Response.cpp AI_part/Post.cpp AI_part/Request.cpp \
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
