SHELL = /bin/sh

-include Variables.mk

SRCS		:=	$(SRCS:%=$(SRC_DIR)/%)

OBJS		:=	$(SRCS:%.cpp=$(OBJ_DIR)/%.o)

DEPS		:=	$(OBJS:.o=.d)

###############################################################################
#								FLAGS										  #
###############################################################################

CXX			=	c++

CXXFLAGS	=	-Wall -Wextra -Werror -pedantic -std=c++98 -ggdb3

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

bonus:
	$(MAKE) BONUS=1 all
.PHONY:bonus

clean:
	rm -rf $(shell find . -type d -name '.obj*')
.PHONY:clean

fclean: clean
	rm -rf $(shell find . -type f \( -name 'ircserv' -o -name 'Bot' \) )
.PHONY:fclean

re: fclean all
.PHONY:re
