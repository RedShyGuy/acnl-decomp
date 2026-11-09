#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"
#include "script/dIWord.h"

namespace script {
// finds out whether a text has tags (member names are ours)
// RTTI N6script16ContainTagEngineE @ 0x008D3914
// vtable 0x0090A2AC (vptr 0x0090A2B4), offset_to_top 0, 8 entries
class ContainTagEngine : public ::script::MsgEngine
{
public:
    ContainTagEngine() : m_HasTag(0), m_HasTagEnd(0) {}
    virtual ~ContainTagEngine(); // 0x005E71BC slot 0x00
    // 0x005E71AC slot 0x04 (deleting dtor)
    virtual bool IsContinue(); // 0x005E7190 slot 0x10
    virtual void OnTag(const Tag* tag); // 0x005E7184 slot 0x18
    virtual void OnTagEnd(); // 0x005E7178 slot 0x1C

    bool HasTag() const { return m_HasTag || m_HasTagEnd; }

    u8 m_HasTag;      // 0x60
    u8 m_HasTagEnd;   // 0x61
};
ASSERT_SIZE(ContainTagEngine, 0x64);

inline bool IWord::HasTag() const
{
    ContainTagEngine engine;
    const char16* text = GetWord();
    if (text != NULL) {
        engine.Run(text);
    }
    return engine.HasTag();
}
} // namespace script
