OBJ_DIR:=../obj/
OUT_DIR:=../out/
SRC_DIR:=../src/
TEST_DIR:=./

CC=gcc
HEADERS=-I../include -Iinclude
TEST=-lcheck -lm -lpthread -lrt -lsubunit

DEBUG=-g -DDEBUG

CC_WARN=-Wall -Wshadow -Wextra -Wformat=2 -Wpedantic -fmax-errors=10 -Wno-unknown-pragmas
CC_FLAGS=${CC_WARN} -O0 -std=gnu99 ${DEBUG}
PROJECT_NAME=ctl
BUILD=0.0.1
EXE_NAME=${PROJECT_NAME}-${BUILD}
TEST_NAME=${EXE_NAME}-tests

SRC_FILES=$(shell find ../src -type f \( -iname "*.c" ! -iname "main.c" \))

SRC := $(SRC_FILES:${SRC_DIR}%=%)
OBJS := $(SRC:%.c=${OBJ_DIR}%.o)
BINS := $(SRC:%.c=%)
COMPLETE_PRINT=\033[1;32mSuccess \033[0m

suite:
	@${CC} -c ${CC_FLAGS} ${HEADERS} testlinkedlist.c -o ${OBJ_DIR}testlinkedlist.o
	@${CC} -c ${CC_FLAGS} ${HEADERS} test.c -o ${OBJ_DIR}test.o

build: suite ${BINS}
	@${CC} ${OBJS} ${OBJ_DIR}test.o ${OBJ_DIR}testlinkedlist.o ${TEST} -o ${OUT_DIR}${TEST_NAME}

%: %.o
	
%.o: ${SRC_DIR}%.c
	@${CC} -c ${CC_FLAGS} ${HEADERS} $< -o ${OBJ_DIR}$@

runtest: build
	@${OUT_DIR}${TEST_NAME}

memcheck: build
	@valgrind --leak-check=full --show-leak-kinds=all --log-file=../out/memcheck.log ${OUT_DIR}${TEST_NAME}
	@echo "\n"
	@grep --color=always "ERROR SUMMARY:" ../out/memcheck.log
	@echo ---------------------------------------------------------------
	@tail ../out/memcheck.log
	@echo "\n\n"

clean:
	@rm $(OBJ_DIR)* $(OUT_DIR)* *.o 2>/dev/null || true