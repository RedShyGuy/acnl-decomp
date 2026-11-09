#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"
#include "script/dIWord.h"

namespace script {
// counts the characters of a text (member names are ours)
// RTTI N6script9CountCodeE @ 0x008D3E68
// vtable 0x0090BAE4 (vptr 0x0090BAEC), offset_to_top 0, 8 entries
class CountCode : public ::script::MsgEngine
{
public:
    CountCode(bool unknown, bool isCountReturn) : m_Unknown0x60(unknown), m_IsCountReturn(isCountReturn), m_Count(0) {}
    virtual ~CountCode(); // 0x00600854 slot 0x00
    // 0x00600844 slot 0x04 (deleting dtor)
    virtual void OnChar(const CharInfo* info); // 0x0060081C slot 0x14
    virtual void OnTag(const Tag* tag); // 0x00600840 slot 0x18

    u8 m_Unknown0x60;     // 0x60
    u8 m_IsCountReturn;   // 0x61 the new lines count too
    s32 m_Count;          // 0x64
};
ASSERT_SIZE(CountCode, 0x68);
} // namespace script
