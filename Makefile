###############################################################################
#								EXECUTABLE									  #
###############################################################################

NAME		=	ircserv

###############################################################################
#								DESTINATION									  #
###############################################################################

OBJ_DIR		:=	.obj

###############################################################################
#								SOURCES										  #
###############################################################################

SRC_DIR		=	Mandatory

INCLUDES	= 	$(SRC_DIR)/Tools/			\
				$(SRC_DIR)/Parsing/			\
				$(SRC_DIR)/Class/Client/	\
				$(SRC_DIR)/Class/Server/	\
				$(SRC_DIR)/Class/Channel/	\

SRCS		:=	MainCore/main.cpp \

SRCS		+=	Parsing/Parsing.cpp \

SRCS		+=	Tools/cSplit.cpp \

SRCS		+=	Class/Client/Client.cpp		\
				Class/Channel/Channel.cpp	\
				Class/Server/Server.cpp

SRCS		+=	Commands/JOIN/Join.cpp				\
				Commands/JOIN/Parsing.cpp			\
				Commands/JOIN/Join_utilities.cpp	\

SRCS		:=	$(SRCS:%=$(SRC_DIR)/%)

OBJS		:=	$(SRCS:%.cpp=$(OBJ_DIR)/%.o)

DEPS		:=	$(OBJS:.o=.d)

###############################################################################
#								FLAGS										  #
###############################################################################

CXX			=	c++

CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98 -ggdb3

CPPFLAGS	=	-MMD -MP $(addprefix -I,$(INCLUDES))

###############################################################################
#								UTILS										  #
###############################################################################

DIR_CREATE	=	mkdir -p $(@D)

###############################################################################
#								COMPILATION									  #
###############################################################################

$(NAME): $(OBJS)
	$(CXX) $(OBJS) -o $(NAME)
	$(info CREATED $(NAME))

$(OBJ_DIR)/%.o : %.cpp
	$(DIR_CREATE)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

-include $(DEPS) unitest.mk

###############################################################################
#								RULES										  #
###############################################################################

all: $(NAME)
.PHONY:all

clean:
	rm -rf $(OBJ_DIR)
.PHONY:clean

fclean: clean
	rm -rf $(NAME)
.PHONY:fclean

re: fclean all
.PHONY:re
