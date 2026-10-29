#include "sead/seadPtrArrayImpl.h"
#include "sead/seadNew.h"
#include "sead/seadRandom.h"

#include <cstring>

namespace sead {
// 0x001257D0 (name is ours)
void sead::PtrArrayImpl::allocBuffer(s32 ptrNumMax, Heap* heap, s32 alignment)
{
    if (ptrNumMax <= 0) {
        return;
    }
    void* buffer = ::operator new(ptrNumMax * sizeof(void*), heap, alignment, std::nothrow);
    if (buffer != nullptr) {
        mPtrs = static_cast<void**>(buffer);
        mPtrNum = 0;
        mPtrNumMax = ptrNumMax;
    }
}

// 0x005412B4 (name is ours)
void sead::PtrArrayImpl::setBuffer(s32 ptrNumMax, void* buffer)
{
    if (ptrNumMax > 0 && buffer != nullptr) {
        mPtrNum = 0;
        mPtrNumMax = ptrNumMax;
        mPtrs = static_cast<void**>(buffer);
    }
}

// 0x00541110 | nintendogs:bytes [tier A]
void sead::PtrArrayImpl::freeBuffer()
{
    if (mPtrs != nullptr) {
        delete[] mPtrs;
        mPtrs = nullptr;
        mPtrNum = 0;
        mPtrNumMax = 0;
    }
}

// 0x00541268 | mk7dlp:bytes [tier B]
void sead::PtrArrayImpl::shuffle(Random* random)
{
    s32 i = mPtrNum;
    while (i > 1) {
        u32 j = random->getU32(i);
        i--;
        void* ptr = mPtrs[i];
        mPtrs[i] = mPtrs[j];
        mPtrs[j] = ptr;
    }
}

// 0x0054113C (name is ours)
void sead::PtrArrayImpl::sort(CompareCallback cmp)
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

// 0x00541204 (name is ours)
void sead::PtrArrayImpl::insert(s32 pos, void* ptr)
{
    if (pos < 0) {
        return;
    }
    if (mPtrNumMax < mPtrNum + 1 || mPtrNum < pos) {
        return;
    }
    if (mPtrNum > pos) {
        std::memmove(&mPtrs[pos + 1], &mPtrs[pos], (mPtrNum - pos) * sizeof(void*));
    }
    mPtrs[pos] = ptr;
    mPtrNum++;
}

// 0x00138C60 (name is ours)
void sead::PtrArrayImpl::erase(s32 pos, s32 count)
{
    if (pos < 0 || count < 0) {
        return;
    }
    s32 end = pos + count;
    if (mPtrNum < end) {
        return;
    }
    if (mPtrNum > end) {
        std::memmove(&mPtrs[pos], &mPtrs[end], (mPtrNum - end) * sizeof(void*));
    }
    mPtrNum -= count;
}

} // namespace sead
