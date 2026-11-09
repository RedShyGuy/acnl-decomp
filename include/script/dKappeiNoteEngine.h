#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"

namespace script {
// counts the characters of a text without spaces and new lines (member names are ours)
// RTTI N6script16KappeiNoteEngineE @ 0x008D3944
// vtable 0x0090A2FC (vptr 0x0090A304), offset_to_top 0, 8 entries
class KappeiNoteEngine : public ::script::MsgEngine
{
public:
    KappeiNoteEngine() : m_Count(0), m_IsStop(0) {}
    virtual ~KappeiNoteEngine(); // 0x005E7344 slot 0x00
    // 0x005E7334 slot 0x04 (deleting dtor)
    virtual bool IsContinue(); // 0x005E7320 slot 0x10
    virtual void OnChar(const CharInfo* info); // 0x005E72AC slot 0x14
    virtual void OnTag(const Tag* tag); // 0x005E72E4 slot 0x18

    s32 m_Count;   // 0x60
    u8 m_IsStop;   // 0x64
};
ASSERT_SIZE(KappeiNoteEngine, 0x68);
} // namespace script
