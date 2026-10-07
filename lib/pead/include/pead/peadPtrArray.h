#pragma once

// pead::PtrArrayImpl - an array of pointers with a fixed capacity. The class name and binarySearch
// are from the nintendogs symbols (sead); members, the template and the other names are ours.

#include "decomp.h"

namespace pead {
class Heap;

class PtrArrayImpl
{
public:
    typedef int (*CompareCallback)(const void* a, const void* b);

    PtrArrayImpl() : mPtrNum(0), mPtrNumMax(0), mPtrs(nullptr) {}

    // the memory of the pointers (name is ours)
    void setBuffer(int ptrNumMax, void* buffer); // 0x00538734
    // the memory of the pointers from the heap (name is ours)
    void allocBuffer(int ptrNumMax, Heap* heap, int alignment); // 0x00538544
    // frees the memory of allocBuffer (name after sead in the nintendogs symbols)
    void freeBuffer(); // 0x0053850C
    // removes num pointers from index on (name is ours)
    void erase(int index, int num); // 0x00538678
    // puts ptr before index (name is ours)
    void insert(int index, void* ptr); // 0x005386D0

    // index of the element that compares equal to ptr, a negative value if there is none
    int binarySearch(const void* ptr, CompareCallback cmp) const; // 0x007493EC | nintendogs:bytes
    void sort(CompareCallback cmp); // 0x005385B0 (name is ours)

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

    T* at(s32 index) const
    {
        if (static_cast<u32>(index) < static_cast<u32>(mPtrNum)) {
            return static_cast<T*>(mPtrs[index]);
        }
        return nullptr;
    }

    void pushBack(T* ptr)
    {
        if (mPtrNum < mPtrNumMax) {
            mPtrs[mPtrNum] = ptr;
            mPtrNum++;
        }
    }

    void insert(s32 index, T* ptr) { PtrArrayImpl::insert(index, ptr); }

    void clear() { mPtrNum = 0; }
};

// a PtrArray with the memory for N pointers in it (name is ours)
template <typename T, int N>
class FixedPtrArray : public PtrArray<T>
{
public:
    FixedPtrArray() { this->setBuffer(N, mWork); }

    T* mWork[N];
};

} // namespace pead
