#include "ctl/binarytree.h"

#include "string.h"

CTLBinaryNode *CTLBinaryTreeRemove(CTLBinaryNode *root,
                                   const CTLBinaryNode *entryNode,
                                   CTLCompareFunction entryComparer) {
    if (root == NULL) {
        return NULL;
    }
    int cmp = entryComparer(entryNode->entry, root->entry);
    if (cmp == LT) {
        root->left = CTLBinaryTreeRemove(root->left, entryNode, entryComparer);
    } else if (cmp == GT) {
        root->right =
            CTLBinaryTreeRemove(root->right, entryNode, entryComparer);
    } else {
        if (root->left == NULL && root->right == NULL) {
            free(root);
            root = NULL;
        } else if (root->left == NULL) {
            CTLBinaryNode *temp = root;
            root = root->right;
            free(temp);
        } else if (root->right == NULL) {
            CTLBinaryNode *temp = root;
            root = root->left;
            free(temp);
        } else {
            const CTLBinaryNode *temp = CTLBinaryTreeFindMin(root->right);
            root->entry = temp->entry;
            root->right = CTLBinaryTreeRemove(root->right, temp, entryComparer);
        }
    }
    return root;
}

void CTLBinaryTreeAdd(CTLBinaryNode *rootNode, CTLBinaryNode *entryNode,
                      CTLCompareFunction entryComparer) {
    int cmp = entryComparer(entryNode == NULL ? NULL : entryNode->entry,
                            rootNode == NULL ? NULL : rootNode->entry);
    if (cmp == LT) {
        if (rootNode->left == NULL) {
            rootNode->left = entryNode;
        } else {
            CTLBinaryTreeAdd(rootNode->left, entryNode, entryComparer);
        }
    } else {
        if (rootNode->right == NULL) {
            rootNode->right = entryNode;
        } else {
            CTLBinaryTreeAdd(rootNode->right, entryNode, entryComparer);
        }
    }
}

const CTLBinaryNode *CTLBinaryTreeFindMax(CTLBinaryNode *node) {
    if (node == NULL) return NULL;
    while (node->right != NULL) node = node->right;
    return node;
}

const CTLBinaryNode *CTLBinaryTreeFindMin(CTLBinaryNode *node) {
    if (node == NULL) return NULL;
    while (node->left != NULL) node = node->left;
    return node;
}

const void *CTLBinaryTreeFind(CTLBinaryNode *node, CTLBinaryNode *entryNode,
                              CTLCompareFunction entryComparer) {
    if (node == NULL) {
        return NULL;
    }
    int cmp = entryComparer(entryNode->entry, node->entry);
    if (cmp == EQ) {
        return node->entry;
    } else if (cmp == LT) {
        return CTLBinaryTreeFind(node->left, entryNode, entryComparer);
    } else {
        return CTLBinaryTreeFind(node->right, entryNode, entryComparer);
    }
}

bool CTLBinaryTreeEquals(const CTLBinaryNode *a, const CTLBinaryNode *b,
                         CTLCompareFunction entryComparer) {
    if (a == NULL || b == NULL) {
        return true;
    }
    if (a == NULL || b == NULL) {
        return false;
    }

    if (entryComparer(a->entry, b->entry) != EQ) {
        return false;
    }

    return CTLBinaryTreeEquals(a->left, b->left, entryComparer) &&
           CTLBinaryTreeEquals(a->right, b->right, entryComparer);
}

CTLBinaryNode *CTLCreateBinaryNode(const void *entry) {
    CTLBinaryNode *node = calloc(sizeof(CTLBinaryNode), 1);
    node->entry = entry;
    return node;
}

#ifdef DEBUG
#define PRINT_BUFFER_SIZE 512
void CTLPrintBinaryNode(CTLBinaryNode *node, const char *prefix,
                        const char *childPrefix) {
    char newPrefix[PRINT_BUFFER_SIZE];
    char newChildPrefix[PRINT_BUFFER_SIZE];

    printf("%s%d\n", prefix, *(int *)(node->entry));

    strcpy(newPrefix, childPrefix);
    strcat(newPrefix, "├── ");
    strcpy(newChildPrefix, childPrefix);
    strcat(newChildPrefix, "│   ");

    if (node->left != NULL) {
        CTLPrintBinaryNode(node->left, newPrefix, newChildPrefix);
    }

    memset(newPrefix, 0, PRINT_BUFFER_SIZE);
    memset(newChildPrefix, 0, PRINT_BUFFER_SIZE);

    strcpy(newPrefix, childPrefix);
    strcat(newPrefix, "└──");
    strcpy(newChildPrefix, childPrefix);
    strcat(newChildPrefix, "    ");

    if (node->right != NULL) {
        CTLPrintBinaryNode(node->right, newPrefix, newChildPrefix);
    }
}
#endif
