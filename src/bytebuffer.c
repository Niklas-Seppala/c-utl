#include "ctl/bytebuffer.h"
#include <string.h>
#include <assert.h>

#define BUFFER_OVERFLOW_ASSERT_ERROR() {assert(false && "Prevented buffer overlow"); return 0; }

CTLByteBuffer *CTLHeapByteBufferAllocate(size_t size) {
    CTLByteBuffer *buffer = malloc(sizeof(CTLByteBuffer));
    buffer->size = size;
    buffer->offset = 0;
    buffer->backingBufferIsAllowedToResize = true;
    buffer->bytes = calloc(size, sizeof(char));
    return buffer;
}

CTLByteBuffer *CTLByteBufferAllocateShare(CTLByteBuffer *buffer) {
    CTLByteBuffer *other = malloc(sizeof(CTLByteBuffer));
    other->size = buffer->size;
    other->offset = 0;
    other->backingBufferIsAllowedToResize = buffer->backingBufferIsAllowedToResize;
    other->bytes = buffer->bytes;
    return other;
}

CTLByteBuffer *CTLByteBufferAllocateSlice(CTLByteBuffer *buffer, size_t start, size_t end) {
    CTLByteBuffer *other = malloc(sizeof(CTLByteBuffer));
    other->offset = 0;
    other->backingBufferIsAllowedToResize = false;
    other->size = end - start;
    other->bytes = buffer->bytes + start;
    return other;
}

CTLByteBuffer *CTLByteBufferWrap(uint8_t *bytes, size_t size) {
    CTLByteBuffer *buffer = malloc(sizeof(CTLByteBuffer));
    buffer->bytes = bytes;
    buffer->backingBufferIsAllowedToResize = false;
    buffer->size = size;
    return buffer;
}

CTLByteBuffer *CTLByteBufferWrapSlice(uint8_t *bytes, size_t start, size_t end) {
    CTLByteBuffer *buffer = malloc(sizeof(CTLByteBuffer));
    buffer->bytes = bytes + start;
    buffer->size = end - start;
    return buffer;
}

inline size_t CTLByteBufferRemainingBytes(CTLByteBuffer * buffer) {
    return buffer->size - buffer->offset;
}

static void reallocIfBufferIsFull(CTLByteBuffer *buffer) {
    if (buffer->offset == buffer->size && buffer->backingBufferIsAllowedToResize) {
        size_t newSize = buffer->size * 2;
        uint8_t *reallocated = realloc(buffer->bytes, newSize);
        assert(reallocated != NULL && "failed to realloc underlying byte buffer");
        if (reallocated != NULL) {
            buffer->bytes = reallocated;
            buffer->size = newSize;
        }
    }
}

void CTLByteBufferJumpToOffset(CTLByteBuffer *buffer, size_t newOffset) {
    assert(newOffset < buffer->size && "newOffset is larger than buffer size");
    buffer->offset = newOffset;
}

void writeU8(CTLByteBuffer *buffer, uint8_t value) {
    reallocIfBufferIsFull(buffer);
    *(buffer->bytes + buffer->offset++) = value & 0xff;
}

void writeU16(CTLByteBuffer *buffer, uint16_t value) {
    reallocIfBufferIsFull(buffer);
    *(buffer->bytes + buffer->offset++) = value & 0xff;
    *(buffer->bytes + buffer->offset++) = (value >> 0x8) & 0xff;
}

void writeU32(CTLByteBuffer *buffer, uint32_t value) {
    reallocIfBufferIsFull(buffer);
    *(buffer->bytes + buffer->offset++) =  value & 0xff;
    *(buffer->bytes + buffer->offset++) = (value >> 8)  & 0xff;
    *(buffer->bytes + buffer->offset++) = (value >> 16) & 0xff;
    *(buffer->bytes + buffer->offset++) = (value >> 24) & 0xff;
}

void writeU64(CTLByteBuffer *buffer, uint64_t value) {
    reallocIfBufferIsFull(buffer);
    *(buffer->bytes + buffer->offset++) =  value & 0xff;
    *(buffer->bytes + buffer->offset++) = (value >> 8)  & 0xff;
    *(buffer->bytes + buffer->offset++) = (value >> 16) & 0xff;
    *(buffer->bytes + buffer->offset++) = (value >> 24) & 0xff;
    *(buffer->bytes + buffer->offset++) = (value >> 32) & 0xff;
    *(buffer->bytes + buffer->offset++) = (value >> 40) & 0xff;
    *(buffer->bytes + buffer->offset++) = (value >> 48) & 0xff;
    *(buffer->bytes + buffer->offset++) = (value >> 56) & 0xff;
}


void writeS8(CTLByteBuffer *buffer, int8_t value) {
    reallocIfBufferIsFull(buffer);
    *(buffer->bytes + buffer->offset++) = value & 0xff;
}


void writeS16(CTLByteBuffer *buffer, int16_t value) {
    reallocIfBufferIsFull(buffer);
    *(buffer->bytes + buffer->offset++) = value & 0xff;
    *(buffer->bytes + buffer->offset++) = (value >> 0x8) & 0xff;
}


void writeS32(CTLByteBuffer *buffer, int32_t value) {
    reallocIfBufferIsFull(buffer);
    *(buffer->bytes + buffer->offset++) =  value & 0xff;
    *(buffer->bytes + buffer->offset++) = (value >> 8)  & 0xff;
    *(buffer->bytes + buffer->offset++) = (value >> 16) & 0xff;
    *(buffer->bytes + buffer->offset++) = (value >> 24) & 0xff;
}


void writeS64(CTLByteBuffer *buffer, int64_t value) {
    reallocIfBufferIsFull(buffer);
    *(buffer->bytes + buffer->offset++) =  value & 0xff;
    *(buffer->bytes + buffer->offset++) = (value >> 8)  & 0xff;
    *(buffer->bytes + buffer->offset++) = (value >> 16) & 0xff;
    *(buffer->bytes + buffer->offset++) = (value >> 24) & 0xff;
    *(buffer->bytes + buffer->offset++) = (value >> 32) & 0xff;
    *(buffer->bytes + buffer->offset++) = (value >> 40) & 0xff;
    *(buffer->bytes + buffer->offset++) = (value >> 48) & 0xff;
    *(buffer->bytes + buffer->offset++) = (value >> 56) & 0xff;
}

static inline bool noBufferOverflow(CTLByteBuffer *buffer, int size) {
    return buffer->offset + size < buffer->size;
}


uint8_t readU8(CTLByteBuffer *buffer) {
    if (noBufferOverflow(buffer, sizeof(uint8_t))) {
        uint8_t value = *(uint8_t *)buffer->bytes + buffer->offset;
        buffer->offset += sizeof(uint8_t);
        return value;
    } else {
        BUFFER_OVERFLOW_ASSERT_ERROR();
    }
}

uint16_t readU16(CTLByteBuffer *buffer) {
    if (noBufferOverflow(buffer, sizeof(uint16_t))) {
        uint16_t value = *(uint16_t *)buffer->bytes + buffer->offset;
        buffer->offset += sizeof(uint16_t);
        return value;
    } else {
        BUFFER_OVERFLOW_ASSERT_ERROR();
    }
}

uint32_t readU32(CTLByteBuffer *buffer) {
    if (noBufferOverflow(buffer, sizeof(uint32_t)))  {
        uint32_t value = *(uint32_t *)buffer->bytes + buffer->offset;
        buffer->offset += sizeof(uint32_t);
        return value;
    } else {
        BUFFER_OVERFLOW_ASSERT_ERROR();
    }
}

uint64_t readU64(CTLByteBuffer *buffer) {
    if (noBufferOverflow(buffer, sizeof(uint64_t))) {
        uint64_t value = *(uint64_t *)buffer->bytes + buffer->offset;
        buffer->offset += sizeof(uint64_t);
        return value;
    } else {
        BUFFER_OVERFLOW_ASSERT_ERROR();
    }
}

int8_t readS8(CTLByteBuffer *buffer) {
    if (noBufferOverflow(buffer, sizeof(int8_t))) {
        int8_t value = *(int8_t *)buffer->bytes + buffer->offset;
        buffer->offset += sizeof(int8_t);
        return value;
    } else {
        BUFFER_OVERFLOW_ASSERT_ERROR();
    }
}

int16_t readS16(CTLByteBuffer *buffer) {
    if (noBufferOverflow(buffer, sizeof(int16_t))) {
        int16_t value = *(int16_t *)buffer->bytes + buffer->offset;
        buffer->offset += sizeof(int16_t);
        return value;
    } else {
        BUFFER_OVERFLOW_ASSERT_ERROR();
    }
}

int32_t readS32(CTLByteBuffer *buffer) {
    if (noBufferOverflow(buffer, sizeof(int32_t))) {
        int32_t value = *(int32_t *)buffer->bytes + buffer->offset;
        buffer->offset += sizeof(int32_t);
        return value;
    } else {
        BUFFER_OVERFLOW_ASSERT_ERROR();
    }
}

int64_t readS64(CTLByteBuffer *buffer) {
    if (noBufferOverflow(buffer, sizeof(int64_t))) {
        int64_t value = *(int64_t *)buffer->bytes + buffer->offset;
        buffer->offset += sizeof(int64_t);
        return value;
    } else {
        BUFFER_OVERFLOW_ASSERT_ERROR();
    }
}
