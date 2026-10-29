#pragma once

#include "decomp.h"
#include "sead/seadAudioFxMemoryMgr.h"

namespace nw {
namespace snd {
class SoundHeap;
} // namespace snd
} // namespace nw

namespace sead {
// RTTI N4sead19AudioFxMemoryMgrCtrE @ 0x008D1C4C
// vtable 0x0090613C (vptr 0x00906144), offset_to_top 0, 2 entries
class AudioFxMemoryMgrCtr : public ::sead::AudioFxMemoryMgr
{
public:
    AudioFxMemoryMgrCtr(); // 0x00138D5C
    // (the original falls into the destructor of AudioFxMemoryMgr with a nop)
    virtual ~AudioFxMemoryMgrCtr(); // 0x00546624 slot 0x00
    // 0x0054A140 slot 0x04 (deleting dtor)

    // the memory of the effects of fxMgr: size bytes (plus the heap) from soundHeap as an ExpHeap
    // "AudioFx"; when the sound heap frees the block, the effects are taken out and the heap is destroyed
    void allocFromSoundHeap(AudioFxMgr* fxMgr, nw::snd::SoundHeap* soundHeap, u32 size); // 0x0054A0C0 (name is ours)

private:
    // the free callback of the sound heap (arg: the AudioFxMemoryMgrCtr)
    static void soundHeapFreeCallback_(void* memory, u32 size, void* arg); // 0x0054A08C (name is ours)
};
} // namespace sead
