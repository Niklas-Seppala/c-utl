#if !defined(CTL_BTREE_H)
#define CTL_BTREE_H
#include "ctl.h"
#include "ctl/binarytree.h"
#include "ctl/compare.h"
#include "ctl/iterator.h"

typedef struct __CTLTreeSet *CTLTreeSet;

/**
 * @brief
 *
 * @param entryComparer
 * @return CTLTreeSet
 */
CTLTreeSet CTLTreeSetCreate(CTLCompareFunction entryComparer);

/**
 * @brief
 *
 * @param set
 * @return const CTLBinaryNode*
 */
const CTLBinaryNode *CTLTreeSetAsBinaryTree(CTLTreeSet set);

/**
 * @brief
 *
 * @param tree
 * @param entry
 */
void CTLTreeSetAdd(const CTLTreeSet tree, const void *entry);

/**
 * @brief
 *
 * @param tree
 * @return size_t
 */
size_t CTLTreeSetSize(const CTLTreeSet tree);

/**
 * @brief
 *
 * @param tree
 * @param entry
 * @return void*
 */
const void *CTLTreeSetFind(const CTLTreeSet tree, const void *entry);

/**
 * @brief
 *
 * @param tree
 * @param entry
 * @return true
 * @return false
 */
bool CTLTreeSetContains(const CTLTreeSet tree, const void *entry);

/**
 * @brief
 *
 * @param tree
 * @param entry
 * @return const void*
 */
const void *CTLTreeSetRemove(const CTLTreeSet tree, const void *entry);

/**
 * @brief
 *
 * @param tree
 * @param entry
 * @return CTLIterator
 */
CTLIterator CTLTreeSetIterator(const CTLTreeSet tree, const void *entry);

/**
 * @brief
 *
 * @param tree
 * @param entry
 * @return true
 * @return false
 */
bool CTLTreeSetContains(const CTLTreeSet tree, const void *entry);

/**
 * @brief
 *
 * @param a
 * @param b
 * @return true
 * @return false
 */
bool CTLTreeSetEquals(const CTLTreeSet a, const CTLTreeSet b);

#endif  // CTL_BTREE_H
