#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"

namespace script {
// the last character of a text that is no space or new line (member name is ours)
// RTTI N6script19PatchimTargetEngineE @ 0x008D3A1C
// vtable 0x0090A4C4 (vptr 0x0090A4CC), offset_to_top 0, 8 entries
class PatchimTargetEngine : public ::script::MsgEngine
{
public:
    PatchimTargetEngine() : m_LastChar(0) {}
    virtual ~PatchimTargetEngine(); // 0x005E9DC0 slot 0x00
    // 0x005E9DB0 slot 0x04 (deleting dtor)
    virtual void OnChar(const CharInfo* info); // 0x005E9D84 slot 0x14

    char16 m_LastChar;  // 0x60
};
ASSERT_SIZE(PatchimTargetEngine, 0x64);
} // namespace script
