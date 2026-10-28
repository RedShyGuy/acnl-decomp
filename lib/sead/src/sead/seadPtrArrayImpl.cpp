#include "sead/seadPtrArrayImpl.h"
#include "sead/seadRandom.h"

namespace sead {
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

} // namespace sead
