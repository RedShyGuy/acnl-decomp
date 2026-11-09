#pragma once

#include "decomp.h"
#include "script/dWordCPtr.h"

namespace script {
// a constant text of a message resource (no size: 0x3FFFFFFF characters)
// RTTI N6script7WordResE @ 0x008D3DDC
// vtable 0x0090B694 (vptr 0x0090B69C), offset_to_top 0, 12 entries
class WordRes : public ::script::WordCPtr
{
public:
    static const u32 SIZE_UNLIMITED = 0x3FFFFFFF;

    DECOMP_EMIT_COPY WordRes() : WordCPtr(NULL, 0) {} // 0x005FDAA8
    DECOMP_EMIT_COPY explicit WordRes(const char16* text) : WordCPtr(text, SIZE_UNLIMITED) {} // 0x005FDA8C
    virtual ~WordRes(); // 0x005FDAC8 slot 0x00
    // 0x005FDAC4 slot 0x04 (deleting dtor)
};
} // namespace script
