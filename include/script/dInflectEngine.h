#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"
#include "script/dIWord.h"

namespace script {
// takes the inflection tag (group 14, index 0) of a text into the WordInfo of a word (member name is ours)
// RTTI N6script13InflectEngineE @ 0x008D3864
// vtable 0x0090A158 (vptr 0x0090A160), offset_to_top 0, 8 entries
class InflectEngine : public ::script::MsgEngine
{
public:
    explicit InflectEngine(IWord* word) : m_pWord(word) {}
    virtual ~InflectEngine(); // 0x005E41D8 slot 0x00
    // 0x005E41C8 slot 0x04 (deleting dtor)
    virtual void OnTag(const Tag* tag); // 0x005E4184 slot 0x18

    IWord* m_pWord;  // 0x60
};
ASSERT_SIZE(InflectEngine, 0x64);
} // namespace script
