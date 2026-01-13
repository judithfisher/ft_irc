# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jfischer <jfischer@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/09 15:25:05 by jfischer          #+#    #+#              #
#    Updated: 2026/01/10 11:48:22 by jfischer         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_irc

C++ = c++
FLAGS = -g -Wall -Wextra -Werror -std=c++98 

SRCS = 	Client.cpp \
		checkers.cpp \
		main.cpp \
		Server.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(C++) $(FLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp
	$(C++) $(FLAGS) -c $< -o $@
clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re