#pragma once

#include "decomp.h"
#include "nw/snd/snd_SoundMemoryAllocatable.h"
#include "nw/ut/internal/ut_LinkListImpl.h"

namespace nw {
namespace snd {
class SoundPlayer;

namespace internal {
// A heap of a sound player for the data of one sound: allocations go up from the start, Clear frees all
// (the class and function names are from the reference symbols, the members are ours).
// RTTI N2nw3snd8internal10PlayerHeapE @ 0x008D0984
class PlayerHeap : public ::nw::snd::SoundMemoryAllocatable
{
public:
    // (names are ours)
    enum State : u8
    {
        STATE_CONSTRUCTED = 0,
        STATE_INITIALIZED = 1,
        STATE_TASK_FINISHED = 2, // the load task of the heap is done
        STATE_DESTRUCTED = 3,
    };
    // the alignment of the allocations
    static const u32 ALIGNMENT = 32;

    PlayerHeap(); // 0x004C5E70 | fefates:bytes [tier B]
    virtual ~PlayerHeap(); // 0x004C5EC4 slot 0x00 | mk7dlp:callseq (symbols.json: TaskThread::ThreadProc)

    virtual void* Alloc(u32 size); // 0x004C5E0C slot 0x08 | nintendogs:bytes

    // the heap in the memory; false if it is too small for the alignment
    bool Create(void* pMemory, u32 size); // 0x004C5E44 | nintendogs:bytes [tier A]
    void Clear(); // 0x004C5E38 | nintendogs:bytes [tier A]

    void* m_pSound;             // 0x04
    SoundPlayer* m_pPlayer;     // 0x08
    u8* m_pStartAddress;        // 0x0C
    u8* m_pEndAddress;          // 0x10
    u8* m_pAllocAddress;        // 0x14
    State m_State;              // 0x18
    ut::LinkListNode m_Link;    // 0x1C (SoundPlayer)
};
ASSERT_SIZE(PlayerHeap, 0x24);
} // namespace internal
} // namespace snd
} // namespace nw
