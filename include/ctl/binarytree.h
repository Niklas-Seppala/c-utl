#if !defined(CTL_BINARY_TREE_H)
#define CTL_BINARY_TREE_H
#include "ctl.h"
#include "ctl/compare.h"

typedef struct ctlBinaryNode {
    struct ctlBinaryNode *left;
    struct ctlBinaryNode *right;
    const void *entry;
} CTLBinaryNode;

/**
 * @brief
 *
 * @param entry
 * @return CTLBinaryNode*
 */
CTLBinaryNode *CTLCreateBinaryNode(const void *entry);

/**
 * @brief
 *
 * @param rootNode
 * @param entryNode
 * @param entryComparer
 */
void CTLBinaryTreeAdd(CTLBinaryNode *rootNode, CTLBinaryNode *entryNode,
                      CTLCompareFunction entryComparer);

/**
 * @brief
 *
 * @param node
 * @param entryNode
 * @param entryComparer
 * @return const void*
 */
const void *CTLBinaryTreeFind(CTLBinaryNode *node, CTLBinaryNode *entryNode,
                              CTLCompareFunction entryComparer);

/**
 * @brief
 *
 * @param rootNode
 * @param entryNode
 * @param entryComparer
 * @return CTLBinaryNode*
 */
CTLBinaryNode *CTLBinaryTreeRemove(CTLBinaryNode *rootNode,
                                   const CTLBinaryNode *entryNode,
                                   CTLCompareFunction entryComparer);

/**
 * @brief
 *
 * @param a
 * @param b
 * @param entryComparer
 * @return true
 * @return false
 */
bool CTLBinaryTreeEquals(const CTLBinaryNode *a, const CTLBinaryNode *b,
                         CTLCompareFunction entryComparer);

/**
 * @brief
 *
 * @param node
 * @return const void*
 */
const CTLBinaryNode *CTLBinaryTreeFindMin(CTLBinaryNode *node);

/**
 * @brief
 *
 * @param node
 * @return const void*
 */
const CTLBinaryNode *CTLBinaryTreeFindMax(CTLBinaryNode *node);

#ifdef DEBUG
void CTLPrintBinaryNode(CTLBinaryNode *node, const char *prefix,
                        const char *childPrefix);
#endif

#endif  // CTL_BINARY_TREE_H
