#pragma once

#include "decomp.h"
#include "nn/nex/nex_CriticalSection.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// The bundling of the packets of a stream (part of StreamSettings). The layout is from the
// constructor of StreamSettings; the member names are ours.
// RTTI N2nn3nex14StreamBundlingE @ 0x008CE3DC
// vtable 0x008FCD68 (vptr 0x008FCD70), offset_to_top 0, 2 entries
class StreamBundling : public ::nn::nex::RootObject
{
public:
    // (inline)
    StreamBundling()
        : m_IsEnabled(0), m_Interval(0), m_LastTime(0), m_Unknown0x18(0xFFFF), m_Unknown0x1A(0xFFFF), m_Lock(0x20000000)
    {
    }
    virtual ~StreamBundling(); // 0x003745A4 slot 0x00
    // 0x00374568 slot 0x04 (deleting dtor)

    // (inline) the values without the lock
    StreamBundling& operator=(const StreamBundling& other)
    {
        m_IsEnabled = other.m_IsEnabled;
        m_Interval = other.m_Interval;
        m_LastTime = other.m_LastTime;
        m_Unknown0x18 = other.m_Unknown0x18;
        m_Unknown0x1A = other.m_Unknown0x1A;
        return *this;
    }

    u8 m_IsEnabled;          // 0x04
    s32 m_Interval;          // 0x08 (milliseconds between the sendings of the bundles)
    u64 m_LastTime;          // 0x10 (the time of the last sending of the bundles)
    u16 m_Unknown0x18;       // 0x18
    u16 m_Unknown0x1A;       // 0x1A
    CriticalSection m_Lock;  // 0x1C
};
ASSERT_OFFSET(StreamBundling, m_Lock, 0x1C);
ASSERT_SIZE(StreamBundling, 0x48);

// switches of the bundling of the packets (read by PRUDPMessageV0::Pack and the history queue;
// the names are ours)
extern bool g_Unknown0x96C92B;
extern bool g_Unknown0x96C92C;
extern bool g_Unknown0x96C92D;
} // namespace nex
} // namespace nn
