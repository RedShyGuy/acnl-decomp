#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"
#include "script/dIWord.h"

namespace script {
// copies one line of a text into a word (member names are ours)
// RTTI N6script14StrRemakeColorE @ 0x008D38DC
// vtable 0x0090A260 (vptr 0x0090A268), offset_to_top 0, 8 entries
class StrRemakeColor : public ::script::MsgEngine
{
public:
    StrRemakeColor() : m_pDst(NULL), m_pDstLast(NULL), m_Line(0), m_CurLine(0), m_IsStop(0), m_IsFull(0) {}
    virtual ~StrRemakeColor(); // 0x005E7004 slot 0x00
    // 0x005E6FF4 slot 0x04 (deleting dtor)
    virtual void End(); // 0x005E6ED4 slot 0x0C
    virtual bool IsContinue(); // 0x005E6FE0 slot 0x10
    virtual void OnChar(const CharInfo* info); // 0x005E6E7C slot 0x14
    virtual void OnTag(const Tag* tag); // 0x005E6EEC slot 0x18

    // whether the line exists and fits into dst
    bool CopyLine(IWord* dst, const IWord* src, s32 line); // 0x005E6F24 (name is ours)

    char16* m_pDst;       // 0x60
    char16* m_pDstLast;   // 0x64
    s32 m_Line;           // 0x68
    s32 m_CurLine;        // 0x6C
    u8 m_IsStop;          // 0x70
    u8 m_IsFull;          // 0x71
};
ASSERT_SIZE(StrRemakeColor, 0x74);
} // namespace script
