CC = gcc
CFLAGS = -c -Wall -Werror -Wextra
CFLAGS1 = -Wall -Werror -Wextra
LDFLAGS = $(shell pkg-config --cflags --libs check)
TEST_FLAGS = --coverage $(LDFLAGS)

FUNC_DIR = lib_functions
OBJ_DIR = lib_functions
GCOV_DIR = report
EXECUTABLE = s21_test
PWD = $(shell pwd)

SRCS = $(shell find $(PWD)/$(FUNC_DIR) -maxdepth 1 -iname '*.c')
OBJSNAME = $(shell basename -a $(SRCS))
OBJS = $(addprefix $(OBJ_DIR)/,$(OBJSNAME:.c=.o))

TESTS_SOURCE = tests/s21_tests.c
TESTS_OBJECTS = $(patsubst %.c,%.o, ${TESTS_SOURCE})
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
  OPEN_CMD = xdg-open
	ADD_LIB = -lcheck -lsubunit -lm -lrt -lpthread -D_GNU_SOURCE
	BREW = .
endif
ifeq ($(UNAME_S),Darwin)
	OPEN_CMD = open
	ADD_LIB =
	BREW := ${HOME}/homebrew
endif

PATH := $(BREW)/bin:$(PATH)

all: clean s21_string.a

./$(OBJ_DIR)/%.o:%.c
	$(CC) $(CFLAGS) $< -o $@

./$(OBJ_DIR)/%.o:*/%.c
	$(CC) $(CFLAGS) $< -o $@

s21_string.a: $(OBJS)
	ar rcs $@ $^
	ranlib s21_string.a 

test:
	make s21_string.a
	$(CC) $(CFLAGS1) $(TESTS_SOURCE) s21_string.a -o $(EXECUTABLE) $(LDFLAGS)
	./$(EXECUTABLE)

gcov_report:
	make clean
	mkdir -p $(GCOV_DIR)
	$(CC) $(SRCS) $(TESTS_SOURCE) -o greport $(TEST_FLAGS)
	./greport
	lcov -t "s21_string" -o s21_string.info -c -d .
	genhtml -o ./$(GCOV_DIR) s21_string.info
	$(OPEN_CMD) ./report/index.html

lcov:
ifeq ("", "$(shell PATH=$(PATH) which lcov)")
	$(error Need to install lcov)
endif

valgrind: clean
	make s21_string.a
	gcc -g ${TESTS_SOURCE} s21_string.a -o test -lcheck -lcheck -lpthread -lm -D_GNU_SOURCE -lrt -lsubunit
	CK_FORK=no valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all --verbose --log-file=RESULT_VALGRIND.txt ./test

style: clean
	cp ../materials/linters/.clang-format ../src/.clang-format
	clang-format -n *.h lib_functions/*.c tests/*.c
	rm .clang-format

clean:
	-rm -rf $(OBJ_DIR)/*.o
	-rm -rf $(EXECUTABLE)
	-rm -rf *.o && rm -rf *.gcno
	-rm -rf *.a && rm -rf *.gcda
	-rm -rf *.info && rm -rf *.gcov
	-rm -rf ./test && rm -rf ./gcov_report
	-rm -rf ./report/
	-rm -rf s21_test greport
	-rm -rf RESULT_VALGRIND.txt

.PHONY: all clean check lcov