# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: soulang <soulang@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/23 11:42:24 by soulang           #+#    #+#              #
#    Updated: 2024/04/27 10:18:08 by soulang          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = Webserv
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 #-fsanitize=address -g3
CXX = c++

SRCS = main.cpp ConfParser/parser.cpp ConfParser/server.cpp \
	ConfParser/location.cpp ConfParser/Sed/SedIsLoser.cpp

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