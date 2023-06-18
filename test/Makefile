OBJ_DIR:=../obj/
OUT_DIR:=../out/
SRC_DIR:=../src/
TEST_DIR:=./

#export CK_FORK=foo

CC=gcc
HEADERS=-I../include -Iinclude

MEMCHECK=
S_LIBSAN=
DEBUG=-g -DDEBUG $(MEMCHECK)
TEST=-lm -lcheck -lsubunit $(S_LIBSAN) $(MEMCHECK)

CC_WARN=-Wall -Wshadow -Wextra -Wformat=2 -Wpedantic -fmax-errors=10 -Wno-unknown-pragmas
CC_FLAGS=${CC_WARN} -O0 -std=gnu99 ${DEBUG} -DRT_NULL_CHECKS -DRT_INDEX_BOUNDS_CHECKS
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
	@${CC} -c ${CC_FLAGS} ${HEADERS} testhashmap.c -o ${OBJ_DIR}testhashmap.o
	@${CC} -c ${CC_FLAGS} ${HEADERS} testvector.c -o ${OBJ_DIR}testvector.o

build: suite ${BINS}
	@${CC} ${OBJS} ${OBJ_DIR}test.o ${OBJ_DIR}testlinkedlist.o ${OBJ_DIR}testhashmap.o ${OBJ_DIR}testvector.o ${TEST} -o ${OUT_DIR}${TEST_NAME}

%: %.o
	
%.o: ${SRC_DIR}%.c
	@${CC} -c ${CC_FLAGS} ${HEADERS} $< -o ${OBJ_DIR}$@

run: build
	@${OUT_DIR}${TEST_NAME}

clean:
	@rm $(OBJ_DIR)* $(OUT_DIR)* *.o 2>/dev/null || true