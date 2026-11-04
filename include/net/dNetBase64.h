#pragma once

#include "decomp.h"

namespace sead {
class Heap;
} // namespace sead

namespace net {
// Binary data as Base64 text in UTF-16 and back (the work buffer is from heap, by default g_pWorkHeap,
// or given). The results are the characters / bytes written. (Names are ours.)
u32 EncodeBase64(const u8* src, u32 srcSize, char16* dst, u32 dstLength, sead::Heap* heap); // 0x0051AE28
u32 EncodeBase64(const u8* src, u32 srcSize, char16* dst, u32 dstLength, char* work, u32 workSize); // 0x0051AF20
u32 DecodeBase64(const char16* src, u32 srcLength, u8* dst, u32 dstSize, sead::Heap* heap); // 0x0051AFAC
u32 DecodeBase64(const char16* src, u32 srcLength, u8* dst, u32 dstSize, char* work, u32 workSize); // 0x0051B09C

extern sead::Heap* g_pWorkHeap;
} // namespace net
