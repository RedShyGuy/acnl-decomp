#pragma once

#include "decomp.h"
#include "script/dWordCPtr.h"

namespace script {
// a constant text of the save data
// RTTI N6script10WordCPtrSvE @ 0x008D3768
// vtable 0x00909CC0 (vptr 0x00909CC8), offset_to_top 0, 12 entries
class WordCPtrSv : public ::script::WordCPtr
{
public:
    WordCPtrSv(const char16* word, u32 size); // 0x005D914C (name is ours)
    virtual ~WordCPtrSv(); // 0x005D9168 slot 0x00
    // 0x005D9164 slot 0x04 (deleting dtor)
    virtual bool IsSv() const; // 0x0075B9FC slot 0x2C
};
} // namespace script
