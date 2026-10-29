#include "sead/seadAudioSoundHeapCtr.h"
#include "sead/seadHeap.h"
#include "sead/seadHeapMgr.h"
#include "sead/seadNew.h"

namespace sead {
// 0x0012CAD4 (name is ours)
sead::AudioSoundHeapCtr::AudioSoundHeapCtr(u32 size, Heap* heap)
    : SoundHeap(), hostio::Node(), mBuffer(nullptr), mDataMgr(nullptr), mArchive(nullptr)
{
    if (heap == nullptr) {
        heap = HeapMgr::s_pInstance->getCurrentHeap();
    }
    if (size == 0) {
        size = heap->getMaxAllocatableSize(32) - 32;
    }
    mBuffer = new (heap, 32) u8[size];
    Create(mBuffer, size);
}

// 0x00548B58 | nintendogs:bytes [tier A]
void sead::AudioSoundHeapCtr::setSoundDataManagement(nw::snd::SoundDataManager& dataMgr, nw::snd::SoundArchive& archive)
{
    mDataMgr = &dataMgr;
    mArchive = &archive;
}

// 0x00548BB0 slot 0x00
// 0x00548B64 (deleting dtor)
sead::AudioSoundHeapCtr::~AudioSoundHeapCtr()
{
    Destroy();
    if (mBuffer != nullptr) {
        delete[] mBuffer;
        mBuffer = nullptr;
    }
}
} // namespace sead
