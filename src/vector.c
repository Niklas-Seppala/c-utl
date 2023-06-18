#include "ctl/vector.h"

#include <string.h>

#include "ctl/math.h"

#define DEFAULT_CAPACITY 10

struct vec {
    size_t capacity;
    size_t size;
    void **array;
    CTLCompareFunction entryComaparer;
};

/**
 * @brief
 *
 * @param vec
 */
static void resize(struct vec *vec);

/**
 * @brief Get the Remaining Count Exclusive object
 *
 * @param vec
 * @param index
 * @return size_t
 */
static size_t getRemainingCountExclusive(struct vec *vec, size_t index);
/**
 * @brief Get the Remaining Count Inclusive object
 *
 * @param vec
 * @param index
 * @return size_t
 */
static size_t getRemainingCountInclusive(struct vec *vec, size_t index);

CTLVector CTLVectorCreate(CTLCompareFunction entryComprare,
                          size_t startCapacity) {
    NOT_NULL(entryComprare);
    CTLVector vec = calloc(sizeof(struct vec), 1);

    if (startCapacity <= 0) {
        startCapacity = DEFAULT_CAPACITY;
    }

    vec->array = calloc(sizeof(void **), startCapacity);
    vec->capacity = startCapacity;
    vec->entryComaparer = entryComprare;
    vec->size = 0;
    return vec;
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
    UNUSED_PARAMETER(vec);
    TODO("This is not yet implemented");
    return NULL;
}

static void resize(struct vec *vec) {
    vec->capacity <<= 1;
    vec->array = realloc(vec->array, vec->capacity * sizeof(uintptr_t));
}

inline static size_t getRemainingCountExclusive(struct vec *vec, size_t index) {
    return vec->size - (index + 1);
}

inline static size_t getRemainingCountInclusive(struct vec *vec, size_t index) {
    return vec->size - index;
}
