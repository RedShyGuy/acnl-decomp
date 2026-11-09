#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"

namespace script {
// whether a text begins with a vowel or h (for the elision of the articles; member names are ours)
// RTTI N6script13ElisionEngineE @ 0x008D384C
// vtable 0x0090A130 (vptr 0x0090A138), offset_to_top 0, 8 entries
class ElisionEngine : public ::script::MsgEngine
{
public:
    ElisionEngine() : m_IsElision(0), m_IsDone(0) {}
    virtual ~ElisionEngine(); // 0x005E4174 slot 0x00
    // 0x005E4164 slot 0x04 (deleting dtor)
    virtual bool IsContinue(); // 0x005E4150 slot 0x10
    virtual void OnChar(const CharInfo* info); // 0x005E40EC slot 0x14

    u8 m_IsElision;  // 0x60
    u8 m_IsDone;     // 0x61 after the first character
};
ASSERT_SIZE(ElisionEngine, 0x64);
} // namespace script
