COMPLETE_PRINT=\033[1;32mSuccess\033[0m

BUILD=0.0.1
BINARY=ctl-${BUILD}-test
CODEDIRS=${shell ls -d ../src ../src/*/ src}
INCDIRS=./include/ ../include/

CC=gcc
MEMCHECK=
S_LIBSAN=
TEST=-lm -lcheck -lsubunit $(S_LIBSAN) $(MEMCHECK)
DEBUG=-g -DDEBUG $(MEMCHECK)
MKDIR_P = mkdir -p
DEPFLAGS=-MP -MD
RT_NULL_CHECKS=-DRT_NULL_CHECKS -DRT_NULL_KILLS -DCTL_MERGESORT_DYNAMIC_ALLOC -DRT_INDEX_BOUNDS_CHECKS
CC_WARN=-Wall -Wshadow -Wextra -Wformat=2 -Wpedantic -fmax-errors=10 -Wno-unknown-pragmas
CFLAGS=${CC_WARN} -O0 -std=gnu99 ${DEBUG} ${RT_NULL_CHECKS} $(foreach D,$(INCDIRS),-I$(D)) ${DEPFLAGS}

CFILES=$(foreach D,$(CODEDIRS),$(wildcard $(D)/*.c))
OBJECTS=$(patsubst %.c,%.o,$(CFILES))
DEPFILES=$(patsubst %.c,%.d,$(CFILES))

export CK_TIMEOUT_MULTIPLIER=10

all: $(BINARY)
$(BINARY): $(OBJECTS)
	$(CC) $^ -o $@ ${TEST}
	@echo "${COMPLETE_PRINT}"
	@echo "\n"

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@rm -rf $(BINARY) $(OBJECTS) $(DEPFILES) 2>/dev/null || true

distribute: clean
	tar zcvf dist.tgz *

diff:
	@git status
	@git diff --stat

# include the dependencies
-include $(DEPFILES)

# add .PHONY so that the non-targetfile - rules work even if a file with the same name exists.
.PHONY: all clean distribute diff
	
run: $(BINARY)
	./${BINARY} ${RUN_ARGS}

valgrind: ${BINARY}
	valgrind ./${BINARY}