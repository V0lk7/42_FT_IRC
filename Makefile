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

SRC_DIR		:=	Mandatory

INCLUDES	:= 	$(SRC_DIR)/Tools/			\
				$(SRC_DIR)/Commands/		\
				$(SRC_DIR)/Parsing/			\
				$(SRC_DIR)/Class/Client/	\
				$(SRC_DIR)/Class/Server/	\
				$(SRC_DIR)/Class/Channel/	\
				$(SRC_DIR)/Commands/KICK/	\

SRCS		:=	MainCore/main.cpp 					\

SRCS		+=	Parsing/Parsing.cpp 				\

SRCS		+=	Tools/cSplit.cpp 					\

SRCS		+=	Class/Client/Client.cpp				\
				Class/Channel/Channel.cpp			\
				Class/Server/Server.cpp

SRCS		+=	Commands/TOPIC/Topic.cpp			\

SRCS		+=	Commands/JOIN/Join.cpp				\
				Commands/JOIN/Join_parsing.cpp		\
				Commands/JOIN/Join_utilities.cpp	\

SRCS		+=	Commands/KICK/Kick.cpp				\
				Commands/KICK/ParsingKick.cpp		\

SRCS		+=	Commands/MODE/Mode.cpp				\
				Commands/MODE/Mode_parsing.cpp		\
				Commands/MODE/Mode_utilities.cpp	\

SRCS		+=	Commands/INVITE/Invite.cpp			\

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
