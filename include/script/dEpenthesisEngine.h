#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"

namespace script {
// whether a text begins with an a or e vowel (member names are ours)
// RTTI N6script16EpenthesisEngineE @ 0x008D392C
// vtable 0x0090A2D4 (vptr 0x0090A2DC), offset_to_top 0, 8 entries
class EpenthesisEngine : public ::script::MsgEngine
{
public:
    explicit EpenthesisEngine(bool isE) : m_IsE(isE), m_IsMatch(0), m_IsDone(0) {}
    virtual ~EpenthesisEngine(); // 0x005E729C slot 0x00
    // 0x005E728C slot 0x04 (deleting dtor)
    virtual bool IsContinue(); // 0x005E7278 slot 0x10
    virtual void OnChar(const CharInfo* info); // 0x005E71CC slot 0x14

    u8 m_IsE;       // 0x60 the e vowels (else the a vowels)
    u8 m_IsMatch;   // 0x61
    u8 m_IsDone;    // 0x62 after the first character
};
ASSERT_SIZE(EpenthesisEngine, 0x64);
} // namespace script
