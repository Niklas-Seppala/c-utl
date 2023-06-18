#include "assert.h"
#include "ctl.h"
#include "ctl/binarytree.h"
#include "ctl/compare.h"
#include "ctl/error.h"
#include "ctl/hashfun.h"
#include "ctl/hashmap.h"
#include "ctl/io/logger.h"
#include "ctl/linkedlist.h"
#include "ctl/treeset.h"

int main() {
    INDEX_IN_BOUNDS(11L, 0L, 2L);
    INDEX_IN_LOWER_BOUND(5L, 6L);
    INDEX_IN_UPPER_BOUND(11L, 10L);
    return 0;
}
