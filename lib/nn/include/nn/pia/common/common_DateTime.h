#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace common {
// RTTI N2nn3pia6common8DateTimeE @ 0x008CFF0C
// vtable 0x00901614 (vptr 0x0090161C), offset_to_top 0, 2 entries
//
// A date and a time of day. Only the destructor is in common; the layout is from
// inet::NexSessionInfo, the member names are ours.
class DateTime : public ::nn::pia::common::RootObject
{
public:
    // (inline in inet::NexSessionInfo)
    DateTime() { Clear(); }
    // (inline in inet::NexMatchmakeSession; name is ours)
    DateTime(u16 year, u8 month, u8 day, u8 hour, u8 minute, u8 second)
        : m_Year(year), m_Month(month), m_Day(day), m_Hour(hour), m_Minute(minute), m_Second(second), m_Unknown0xB(1)
    {
    }
    virtual ~DateTime(); // 0x00429434 slot 0x00
    // 0x00429430 slot 0x04 (deleting dtor)

    // (inline; name is ours)
    void Clear()
    {
        m_Year = 0;
        m_Month = 0;
        m_Day = 0;
        m_Hour = 0;
        m_Minute = 0;
        m_Second = 0;
        m_Unknown0xB = 0;
    }
    // (inline; name is ours)
    void Set(const DateTime& rhs)
    {
        m_Year = rhs.m_Year;
        m_Month = rhs.m_Month;
        m_Day = rhs.m_Day;
        m_Hour = rhs.m_Hour;
        m_Minute = rhs.m_Minute;
        m_Second = rhs.m_Second;
        m_Unknown0xB = rhs.m_Unknown0xB;
    }

    u16 m_Year;        // 0x4
    u8 m_Month;        // 0x6
    u8 m_Day;          // 0x7
    u8 m_Hour;         // 0x8
    u8 m_Minute;       // 0x9
    u8 m_Second;       // 0xA
    u8 m_Unknown0xB;   // 0xB
};
ASSERT_SIZE(DateTime, 0xC);
} // namespace common
} // namespace pia
} // namespace nn
