#if !defined(CTL_BYTEBUFFER_H)
#define CTL_BYTEBUFFER_H

#include "ctl.h"

typedef struct bytebuffer {
    char *bytes;
    size_t size;
    size_t offset;
} CTLByteBuffer;

/**
 * @brief 
 * 
 * @param size 
 * @return CTLByteBuffer* 
 */
CTLByteBuffer *CTLHeapByteBufferAllocate(size_t size);

/**
 * @brief 
 * 
 * @param buffer 
 * @return CTLByteBuffer* 
 */
CTLByteBuffer *CTLByteBufferAllocateShare(CTLByteBuffer *buffer);

/**
 * @brief 
 * 
 * @param buffer 
 * @param start 
 * @param end 
 * @return CTLByteBuffer* 
 */
CTLByteBuffer *CTLByteBufferAllocateSlice(CTLByteBuffer *buffer, size_t start, size_t end);

/**
 * @brief 
 * 
 * @param bytes 
 * @param size 
 * @return CTLByteBuffer* 
 */
CTLByteBuffer *CTLByteBufferWrap(char *bytes, size_t size);

/**
 * @brief 
 * 
 * @param bytes 
 * @param start 
 * @param end 
 * @return CTLByteBuffer* 
 */
CTLByteBuffer *CTLByteBufferWrapSlice(char *bytes, size_t start, size_t end);

/**
 * @brief 
 * 
 * @param buffer 
 * @return size_t 
 */
size_t CTLByteBufferRemainingBytes(CTLByteBuffer *buffer);

/**
 * @brief 
 * 
 * @param buffer 
 * @param byte 
 */
void writeU8(CTLByteBuffer *buffer, uint8_t byte);

/**
 * @brief 
 * 
 * @param buffer 
 * @param byte 
 */
void writeU16(CTLByteBuffer *buffer, uint16_t byte);

/**
 * @brief 
 * 
 * @param buffer 
 * @param byte 
 */
void writeU32(CTLByteBuffer *buffer, uint32_t byte);

/**
 * @brief 
 * 
 * @param buffer 
 * @param byte 
 */
void writeU64(CTLByteBuffer *buffer, uint64_t byte);

/**
 * @brief 
 * 
 * @param buffer 
 * @param byte 
 */
void writeS8(CTLByteBuffer *buffer, int8_t byte);

/**
 * @brief 
 * 
 * @param buffer 
 * @param byte 
 */
void writeS16(CTLByteBuffer *buffer, int16_t byte);

/**
 * @brief 
 * 
 * @param buffer 
 * @param byte 
 */
void writeS32(CTLByteBuffer *buffer, int32_t byte);

/**
 * @brief 
 * 
 * @param buffer 
 * @param byte 
 */
void writeS64(CTLByteBuffer *buffer, int64_t byte);

/**
 * @brief 
 * 
 * @param buffer 
 * @return uint8_t 
 */
uint8_t readU8(CTLByteBuffer *buffer);

/**
 * @brief 
 * 
 * @param buffer 
 * @return uint16_t 
 */
uint16_t readU16(CTLByteBuffer *buffer);

/**
 * @brief 
 * 
 * @param buffer 
 * @return uint32_t 
 */
uint32_t readU32(CTLByteBuffer *buffer);

/**
 * @brief 
 * 
 * @param buffer 
 * @return uint64_t 
 */
uint64_t readU64(CTLByteBuffer *buffer);

/**
 * @brief 
 * 
 * @param buffer 
 * @return int8_t 
 */
int8_t readS8(CTLByteBuffer *buffer);

/**
 * @brief 
 * 
 * @param buffer 
 * @return int16_t 
 */
int16_t readS16(CTLByteBuffer *buffer);

/**
 * @brief 
 * 
 * @param buffer 
 * @return int32_t 
 */
int32_t readS32(CTLByteBuffer *buffer);

/**
 * @brief 
 * 
 * @param buffer 
 * @return int64_t 
 */
int64_t readS64(CTLByteBuffer *buffer);


#endif // CTL_BYTEBUFFER_H
