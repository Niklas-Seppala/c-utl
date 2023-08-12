#include "ctl/bytebuffer.h"

CTLByteBuffer *CTLHeapByteBufferAllocate(size_t size) {
    CTLByteBuffer *buffer = malloc(sizeof(CTLByteBuffer));
    buffer->size = size;
    buffer->offset = 0;
    buffer->bytes = calloc(size, sizeof(char));
    return buffer;
}

CTLByteBuffer *CTLByteBufferAllocateShare(CTLByteBuffer *buffer) {
    CTLByteBuffer *other = malloc(sizeof(CTLByteBuffer));
    other->size = buffer->size;
    other->offset = 0;
    other->bytes = buffer->bytes;
    return other;
}

CTLByteBuffer *CTLByteBufferAllocateSlice(CTLByteBuffer *buffer, size_t start, size_t end) {
    CTLByteBuffer *other = malloc(sizeof(CTLByteBuffer));
    other->offset = 0;
    other->size = end - start;
    other->bytes = buffer->bytes + start;
    return other;
}

CTLByteBuffer *CTLByteBufferWrap(char *bytes, size_t size) {
    CTLByteBuffer *buffer = malloc(sizeof(CTLByteBuffer));
    buffer->bytes = bytes;
    buffer->size = size;
    return buffer;
}

CTLByteBuffer *CTLByteBufferWrapSlice(char *bytes, size_t start, size_t end) {
    CTLByteBuffer *buffer = malloc(sizeof(CTLByteBuffer));
    buffer->bytes = bytes + start;
    buffer->size = end - start;
    return buffer;
}

inline size_t CTLByteBufferRemainingBytes(CTLByteBuffer * buffer) {
    return buffer->size - buffer->offset;
}


void writeU8(CTLByteBuffer *buffer, uint8_t byte) {

}

void writeU16(CTLByteBuffer *buffer, uint16_t byte) {

}

void writeU32(CTLByteBuffer *buffer, uint32_t byte) {

}

void writeU64(CTLByteBuffer *buffer, uint64_t byte) {

}


void writeS8(CTLByteBuffer *buffer, int8_t byte) {

}


void writeS16(CTLByteBuffer *buffer, int16_t byte) {

}


void writeS32(CTLByteBuffer *buffer, int32_t byte) {

}


void writeS64(CTLByteBuffer *buffer, int64_t byte) {

}


uint8_t readU8(CTLByteBuffer *buffer) {
    return 0;
}


uint16_t readU16(CTLByteBuffer *buffer) {
    return 0;
}


uint32_t readU32(CTLByteBuffer *buffer) {
    return 0;
}


uint64_t readU64(CTLByteBuffer *buffer) {
    return 0;
}


int8_t readS8(CTLByteBuffer *buffer) {
    return 0;
}

int16_t readS16(CTLByteBuffer *buffer) {
    return 0;
}

int32_t readS32(CTLByteBuffer *buffer) {
    return 0;
}

int64_t readS64(CTLByteBuffer *buffer) {
    return 0;
}
