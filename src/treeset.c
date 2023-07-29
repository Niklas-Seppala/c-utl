#include "ctl/treeset.h"

#include "string.h"

struct __CTLTreeSet {
    CTLBinaryNode *root;
    CTLCompareFunction entryComparer;
    size_t size;
};

CTLTreeSet CTLTreeSetCreate(CTLCompareFunction entryComparer) {
    CTLTreeSet tree = calloc(sizeof(struct __CTLTreeSet), 1);
    tree->entryComparer = entryComparer;
    return tree;
}

inline const CTLBinaryNode *CTLTreeSetAsBinaryTree(CTLTreeSet set) {
    return set->root;
}

void CTLTreeSetAdd(CTLTreeSet tree, const void *entry) {
    if (tree->root == NULL) {
        tree->root = CTLCreateBinaryNode(entry);
        tree->size++;
        return;
    }
    CTLBinaryTreeAdd(tree->root, CTLCreateBinaryNode(entry),
                     tree->entryComparer);
    tree->size++;
    CTLPrintBinaryNode(tree->root, "", "");
}

inline size_t CTLTreeSetSize(CTLTreeSet tree) { return tree->size; }

const void *CTLTreeSetFind(const CTLTreeSet tree, const void *entry) {
    NOT_NULL(tree);
    CTLBinaryNode *wrapper = CTLCreateBinaryNode(entry);
    const void *result =
        CTLBinaryTreeFind(tree->root, wrapper, tree->entryComparer);
    free(wrapper);
    return result;
}

bool CTLTreeSetContains(const CTLTreeSet tree, const void *entry) {
    NOT_NULL(tree);
    return CTLTreeSetFind(tree, entry) != NULL;
}

bool CTLTreeSetEquals(const CTLTreeSet a, const CTLTreeSet b) {
    if (a == NULL && b == NULL) return true;
    if (a == NULL || b == NULL) return false;
    if (a->entryComparer != b->entryComparer) {
        return false;
    }
    if (CTLTreeSetSize(a) != CTLTreeSetSize(b)) {
        return false;
    }
    return CTLBinaryTreeEquals(a->root, b->root, a->entryComparer);
}

const void *CTLTreeSetRemove(const CTLTreeSet tree, const void *entry) {
    NOT_NULL(tree);
    CTLBinaryNode *wrapper = CTLCreateBinaryNode(entry);
    CTLBinaryNode *result =
        CTLBinaryTreeRemove(tree->root, wrapper, tree->entryComparer);
    free(wrapper);
    CTLPrintBinaryNode(tree->root, "", "");
    if (result != NULL) {
        const void *val = result->entry;
        free(result);
        return val;
    } else {
        return NULL;
    }
}
