COMPLETE_PRINT=\033[1;32mSuccess \033[0m

OBJ_DIR:=./obj/
OUT_DIR:=./out/
SRC_DIR:=src/

CC=gcc

##################### DEBUG ####################
DEBUG=-g -DDEBUG
################################################

MKDIR_P = mkdir -p

RT_NULL_CHECKS=-DRT_NULL_CHECKS -DRT_NULL_KILLS -DCTL_MERGESORT_DYNAMIC_ALLOC
CC_WARN=-Wall -Wshadow -Wextra -Wformat=2 -Wpedantic -fmax-errors=10 -Wno-unknown-pragmas
CC_FLAGS=${CC_WARN} -O0 -std=gnu99 ${DEBUG} ${RT_NULL_CHECKS}
PROJECT_NAME=ctl
BUILD=0.0.1
EXE_NAME=${PROJECT_NAME}-${BUILD}

SRC_FILES=$(shell find src -type f -iname '*.c')
SRC := $(SRC_FILES:${SRC_DIR}%=%)
OBJS := $(SRC:%.c=%.o)
OUT_DIR := out/
OBJ_DIR := obj/
BINS := $(SRC:%.c=%)

.PHONY: build

build: directories ${BINS}
	${CC} ${OBJS} -o ${OUT_DIR}${EXE_NAME}
	@mv ./*.o ${OBJ_DIR}
	@echo "$(COMPLETE_PRINT)"
	@echo $(shell git rev-parse --short HEAD)
	@echo ${OUT_DIR}${EXE_NAME}

%.o: ${SRC_DIR}%.c
	${CC} -Iinclude ${CC_FLAGS} -c $< -o $@

%: %.o
	

run: build
	@${OUT_DIR}${EXE_NAME} ${RUN_ARGS}

directories: ${OUT_DIR} ${OBJ_DIR}

${OBJ_DIR}:
	${MKDIR_P} ${OBJ_DIR}

${OUT_DIR}:
	${MKDIR_P} ${OUT_DIR}

clean:
	@rm $(OBJ_DIR)* $(OUT_DIR)* *.o 2>/dev/null || true

valgrind: build
	valgrind --leak-check=full ${OUT_DIR}${EXE_NAME}

unit-test:
	@make runtest -C ./test

memcheck-test:
	@make memcheck -C ./test