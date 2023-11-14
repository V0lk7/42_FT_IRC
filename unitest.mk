DIRUNITEST	:=	Unitary-Test

TEST		:=	$(shell find $(DIRUNITEST) -name '*.cpp')
TEST_OBJS	:=	$(TEST:%.cpp=$(OBJ_DIR)/%.o)

ARCHIVE		:=	$(DIRUNITEST)/irc.a
RUNNER		:=	$(DIRUNITEST)/runtest

$(ARCHIVE): $(OBJS)
	ar rcs $(ARCHIVE) $(OBJS)

$(TEST_OBJS): CPPFLAGS += -I $(DIRUNITEST)

$(TEST_OBJS): CXXFLAGS = -Wall -Wextra -ggdb3

$(RUNNER): $(ARCHIVE) $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TEST_OBJS) $(ARCHIVE) -o $(RUNNER)

clear:
	rm -rf $(ARCHIVE)
.PHONY: clear

rclean:
	rm -rf $(RUNNER)
.PHONY: rclean

test: clear fclean rclean $(RUNNER)
.PHONY: test

run:
	cd unitest/ && ./runtest
.PHONY: run

valrun:
	cd $(DIRUNITEST)/ && valgrind --leaks-check=full --show-kind=all ./runtest
.PHONY: valrun
