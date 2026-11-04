#include "net/dNetBase64.h"

#include "nn/enc/enc_Api.h"
#include "nn/util/util_Base64.h"
#include "sead/seadHeap.h"
#include <cstring>

namespace net {
// 0x00953CBC (name is ours; set by the game)
sead::Heap* g_pWorkHeap;

// 0x0051AE28 (name is ours)
u32 EncodeBase64(const u8* src, u32 srcSize, char16* dst, u32 dstLength, sead::Heap* heap)
{
    if (heap == NULL) {
        heap = g_pWorkHeap;
        if (heap == NULL) {
            return 0;
        }
    }
    u32 workSize = srcSize * 2;
    char* work = static_cast<char*>(heap->tryAlloc(workSize, 4));
    if (work == NULL) {
        return 0;
    }
    u32 length = 0;
    if (nn::util::EncodeBase64(src, srcSize, work, workSize).IsSuccess()) {
        s32 workLength = std::strlen(work);
        s32 dstCount = dstLength;
        if (nn::enc::ConvertStringUtf8ToUtf16Native(reinterpret_cast<u16*>(dst), &dstCount,
                                                    reinterpret_cast<const u8*>(work), &workLength)
                .IsSuccess()) {
            if (static_cast<u32>(dstCount) > dstLength) {
                dstCount = dstLength;
            }
            length = dstCount;
            dst[length] = 0;
        }
    }
    heap->free(work);
    return length;
}

// 0x0051AF20 (name is ours)
u32 EncodeBase64(const u8* src, u32 srcSize, char16* dst, u32 dstLength, char* work, u32 workSize)
{
    if (work == NULL) {
        return 0;
    }
    u32 length = 0;
    if (nn::util::EncodeBase64(src, srcSize, work, workSize).IsSuccess()) {
        s32 workLength = std::strlen(work);
        s32 dstCount = dstLength;
        if (nn::enc::ConvertStringUtf8ToUtf16Native(reinterpret_cast<u16*>(dst), &dstCount,
                                                    reinterpret_cast<const u8*>(work), &workLength)
                .IsSuccess()) {
            if (static_cast<u32>(dstCount) > dstLength) {
                dstCount = dstLength;
            }
            length = dstCount;
            dst[length] = 0;
        }
    }
    return length;
}

// 0x0051AFAC (name is ours)
u32 DecodeBase64(const char16* src, u32 srcLength, u8* dst, u32 dstSize, sead::Heap* heap)
{
    if (heap == NULL) {
        heap = g_pWorkHeap;
        if (heap == NULL) {
            return 0;
        }
    }
    u32 workSize = dstSize * 2;
    char* work = static_cast<char*>(heap->tryAlloc(workSize, 4));
    if (work == NULL) {
        return 0;
    }
    u32 size = 0;
    s32 workCount = workSize;
    s32 srcCount = srcLength;
    if (nn::enc::ConvertStringUtf16NativeToUtf8(reinterpret_cast<u8*>(work), &workCount,
                                                reinterpret_cast<const u16*>(src), &srcCount)
            .IsSuccess()) {
        if (static_cast<u32>(workCount) >= workSize) {
            workCount = workSize - 1;
        }
        work[workCount] = '\0';
        u32 decodedSize;
        if (nn::util::DecodeBase64(work, dst, dstSize, &decodedSize).IsSuccess()) {
            size = decodedSize;
        }
    }
    heap->free(work);
    return size;
}

// 0x0051B09C (name is ours)
u32 DecodeBase64(const char16* src, u32 srcLength, u8* dst, u32 dstSize, char* work, u32 workSize)
{
    if (work == NULL) {
        return 0;
    }
    u32 size = 0;
    s32 workCount = workSize;
    s32 srcCount = srcLength;
    if (nn::enc::ConvertStringUtf16NativeToUtf8(reinterpret_cast<u8*>(work), &workCount,
                                                reinterpret_cast<const u16*>(src), &srcCount)
            .IsSuccess()) {
        if (static_cast<u32>(workCount) >= workSize) {
            workCount = workSize - 1;
        }
        work[workCount] = '\0';
        u32 decodedSize;
        if (nn::util::DecodeBase64(work, dst, dstSize, &decodedSize).IsSuccess()) {
            size = decodedSize;
        }
    }
    return size;
}
} // namespace net
