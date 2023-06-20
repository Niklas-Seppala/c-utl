#include "ctl/vector.h"

#include <string.h>

#include "ctl/math.h"

#define DEFAULT_CAPACITY 10

/**
 * @brief
 *
 * @param vec
 */
static void resize(struct vector *vec);

/**
 * @brief Get the Remaining Count Exclusive object
 *
 * @param vec
 * @param index
 * @return size_t
 */
static size_t getRemainingCountExclusive(struct vector *vec, size_t index);
/**
 * @brief Get the Remaining Count Inclusive object
 *
 * @param vec
 * @param index
 * @return size_t
 */
static size_t getRemainingCountInclusive(struct vector *vec, size_t index);

/**
 * @brief
 *
 * @param stackVector
 * @param entryComparer
 * @param startCapacity
 * @return CTLVector
 */
static CTLVector initVector(CTLVector stackVector,
                            CTLCompareFunction entryComparer,
                            size_t startCapacity);

// -------------------------------------------------- //
// Public API implementations
// -------------------------------------------------- //

CTLVector CTLVectorHeapAlloc(CTLCompareFunction entryComparer,
                             size_t startCapacity) {
    NOT_NULL(entryComparer);
    CTLVector vec = calloc(sizeof(struct vector), 1);
    return initVector(vec, entryComparer, startCapacity);
}

void CTLVectorFree(CTLVector *vec) {
    NOT_NULL(*vec);
    NOT_NULL((*vec)->array)
    free((*vec)->array);
    free(*vec);
    *vec = NULL;
}

inline size_t CTLVectorSize(CTLVector vec) {
    NOT_NULL(vec);
    return vec->size;
}

inline size_t CTLVectorCapacity(CTLVector vec) {
    NOT_NULL(vec);
    return vec->capacity;
}

inline bool CTLVectorIsEmpty(CTLVector vec) {
    NOT_NULL(vec);
    return vec->size == 0;
}

inline void *CTLVectorGet(CTLVector vec, size_t index) {
    NOT_NULL(vec);
    INDEX_IN_UPPER_BOUND(index, vec->size);
    return vec->array[index];
}

void CTLVectorAdd(CTLVector vec, const void *value) {
    NOT_NULL(vec);
    if (vec->size == vec->capacity) {
        resize(vec);
    }
    vec->array[vec->size++] = (void *)value;
}

void CTLVectorAddAll(CTLVector vec, CTLIterator values) {
    NOT_NULL(vec);
    while (CTLIteratorHasNext(values)) {
        CTLVectorAdd(vec, CTLIteratorNext(values));
    }
}

void CTLVectorInsert(CTLVector vec, size_t index, const void *value) {
    NOT_NULL(vec);
    INDEX_IN_UPPER_BOUND(index, vec->size + 1);

    if (vec->size == vec->capacity) {
        resize(vec);
    }

    if (getRemainingCountInclusive(vec, index) > 0) {
        memmove(vec->array + (index + 1), vec->array + index,
                sizeof(uintptr_t) * getRemainingCountInclusive(vec, index));
    }

    vec->array[index] = (void *)value;
    vec->size++;
}

bool CTLVectorRemove(CTLVector vec, const void *value) {
    NOT_NULL(vec);
    for (size_t i = 0; i < vec->size; i++) {
        const void *element = vec->array[i];
        if (vec->entryComaparer(value, element) == EQ) {
            CTLVectorRemoveAt(vec, i);
            return true;
        }
    }
    return false;
}

void *CTLVectorRemoveAt(CTLVector vec, size_t index) {
    NOT_NULL(vec);
    INDEX_IN_UPPER_BOUND(index, vec->size);

    void *value = vec->array[index];
    if (getRemainingCountExclusive(vec, index) > 0) {
        memmove(vec->array + index, vec->array + (index + 1),
                sizeof(uintptr_t) * getRemainingCountExclusive(vec, index));
    }
    vec->size--;
    return value;
}

bool CTLVectorContains(CTLVector vec, const void *value) {
    NOT_NULL(vec);

    for (size_t i = 0; i < vec->size; i++) {
        const void *element = vec->array[i];
        if (vec->entryComaparer(value, element) == EQ) {
            return true;
        }
    }
    return false;
}

bool CTLVectorContainsAll(CTLVector vec, CTLIterator values) {
    NOT_NULL(vec);
    NOT_NULL(values);

    while (CTLIteratorHasNext(values)) {
        if (!CTLVectorContains(vec, CTLIteratorNext(values))) {
            return false;
        }
    }
    return true;
}

bool CTLVectorRemoveAll(CTLVector vec, CTLIterator values) {
    NOT_NULL(vec);
    NOT_NULL(values);

    bool result = false;
    while (CTLIteratorHasNext(values)) {
        result = CTLVectorRemove(vec, CTLIteratorNext(values));
    }
    return result;
}

void CTLVectorRemoveRange(CTLVector vec, size_t start, size_t end) {
    NOT_NULL(vec);
    INDEX_IN_UPPER_BOUND(start, vec->size + 1);
    INDEX_IN_UPPER_BOUND(start, end + 1);
    INDEX_IN_UPPER_BOUND(end, vec->size + 1);

    size_t count = end - start;
    for (size_t i = 0; i < count; i++) {
        CTLVectorRemoveAt(vec, start);
    }
}

bool CTLVectorRetainAll(CTLVector vec, CTLIterator values) {
    NOT_NULL(vec);
    NOT_NULL(values);

    bool result = false;

    for (size_t i = 0; i < vec->size; i++) {
        void *element = vec->array[i];
        bool found = false;
        while (CTLIteratorHasNext(values)) {
            if (vec->entryComaparer(CTLIteratorNext(values), element) == EQ) {
                found = true;
                break;
            }
        }
        if (!found) {
            result = true;
            CTLVectorRemoveAt(vec, i);
            i--;
        }
        // Reuse the same iterator.
        CTLIteratorResetHead(values);
    }

    return result;
}

bool CTLVectorRemoveIf(CTLVector vec, CTLpredicate predicate) {
    NOT_NULL(vec);
    NOT_NULL(predicate);

    bool result = false;
    for (size_t i = 0; i < vec->size; i++) {
        const void *element = vec->array[i];
        if (predicate(element)) {
            CTLVectorRemoveAt(vec, i--);  // Because vec->size--;
            result = true;
        }
    }
    return result;
}

bool CTLVectorRetainIf(CTLVector vec, CTLpredicate predicate) {
    NOT_NULL(vec);
    NOT_NULL(predicate);

    bool result = false;
    for (size_t i = 0; i < vec->size; i++) {
        const void *element = vec->array[i];
        if (!predicate(element)) {
            CTLVectorRemoveAt(vec, i--);  // Because vec->size--;
            result = true;
        }
    }
    return result;
}

inline void CTLVectorClear(CTLVector vec) { vec->size = 0; }

CTLIterator CTLVectorIterator(CTLVector vec) {
    return CTLIteratorAllocateArrayIterator(vec->array, vec->size);
}

static void resize(struct vector *vec) {
    vec->capacity <<= 1;
    vec->array = realloc(vec->array, vec->capacity * sizeof(uintptr_t));
}

inline static size_t getRemainingCountExclusive(struct vector *vec,
                                                size_t index) {
    return vec->size - (index + 1);
}

inline static size_t getRemainingCountInclusive(struct vector *vec,
                                                size_t index) {
    return vec->size - index;
}

static CTLVector initVector(CTLVector vector, CTLCompareFunction entryComparer,
                            size_t startCapacity) {
    if (startCapacity <= 0) {
        startCapacity = DEFAULT_CAPACITY;
    }

    vector->array = calloc(sizeof(void **), startCapacity);
    vector->capacity = startCapacity;
    vector->entryComaparer = entryComparer;
    vector->size = 0;
    return vector;
}