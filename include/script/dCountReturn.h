#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"
#include "script/dIWord.h"

namespace script {
// counts the new lines of a text (member names are ours)
// RTTI N6script11CountReturnE @ 0x008D3774
// vtable 0x00909CF8 (vptr 0x00909D00), offset_to_top 0, 8 entries
class CountReturn : public ::script::MsgEngine
{
public:
    CountReturn() : m_Count(0) {}
    virtual ~CountReturn(); // 0x005D9198 slot 0x00
    // 0x005D9188 slot 0x04 (deleting dtor)
    virtual void OnChar(const CharInfo* info); // 0x005D916C slot 0x14

    s32 m_Count;  // 0x60
};
ASSERT_SIZE(CountReturn, 0x64);
} // namespace script
