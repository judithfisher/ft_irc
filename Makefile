# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jfischer <jfischer@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/09 15:25:05 by jfischer          #+#    #+#              #
#    Updated: 2026/01/31 23:34:46 by jfischer         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_irc

CXX = c++
FLAGS = -Wall -Wextra -Werror -std=c++98 -g 
# -g for valgrind lines showing


SRCS = 	Client.cpp \
		checkers.cpp \
		helpers.cpp \
		main.cpp \
		Server.cpp \
		ServerSocket.cpp \
		ServerCommands.cpp \
		ServerChannels.cpp \
		Channel.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(FLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp
	$(CXX) $(FLAGS) -c $< -o $@
clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
