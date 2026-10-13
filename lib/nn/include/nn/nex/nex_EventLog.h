#pragma once

#include "decomp.h"
#include "nn/nex/nex_Log.h"

namespace nn {
namespace nex {
// The log of the events: the lines start with the name of their level. The names of the members
// and of the levels are ours.
// RTTI N2nn3nex8EventLogE @ 0x008CF6D0
// vtable 0x008FFC4C (vptr 0x008FFC54), offset_to_top 0, 5 entries
class EventLog : public ::nn::nex::Log
{
public:
    enum Level
    {
        LEVEL_DEBUG = 1,
        LEVEL_INFO = 2,
        LEVEL_WARNING = 3,
        LEVEL_ERROR = 4,
        LEVEL_FATAL = 5,
        LEVEL_ALWAYS = 6
    };

    EventLog() : m_Level(LEVEL_INFO), m_Unknown0x11(0) {} // (inline)
    virtual ~EventLog(); // 0x003D5000 slot 0x00
    // 0x003D4F84 slot 0x04 (deleting dtor)
    virtual void AppendLevel(wchar_t* pBuffer, u32 size, u8 level); // 0x003D4EBC slot 0x10

    u8 m_Level;              // 0x10
    u8 m_Unknown0x11;        // 0x11
    u8 m_Unknown0x12[0xA];   // 0x12
};
ASSERT_SIZE(EventLog, 0x1C);
} // namespace nex
} // namespace nn
