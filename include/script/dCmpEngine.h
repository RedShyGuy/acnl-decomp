#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"
#include "script/dIWord.h"

namespace script {
// compares two texts exactly (as far as their terminating 0)
// RTTI N6script9CmpEngineE @ 0x008D3E50
// vtable 0x0090BABC (vptr 0x0090BAC4), offset_to_top 0, 8 entries
class CmpEngine : public ::script::MsgEngine
{
public:
    CmpEngine() {}
    virtual ~CmpEngine(); // 0x0060080C slot 0x00
    // 0x006007FC slot 0x04 (deleting dtor)

    bool Compare(const IWord* a, const IWord* b); // 0x006006E4 (name is ours)
};
ASSERT_SIZE(CmpEngine, 0x60);

// (inline; name is ours)
DECOMP_EMIT_COPY inline bool IsEqualWord(const IWord* a, const IWord* b) // 0x0075C8CC
{
    CmpEngine engine;
    return engine.Compare(a, b);
}
} // namespace script
