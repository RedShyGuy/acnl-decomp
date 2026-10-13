#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_Time.h"
#include <cstdarg>

namespace nn {
namespace nex {
// The parts of a line of a Log (most of them write nothing in this build). The names of the
// members are ours; the functions keep their slot names.
// RTTI N2nn3nex12OutputFormatE @ 0x008CE144
// vtable 0x008FC5E8 (vptr 0x008FC5F0), offset_to_top 0, 9 entries
class OutputFormat : public ::nn::nex::RootObject
{
public:
    // (inline)
    OutputFormat() : m_Unknown0x04(0), m_Unknown0x08(0)
    {
        for (u32 i = 0; i < sizeof(m_Flags); i++) {
            m_Flags[i] = false;
        }
        Time time = SystemClock::GetTime();
        m_Unknown0x18 = 0;
        m_StartTime = static_cast<u32>(time.m_Value);
    }
    virtual ~OutputFormat(); // 0x0035D648 slot 0x00
    // 0x0035D630 slot 0x04 (deleting dtor)
    // starts the line
    virtual void vf_0x08(wchar_t* pBuffer, u32 size); // 0x0035D5DC slot 0x08
    // appends "("
    virtual void vf_0x0C(wchar_t* pBuffer, u32 size); // 0x0035D5E8 slot 0x0C
    virtual void vf_0x10(wchar_t* pBuffer, u32 size); // 0x0035D5D4 slot 0x10
    virtual void vf_0x14(wchar_t* pBuffer, u32 size); // 0x0035D5D8 slot 0x14
    virtual void vf_0x18(wchar_t* pBuffer, u32 size); // 0x0035D628 slot 0x18
    // the message (nothing in this build)
    virtual u32 vf_0x1C(wchar_t* pBuffer, u32 size, const wchar_t* pFormat, va_list args); // 0x0035D5CC slot 0x1C
    virtual void vf_0x20(wchar_t* pBuffer, u32 size); // 0x0035D62C slot 0x20

    u32 m_Unknown0x04;  // 0x04
    u32 m_Unknown0x08;  // 0x08
    bool m_Flags[10];   // 0x0C
    u32 m_Unknown0x18;  // 0x18
    u32 m_StartTime;    // 0x1C, the time when it was made (low word)
};
ASSERT_SIZE(OutputFormat, 0x20);
} // namespace nex
} // namespace nn
