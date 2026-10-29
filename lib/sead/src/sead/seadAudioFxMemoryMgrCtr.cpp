#include "sead/seadAudioFxMemoryMgr.h"
#include "sead/seadAudioFxMemoryMgrCtr.h"
#include "sead/seadExpHeap.h"
#include "nw/snd/snd_SoundHeap.h"

namespace sead {
// (the constructor of AudioFxMemoryMgr is out of line in the original: 0x0013C69C)
// 0x00138D5C (name is ours)
sead::AudioFxMemoryMgrCtr::AudioFxMemoryMgrCtr() : AudioFxMemoryMgr()
{
}

// 0x00546624 slot 0x00
// 0x0054A140 (deleting dtor)
sead::AudioFxMemoryMgrCtr::~AudioFxMemoryMgrCtr()
{
    // empty (in the original too)
}

// 0x0054A08C (name is ours)
void sead::AudioFxMemoryMgrCtr::soundHeapFreeCallback_(void* memory, u32 size, void* arg)
{
    AudioFxMemoryMgrCtr* self = static_cast<AudioFxMemoryMgrCtr*>(arg);
    self->dispose();
    if (self->mHeap != nullptr) {
        self->mHeap->destroy();
        self->mHeap = nullptr;
    }
}

// 0x0054A0C0 (name is ours)
void sead::AudioFxMemoryMgrCtr::allocFromSoundHeap(AudioFxMgr* fxMgr, nw::snd::SoundHeap* soundHeap, u32 size)
{
    // the heap object, a block header and 4 bytes more (ARMCC forms the constant as 4 + 168; our reading
    // of the sum)
    size += sizeof(ExpHeap) + sizeof(ExpHeap::MemBlock) + 4;
    void* buffer = soundHeap->Alloc(size, &soundHeapFreeCallback_, this);
    SafeStringBase<char> name("AudioFx");
    Heap* heap = ExpHeap::CreateRoot(buffer, size, &name, true);
    setup(fxMgr, heap);
}
} // namespace sead
