# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: soulang <soulang@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/23 11:42:24 by soulang           #+#    #+#              #
#    Updated: 2024/05/20 10:18:09 by soulang          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = Webserv
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 #-fsanitize=address -g3
CXX = c++

SRCS = main.cpp Parser/parser.cpp Parser/server.cpp \
	Parser/location.cpp Parser/Sed/SedIsLoser.cpp \
	Response/Response.cpp

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