#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_Time.h"

namespace nn {
namespace nex {
// Waits in a loop for a condition (sleeping between the tests) until a timeout. The names of the
// members and of the modes are ours.
// RTTI N2nn3nex8SpinTestE @ 0x008CF714
// vtable 0x008FFCB8 (vptr 0x008FFCC0), offset_to_top 0, 2 entries
class SpinTest : public ::nn::nex::RootObject
{
public:
    enum Mode
    {
        // SpinOnce goes on after the timeout
        MODE_CONTINUE = 1,
        // SpinOnce stops at the timeout
        MODE_STOP = 2
    };

    SpinTest(unsigned int sleepMilliseconds, unsigned int timeout); // 0x003D5EAC | fefates:bytes [tier B]
    virtual ~SpinTest(); // 0x003D5F48 slot 0x00
    // 0x003D5F24 slot 0x04 (deleting dtor)

    // sleeps once; false if the timeout passed and the mode stops then (the file, the line and the
    // condition are for the messages)
    bool SpinOnce(wchar_t* pFile, unsigned int line, wchar_t* pCondition); // 0x003D5E20 | fefates:bytes-fuzzy [tier B]
    // (inline; name is ours) the time left until the timeout (-1: none)
    u32 GetRemainingTime()
    {
        if (m_Timeout == static_cast<u32>(-1)) {
            return static_cast<u32>(-1);
        }
        m_Elapsed = static_cast<u32>(SystemClock::GetTime().m_Value) - static_cast<u32>(m_StartTime.m_Value);
        return m_Timeout > m_Elapsed ? m_Timeout - m_Elapsed : 0;
    }

    u16 m_Mode;              // 0x04
    u32 m_Timeout;           // 0x08
    Time m_StartTime;        // 0x10
    u32 m_Elapsed;           // 0x18
    u32 m_SleepMilliseconds; // 0x1C
    u32 m_Unknown0x20;       // 0x20
    u32 m_Unknown0x24;       // 0x24
    u32 m_Unknown0x28;       // 0x28
    u32 m_Unknown0x2C;       // 0x2C
    bool m_IsTimedOut;       // 0x30
};
ASSERT_SIZE(SpinTest, 0x38);
} // namespace nex
} // namespace nn
