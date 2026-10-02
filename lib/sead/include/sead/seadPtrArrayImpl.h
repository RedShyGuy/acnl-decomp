#pragma once

#include "decomp.h"

namespace sead {
class PtrArrayImpl
{
public:
    void freeBuffer(); // 0x00541110 | nintendogs:bytes [tier A]
    void shuffle(sead::Random*); // 0x00541268 | mk7dlp:bytes [tier B]
    void binarySearch(const void*, int(*)(const void*, const void*)) const; // 0x007493EC | nintendogs:bytes [tier A]
};
} // namespace sead
