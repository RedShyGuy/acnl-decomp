#pragma once

#include "decomp.h"
#include "nn/os/os_CriticalSection.h"
#include "nw/snd/internal/snd_FrameHeap.h"
#include "nw/snd/snd_SoundMemoryAllocatable.h"

namespace nw {
namespace snd {
// The heap of the game for sound data: a FrameHeap under a lock; freed blocks tell the sound thread
// that their data goes away (the class and function names are from the reference symbols, the members
// are ours).
// RTTI N2nw3snd9SoundHeapE
class SoundHeap : public ::nw::snd::SoundMemoryAllocatable
{
public:
    SoundHeap(); // 0x00132834 | nintendogs:bytes [tier A]
    virtual ~SoundHeap(); // 0x004D50D4 slot 0x00 | nintendogs:bytes

    virtual void* Alloc(u32 size); // 0x004D4F2C slot 0x08 | nintendogs:bytes
    void* Alloc(u32 size, internal::FrameHeap::FreeCallback callback, void* pCallbackArg); // 0x004D4F90 | nintendogs:bytes [tier B]

    bool Create(void* pStartAddress, u32 size); // 0x001327F4 | nintendogs:bytes [tier A]
    void Destroy(); // 0x0013F564 | nintendogs:bytes [tier A]
    // frees all and waits until the sound thread is done with the data
    void Clear(); // 0x004D4FD8 (name is ours)
    // a new level of the heap (-1: no memory)
    int SaveState(); // 0x0012BF60 | nintendogs:bytes [tier B]
    void LoadState(int level); // 0x0013C46C | nintendogs:bytes [tier B]

    // tells the sound thread that the block goes away
    static void DisposeCallbackFunc(void* pMemory, u32 size, void* pArg); // 0x004D4EE8 | fefates:bytes [tier B]

    nn::os::CriticalSection m_CriticalSection; // 0x04
    internal::FrameHeap m_FrameHeap;           // 0x10
};
ASSERT_SIZE(SoundHeap, 0x20);
} // namespace snd
} // namespace nw
