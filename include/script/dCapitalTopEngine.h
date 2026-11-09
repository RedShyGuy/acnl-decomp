#pragma once

#include "decomp.h"
#include "script/dMsgEngineModify.h"

namespace script {
// makes the first character of a text a capital letter (member name is ours)
// RTTI N6script16CapitalTopEngineE @ 0x008D38FC
// vtable 0x0090A288 (vptr 0x0090A290), offset_to_top 0, 7 entries
class CapitalTopEngine : public ::script::MsgEngineModify
{
public:
    CapitalTopEngine() : m_IsContinue(true) {}
    virtual ~CapitalTopEngine(); // 0x005E7174 slot 0x00
    // 0x005E7170 slot 0x04 (deleting dtor)
    virtual bool IsContinue(); // 0x005E7168 slot 0x10
    virtual void OnChar(const CharInfo* info); // 0x005E7138 slot 0x14

    bool m_IsContinue;  // 0x0D
};
ASSERT_SIZE(CapitalTopEngine, 0x10);
} // namespace script
