#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
class EventHandler;

// An event of an EventHandler: its flag is in the handler (set by Signal, cleared when a wait
// returns it). The names of the members and of the functions are ours.
// RTTI N2nn3nex5EventE @ 0x008CF52C
// vtable 0x008FF9C8 (vptr 0x008FF9D0), offset_to_top 0, 2 entries
class Event : public ::nn::nex::RootObject
{
public:
    // (inline; symbols.json names 0x006F899C so, but that is the constructor of a game class)
    Event(nn::nex::EventHandler* pHandler, unsigned int unknown0x08, unsigned int unknown0x0C)
        : m_pHandler(pHandler), m_Unknown0x08(unknown0x08), m_Unknown0x0C(unknown0x0C)
    {
    }
    virtual ~Event(); // 0x003CE9CC slot 0x00
    // 0x003CE9B4 slot 0x04 (deleting dtor)

    // sets or clears the flag of the event (inline; defined in nex_EventHandler.h)
    void Signal();
    void Reset();

    nn::nex::EventHandler* m_pHandler; // 0x04
    u32 m_Unknown0x08;                 // 0x08
    u32 m_Unknown0x0C;                 // 0x0C
    u32 m_Unknown0x10;                 // 0x10 (not set)
};
ASSERT_SIZE(Event, 0x14);
} // namespace nex
} // namespace nn
