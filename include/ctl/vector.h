#if !defined(CTL_VECTOR_H)
#define CTL_VECTOR_H
#include "ctl.h"
#include "ctl/compare.h"
#include "ctl/iterator.h"

typedef struct vec *CTLVector;

/**
 * @brief
 *
 * @param entryComprare
 * @param startCapacity
 * @return CTLVector
 */
CTLVector CTLVectorCreate(CTLCompareFunction entryComprare,
                          size_t startCapacity);

/**
 * @brief
 *
 * @param vec
 */
void CTLVectorFree(CTLVector *vec);

/**
 * @brief
 *
 * @param vec
 * @return size_t
 */
size_t CTLVectorSize(CTLVector vec);

/**
 * @brief
 *
 * @param vec
 * @return size_t
 */
size_t CTLVectorCapacity(CTLVector vec);

/**
 * @brief
 *
 * @param vec
 * @return true
 * @return false
 */
bool CTLVectorIsEmpty(CTLVector vec);

/**
 * @brief
 *
 * @param vec
 * @param index
 * @return void*
 */
void *CTLVectorGet(CTLVector vec, size_t index);

/**
 * @brief
 *
 * @param vec
 * @param value
 * @return true
 * @return false
 */
bool CTLVectorContains(CTLVector vec, const void *value);

/**
 * @brief
 *
 * @param vec
 * @param value
 */
void CTLVectorAdd(CTLVector vec, const void *value);

/**
 * @brief
 *
 * @param vec
 * @param values
 */
void CTLVectorAddAll(CTLVector vec, CTLIterator values);

/**
 * @brief
 *
 * @param vec
 * @param index
 * @param value
 */
void CTLVectorInsert(CTLVector vec, size_t index, const void *value);

/**
 * @brief
 *
 * @param vec
 * @param value
 * @return true
 * @return false
 */
bool CTLVectorRemove(CTLVector vec, const void *value);

/**
 * @brief
 *
 * @param vec
 * @param index
 * @return void*
 */
void *CTLVectorRemoveAt(CTLVector vec, size_t index);

/**
 * @brief
 *
 * @param vec
 * @param values
 * @return true
 * @return false
 */
bool CTLVectorContainsAll(CTLVector vec, CTLIterator values);

/**
 * @brief
 *
 * @param vec
 * @param values
 * @return true
 * @return false
 */
bool CTLVectorRemoveAll(CTLVector vec, CTLIterator values);

/**
 * @brief
 *
 * @param vec
 * @param predicate
 * @return bool
 */
bool CTLVectorRemoveIf(CTLVector vec, CTLpredicate predicate);

/**
 * @brief
 *
 * @param vec
 * @param predicate
 * @return true
 * @return false
 */
bool CTLVectorRetainIf(CTLVector vec, CTLpredicate predicate);

/**
 * @brief
 *
 * @param vec
 */
void CTLVectorClear(CTLVector vec);

/**
 * @brief
 *
 * @param vec
 * @return CTLIterator
 */
CTLIterator CTLVectorIterator(CTLVector vec);

#endif  // CTL_VECTOR_H
