DIRUNITEST	:=	Unitary-Test

TEST		:=	$(shell find $(DIRUNITEST) -name '*.cpp')
TEST_OBJS	:=	$(TEST:%.cpp=$(OBJ_DIR)/%.o)

ARCHIVE		:=	$(DIRUNITEST)/irc.a
RUNNER		:=	$(DIRUNITEST)/runtest

$(ARCHIVE): $(OBJS)
	ar rcs $(ARCHIVE) $(OBJS)

$(TEST_OBJS): CPPFLAGS += -I $(DIRUNITEST)

$(TEST_OBJS): CXXFLAGS = -ggdb3

$(RUNNER): $(ARCHIVE) $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TEST_OBJS) $(ARCHIVE) -o $(RUNNER)

clear:
	rm -rf $(ARCHIVE)
.PHONY: clear

rclean: fclean clear
	rm -rf $(RUNNER)
.PHONY: rclean

test: clear fclean rclean $(RUNNER)
.PHONY: test

run:
	cd $(DIRUNITEST)/ && ./runtest
.PHONY: run

valrun:
	cd $(DIRUNITEST)/ && valgrind --leak-check=full ./runtest
.PHONY: valrun

gdbrun:
	cd $(DIRUNITEST)/ && gdb ./runtest
.PHONY: gdbrun

help:
	@echo "Usage : make | make [OPTION]"
	@echo "OPTION: Normal Usage = [all], [clean], [fclean], [re]"
	@echo "OPTION: Test Usage:"
	@echo "[clear]: Erase Unitary-Test/irc.a"
	@echo "[rclean]: Erase Unitary-Test/runtest"
	@echo "[test]: Compile test files and create executable"
	@echo "[run]: run all the test"
	@echo "[valrun]: run all the testi with valgrind"
	@echo "[gdbrun]: run all the testi with gdb"
