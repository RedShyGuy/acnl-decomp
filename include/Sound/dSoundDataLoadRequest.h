#pragma once

#include "decomp.h"
#include "ssys/st/dListNode.h"

namespace nw { namespace snd { class SoundHeap; } }

// RTTI 20SoundDataLoadRequest @ 0x008CCC0C
// vtable 0x008F5B2C (vptr 0x008F5B34), offset_to_top 0, 2 entries
// A request to load the data of a sound archive item into a sound heap on the SoundDataLoadThread (member and method
// names are ours).
class SoundDataLoadRequest : public ::ssys::st::ListNode
{
public:
    enum State : u8
    {
        STATE_NONE = 0,
        STATE_QUEUED = 1,  // in the list of the thread
        STATE_LOADING = 2,
        STATE_DONE = 3,
        STATE_FAILED = 4,
    };

    SoundDataLoadRequest(); // 0x00128EEC
    virtual ~SoundDataLoadRequest(); // 0x0013E4E0
    // 0x003220C4 (deleting dtor)

    // queues the request (not while one is queued or loading)
    void Request(nw::snd::SoundHeap* heap, u32 itemId); // 0x0032206C
    // takes the request out of the list of the thread
    void Cancel(); // 0x0013E474
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsBusy() const // 0x0013CEF0
    {
        return m_State == STATE_QUEUED || m_State == STATE_LOADING;
    }

    volatile u8 m_State;          // 0x0C (State; the thread changes it)
    nw::snd::SoundHeap* m_pHeap;  // 0x10
    u32 m_ItemId;                 // 0x14
};
ASSERT_SIZE(SoundDataLoadRequest, 0x18);
