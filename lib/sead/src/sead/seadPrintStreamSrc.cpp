#include "sead/seadPrintStreamSrc.h"

namespace sead {
// a global instance that nothing in the binary refers to (name is ours)
// 0x0097F878
PrintStreamSrc s_PrintStreamSrc;

// constructs s_PrintStreamSrc (GCC initializes it constantly)
// 0x0079A3E4 (static initializer)

// 0x00545B44 slot 0x00
u32 sead::PrintStreamSrc::read(void*, u32)
{
    return 0;
}

// 0x00545B5C slot 0x04
u32 sead::PrintStreamSrc::write(const void*, u32 size)
{
    return size;
}

// 0x00545B4C slot 0x08
u32 sead::PrintStreamSrc::skip(s32)
{
    return 0;
}

// 0x00545B64 slot 0x0C
void sead::PrintStreamSrc::rewind()
{
    // empty (in the original too)
}

// 0x00545B54 slot 0x10
bool sead::PrintStreamSrc::isEOF()
{
    return false;
}

// 0x00562A54 slot 0x14
bool sead::PrintStreamSrc::flush()
{
    return true;
}

} // namespace sead
