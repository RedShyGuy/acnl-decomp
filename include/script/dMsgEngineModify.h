#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"

namespace script {
// Walks through a writable text like MsgEngine and remembers the current character, so that the slots can change it
// (no stack of inserted texts; slot and member names are ours). It has no vtable of its own in the binary.
// RTTI N6script15MsgEngineModifyE @ 0x008D38F4
class MsgEngineModify
{
public:
    MsgEngineModify() : m_pCur(NULL), m_pChar(NULL), m_IsModified(false) {}
    virtual ~MsgEngineModify() {}
    virtual void Begin(); // 0x005E712C slot 0x08
    virtual void End(); // 0x005E7124 slot 0x0C
    virtual bool IsContinue(); // 0x005E7130 slot 0x10
    virtual void OnChar(const CharInfo* info) = 0; // slot 0x14
    virtual void OnTag(const Tag* tag); // 0x005E7128 slot 0x18

    // (inline)
    void SetText(char16* text)
    {
        m_pChar = s_pNoStop;
        m_pCur = text;
        Begin();
    }
    // runs until the text reaches *stop or IsContinue stops it
    void Run(char16* const* stop); // 0x005E701C

    char16* m_pCur;     // 0x04
    char16* m_pChar;    // 0x08 the current character
    bool m_IsModified;  // 0x0C

    // the stop that is never reached
    static char16* s_pNoStop;
};
} // namespace script
