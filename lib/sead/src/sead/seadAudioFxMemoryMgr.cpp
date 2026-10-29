#include "sead/seadAudioFxMemoryMgr.h"
#include "sead/seadAudioFxMgr.h"
#include "sead/seadHeap.h"

namespace sead {
// 0x005465E8
void* sead::AudioFxMemoryMgr::alloc(u32 size, s32 alignment)
{
    return mHeap->tryAlloc(size, alignment);
}

// 0x005465F8 (name is ours)
void sead::AudioFxMemoryMgr::setup(AudioFxMgr* fxMgr, Heap* heap)
{
    mFxMgr = fxMgr;
    mHeap = heap;
}

// 0x00546604 | nintendogs:bytes [tier B]
void sead::AudioFxMemoryMgr::dispose()
{
    if (mFxMgr != nullptr) {
        mFxMgr->dispose(this);
    }
}

// 0x00546628 slot 0x00
// 0x00546620 (deleting dtor)
sead::AudioFxMemoryMgr::~AudioFxMemoryMgr()
{
    // empty (in the original too)
}
} // namespace sead
