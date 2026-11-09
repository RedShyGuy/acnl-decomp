#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"
#include "script/dIWord.h"

namespace script {
// appends a text behind the text of a word (with a CopyEngine)
// RTTI N6script9CatEngineE @ 0x008D3E2C
// vtable 0x0090BA6C (vptr 0x0090BA74), offset_to_top 0, 8 entries
class CatEngine : public ::script::MsgEngine
{
public:
    CatEngine() {}
    virtual ~CatEngine(); // 0x005FEB74 slot 0x00
    // 0x005FEB64 slot 0x04 (deleting dtor)

    void Cat(IWord* dst, const IWord* src); // 0x005FEA74 (name is ours)
};
ASSERT_SIZE(CatEngine, 0x60);
} // namespace script
