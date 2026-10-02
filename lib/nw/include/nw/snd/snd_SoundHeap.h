#pragma once

#include "decomp.h"
#include "nw/snd/snd_SoundMemoryAllocatable.h"

namespace nw {
namespace snd {
// RTTI N2nw3snd9SoundHeapE @ 0x008D0B6C
// vtable 0x00903550 (vptr 0x00903558), offset_to_top 0, 3 entries
class SoundHeap : public ::nw::snd::SoundMemoryAllocatable
{
public:
    virtual ~SoundHeap(); // 0x004D50D4 slot 0x00 | nintendogs:bytes
    virtual void vf_0x04(); // 0x004D502C slot 0x04 | virtual slot, introduced by nw::snd::SoundHeap
    virtual void Alloc(unsigned); // 0x004D4F2C slot 0x08 | nintendogs:bytes
    void SaveState(); // 0x0012BF60 | nintendogs:bytes [tier B]
    void Create(void*, unsigned); // 0x001327F4 | nintendogs:bytes [tier A]
    SoundHeap(); // 0x00132834 | nintendogs:bytes [tier A]
    void LoadState(int); // 0x0013C46C | nintendogs:bytes [tier B]
    void Destroy(); // 0x0013F564 | nintendogs:bytes [tier A]
    void DisposeCallbackFunc(void*, unsigned long, void*); // 0x004D4EE8 | fefates:bytes [tier B]
    void Alloc(unsigned, void(*)(void*, unsigned long, void*), void*); // 0x004D4F90 | nintendogs:bytes [tier B]
};
} // namespace snd
} // namespace nw
