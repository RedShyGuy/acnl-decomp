#pragma once

#include "decomp.h"

namespace sead {
class Random;

// An array of pointers with a fixed capacity (as pead::PtrArrayImpl). The class name and
// freeBuffer/shuffle are from the reference symbols; the members, the templates and setBuffer are
// ours (named as in pead).
class PtrArrayImpl
{
public:
    PtrArrayImpl() : mPtrNum(0), mPtrNumMax(0), mPtrs(nullptr) {}

    // the memory of the pointers (name is ours)
    void setBuffer(s32 ptrNumMax, void* buffer); // 0x005412B4
    // frees the memory of the pointers (allocated with new[])
    void freeBuffer(); // 0x00541110 | nintendogs:bytes [tier A]
    // puts the pointers into a random order
    void shuffle(Random* random); // 0x00541268 | mk7dlp:bytes [tier B]

    s32 mPtrNum;    // 0x0
    s32 mPtrNumMax; // 0x4
    void** mPtrs;   // 0x8
};
ASSERT_SIZE(PtrArrayImpl, 0xC);

template <typename T>
class PtrArray : public PtrArrayImpl
{
public:
    s32 size() const { return mPtrNum; }
    T* at(s32 index) const { return static_cast<T*>(mPtrs[index]); }
    T** begin() const { return reinterpret_cast<T**>(mPtrs); }
    T** end() const { return reinterpret_cast<T**>(mPtrs) + mPtrNum; }

    void pushBack(T* ptr)
    {
        if (mPtrNum < mPtrNumMax) {
            mPtrs[mPtrNum] = ptr;
            mPtrNum++;
        }
    }
};

// a PtrArray with the memory for N pointers in it (name is ours)
template <typename T, int N>
class FixedPtrArray : public PtrArray<T>
{
public:
    FixedPtrArray() { this->setBuffer(N, mWork); }

    T* mWork[N];
};
} // namespace sead
