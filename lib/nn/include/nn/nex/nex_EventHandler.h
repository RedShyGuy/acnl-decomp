#pragma once

#include "decomp.h"
#include "nn/nex/nex_CriticalSection.h"
#include "nn/nex/nex_Event.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// A set of events (up to a fixed number) with a flag each. WaitForBoolEvent polls the flags. The
// names of the members, of the flag classes and of the functions without a reference name are
// ours.
// RTTI N2nn3nex12EventHandlerE @ 0x008CE0BC
// vtable 0x008FC4BC (vptr 0x008FC4C4), offset_to_top 0, 2 entries
class EventHandler : public ::nn::nex::RootObject
{
public:
    // the flag of an event
    class Flag
    {
    public:
        Flag() : m_IsSet(false) {}
        ~Flag() {}

        bool m_IsSet;
    };
    // the flags of the events (by the index of the event)
    class Flags
    {
    public:
        Flags() : m_pFlags(nullptr) {}

        Flag* m_pFlags;
    };

    explicit EventHandler(unsigned short size); // 0x0035C88C | fefates:callseq [tier C]
    virtual ~EventHandler(); // 0x0035C998 slot 0x00 | fefates:callseq [tier C]
    // 0x0035C968 slot 0x04 (deleting dtor)

    // a new event in the first free slot
    nn::nex::Event* CreateEventObject(unsigned int unknown0x08, unsigned int unknown0x0C); // 0x0035C774 | fefates:callseq-callee [tier C]
    // the slot of the event (0 if it is not there)
    unsigned short GetEventIndex(nn::nex::Event* pEvent) const; // 0x0013CF0C | fefates:callseq-callee [tier C]
    // waits until the flag of one of the events is set (checked round robin) and clears it; false
    // after the timeout (an error unless isTimeoutExpected)
    bool WaitForBoolEvent(unsigned int timeout, nn::nex::Event** ppEvent, bool isTimeoutExpected) const; // 0x0072A390 | mk7dlp:callseq [tier C]

    // (inline) frees the slot of the event and deletes it
    void DeleteEventObject(nn::nex::Event* pEvent)
    {
        ScopedCS lock(m_Lock);
        m_pFlags->m_pFlags[GetEventIndex(pEvent)].m_IsSet = false;
        m_ppEvents[GetEventIndex(pEvent)] = nullptr;
        delete pEvent;
    }

    mutable CriticalSection m_Lock; // 0x04
    Flags* m_pFlags;                // 0x2C
    Event** m_ppEvents;             // 0x30
    u32 m_EventCount;               // 0x34 (the events created, also the deleted ones)
    u16 m_Size;                     // 0x38

    // the slot where WaitForBoolEvent starts when no event is set (a global counter)
    static u32 s_WaitStartIndex;
};
ASSERT_OFFSET(EventHandler, m_pFlags, 0x2C);
ASSERT_SIZE(EventHandler, 0x3C);

inline void Event::Signal()
{
    m_pHandler->m_pFlags->m_pFlags[m_pHandler->GetEventIndex(this)].m_IsSet = true;
}

inline void Event::Reset()
{
    m_pHandler->m_pFlags->m_pFlags[m_pHandler->GetEventIndex(this)].m_IsSet = false;
}
} // namespace nex
} // namespace nn
