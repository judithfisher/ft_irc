# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jfischer <jfischer@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/09 15:25:05 by jfischer          #+#    #+#              #
#    Updated: 2026/01/31 17:16:09 by jfischer         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_irc

CXX = c++
FLAGS = -Wall -Wextra -Werror -std=c++98

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
