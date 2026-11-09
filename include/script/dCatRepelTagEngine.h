#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"
#include "script/dIWord.h"

namespace script {
// appends a text without its tags (with a CopyRepelTagEngine)
// RTTI N6script17CatRepelTagEngineE @ 0x008D395C
// vtable 0x0090A324 (vptr 0x0090A32C), offset_to_top 0, 8 entries
class CatRepelTagEngine : public ::script::MsgEngine
{
public:
    CatRepelTagEngine() {}
    virtual ~CatRepelTagEngine(); // 0x005E7F3C slot 0x00
    // 0x005E7F2C slot 0x04 (deleting dtor)

    void Cat(IWord* dst, const IWord* src); // 0x005E7D90 (name is ours)
};
ASSERT_SIZE(CatRepelTagEngine, 0x60);
} // namespace script
