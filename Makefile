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

INCLUDES	=	$(SRC_DIR)/Class/Client/	\
				$(SRC_DIR)/Class/Channel/	\
				$(SRC_DIR)/Class/Server/	\
				$(SRC_DIR)/Tools/	\

SRCS		:=	Class/Client/Client.cpp		\
				Class/Channel/Channel.cpp	\

SRCS		:=	MainCore/main.cpp	\
				Class/Client/Client.cpp	\
				Class/Channel/Channel.cpp	\
				Class/Server/Server.cpp	\
				Tools/cSplit.cpp
				
SRCS		:=	$(SRCS:%=$(SRC_DIR)/%)

OBJS		:=	$(SRCS:%.cpp=$(OBJ_DIR)/%.o)

DEPS		:=	$(OBJS:.o=.d)

###############################################################################
#								FLAGS										  #
###############################################################################

CXX			=	c++

CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98 

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
