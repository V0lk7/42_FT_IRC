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

# INCLUDES	=	$(SRC_DIR)/Class/Client/	\
# 				$(SRC_DIR)/Class/Channel/	\
# 				$(SRC_DIR)/Class/Server/	\

INCLUDES	= 	$(SRC_DIR)/Tools/			\
				$(SRC_DIR)/Parsing/			\
				$(SRC_DIR)/Class/Client/	\

SRCS		:=	MainCore/main.cpp \

SRCS		+=	Parsing/Parsing.cpp

SRCS		+=	Tools/cSplit.cpp

SRCS		+=	Class/Client/Client.cpp

SRCS		:=	$(SRCS:%=$(SRC_DIR)/%)

OBJS		:=	$(SRCS:%.cpp=$(OBJ_DIR)/%.o)

DEPS		:=	$(OBJS:.o=.d)

###############################################################################
#								FLAGS										  #
###############################################################################

CC			=	c++

CFLAGS		=	-Wall -Wextra -Werror -pedantic -std=c++98 -g

CPPFLAGS	=	-MMD -MP $(addprefix -I,$(INCLUDES))

###############################################################################
#								UTILS										  #
###############################################################################

DIR_CREATE	=	mkdir -p $(@D)

###############################################################################
#								COMPILATION									  #
###############################################################################

$(NAME): $(OBJS)
	@$(CC) $(OBJS) -o $(NAME)
	$(info CREATED $(NAME))

$(OBJ_DIR)/%.o : %.cpp
	$(DIR_CREATE)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

-include $(DEPS)

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
