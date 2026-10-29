#include "pead/peadHeapMgr.h"
#include "pead/peadPtrArray.h"

#include <cstring>

namespace pead {
// 0x0053850C (name after sead in the nintendogs symbols)
void pead::PtrArrayImpl::freeBuffer()
{
    if (mPtrs != nullptr) {
        DeleteArray(reinterpret_cast<u8*>(mPtrs));
        mPtrs = nullptr;
        mPtrNum = 0;
        mPtrNumMax = 0;
    }
}

// 0x00538544 (name is ours)
void pead::PtrArrayImpl::allocBuffer(int ptrNumMax, Heap* heap, int alignment)
{
    if (ptrNumMax <= 0) {
        return;
    }
    setBuffer(ptrNumMax, NewArray<u8>(ptrNumMax * sizeof(void*), heap, alignment));
}

// 0x005385B0 (name is ours)
void pead::PtrArrayImpl::sort(CompareCallback cmp)
{
    if (mPtrNum <= 1) {
        return;
    }
    s32 lo = 0;
    s32 hi = mPtrNum - 1;
    while (true) {
        // the largest to the back
        s32 last = lo;
        for (s32 i = lo; i < hi; i++) {
            if (cmp(mPtrs[i], mPtrs[i + 1]) > 0) {
                void* ptr = mPtrs[i];
                mPtrs[i] = mPtrs[i + 1];
                mPtrs[i + 1] = ptr;
                last = i;
            }
        }
        hi = last;
        if (lo == hi) {
            return;
        }
        // the smallest to the front
        for (s32 i = last; i > lo; i--) {
            if (cmp(mPtrs[i], mPtrs[i - 1]) < 0) {
                void* ptr = mPtrs[i];
                mPtrs[i] = mPtrs[i - 1];
                mPtrs[i - 1] = ptr;
                last = i;
            }
        }
        lo = last;
        if (lo == hi) {
            return;
        }
    }
}

// 0x00538678 (name is ours)
void pead::PtrArrayImpl::erase(int index, int num)
{
    if (index < 0 || num < 0) {
        return;
    }
    s32 end = index + num;
    if (mPtrNum < end) {
        return;
    }
    if (mPtrNum > end) {
        std::memmove(&mPtrs[index], &mPtrs[end], (mPtrNum - end) * sizeof(void*));
    }
    mPtrNum -= num;
}

// 0x005386D0 (name is ours)
void pead::PtrArrayImpl::insert(int index, void* ptr)
{
    if (index < 0) {
        return;
    }
    if (mPtrNumMax < mPtrNum + 1 || mPtrNum < index) {
        return;
    }
    if (mPtrNum > index) {
        std::memmove(&mPtrs[index + 1], &mPtrs[index], (mPtrNum - index) * sizeof(void*));
    }
    mPtrs[index] = ptr;
    mPtrNum++;
}

// 0x00538734 (name is ours)
void pead::PtrArrayImpl::setBuffer(int ptrNumMax, void* buffer)
{
    if (ptrNumMax > 0 && buffer != nullptr) {
        mPtrNum = 0;
        mPtrNumMax = ptrNumMax;
        mPtrs = static_cast<void**>(buffer);
    }
}

// 0x007493EC | nintendogs:bytes
int pead::PtrArrayImpl::binarySearch(const void* ptr, CompareCallback cmp) const
{
    if (mPtrNum == 0) {
        return -1;
    }
    s32 lo = 0;
    s32 hi = mPtrNum - 1;
    while (lo < hi) {
        s32 mid = (lo + hi) / 2;
        s32 result = cmp(mPtrs[mid], ptr);
        if (result == 0) {
            return mid;
        }
        if (result > 0) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    if (cmp(mPtrs[lo], ptr) == 0) {
        return lo;
    }
    return -1;
}
} // namespace pead
