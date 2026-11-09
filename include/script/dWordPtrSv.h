#pragma once

#include "decomp.h"
#include "script/dWordPtr.h"

namespace script {
// a word on a buffer of the save data: copies into it drop the tags
// RTTI N6script9WordPtrSvE @ 0x008D3EA0
// vtable 0x0090BC00 (vptr 0x0090BC08), offset_to_top 0, 12 entries
class WordPtrSv : public ::script::WordPtr
{
public:
    DECOMP_EMIT_COPY WordPtrSv(char16* buffer, u32 size) : WordPtr(buffer, size) {} // 0x00605360
    virtual ~WordPtrSv(); // 0x003082F4 slot 0x00 | slot vf_0x00 of script::WordPtr
    // 0x00605378 slot 0x04 (deleting dtor)
    virtual bool IsSv() const; // 0x0075E140 slot 0x2C
};
} // namespace script
