NAME		:= Bot

SRCS		:=	main.cpp \
				Bot.cpp

CXXFLAGS	:= -Wall -Wextra -ggdb3 -Werror -std=c++98 -pedantic

$(NAME):	$(SRCS) $(wildcard *.hpp) Makefile
			c++ $(CXXFLAGS) $(SRCS) -o $(NAME)