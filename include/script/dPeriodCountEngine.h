#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"

namespace script {
// counts the periods at the start of a text (member names are ours)
// RTTI N6script17PeriodCountEngineE @ 0x008D3980
// vtable 0x0090A390 (vptr 0x0090A398), offset_to_top 0, 8 entries
class PeriodCountEngine : public ::script::MsgEngine
{
public:
    PeriodCountEngine() : m_Count(0), m_IsStop(0) {}
    virtual ~PeriodCountEngine(); // 0x005E87FC slot 0x00
    // 0x005E87EC slot 0x04 (deleting dtor)
    virtual bool IsContinue(); // 0x005E87D8 slot 0x10
    virtual void OnChar(const CharInfo* info); // 0x005E879C slot 0x14
    virtual void OnTag(const Tag* tag); // 0x005E87BC slot 0x18

    // (inline) the periods at the start of the text
    s32 Count(const char16* const* begin, const char16* const* end)
    {
        m_Count = 0;
        m_IsStop = 0;
        SetText(begin, end);
        SetText2(&s_pNoStop);
        return m_Count;
    }

    s32 m_Count;   // 0x60
    u8 m_IsStop;   // 0x64
};
ASSERT_SIZE(PeriodCountEngine, 0x68);
} // namespace script
