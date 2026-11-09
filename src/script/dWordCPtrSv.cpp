#include "script/dWordCPtrSv.h"

namespace script {
// 0x005D914C (name is ours)
WordCPtrSv::WordCPtrSv(const char16* word, u32 size) : WordCPtr(word, size)
{
}

// 0x005D9168 slot 0x00
// 0x005D9164 (deleting dtor)
WordCPtrSv::~WordCPtrSv()
{
    // empty (in the original too)
}

// 0x0075B9FC slot 0x2C
bool WordCPtrSv::IsSv() const
{
    return true;
}
} // namespace script
