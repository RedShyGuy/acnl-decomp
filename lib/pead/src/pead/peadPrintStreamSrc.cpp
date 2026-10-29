#include "pead/peadPrintStreamSrc.h"

namespace pead {
// a global instance that nothing in the binary refers to (name is ours)
// 0x00982F60
PrintStreamSrc s_PrintStreamSrc;

// constructs s_PrintStreamSrc (GCC initializes it constantly)
// 0x0079A3FC (static initializer)

// 0x005387D4 slot 0x00
u32 pead::PrintStreamSrc::read(void*, u32)
{
    return 0;
}

// 0x005387EC slot 0x04
u32 pead::PrintStreamSrc::write(const void*, u32 size)
{
    return size;
}

// 0x005387DC slot 0x08
u32 pead::PrintStreamSrc::skip(s32)
{
    return 0;
}

// 0x005387F4 slot 0x0C
void pead::PrintStreamSrc::rewind()
{
    // empty (in the original too)
}

// 0x005387E4 slot 0x10
bool pead::PrintStreamSrc::isEOF()
{
    return false;
}

// 0x0053DC24 slot 0x14
bool pead::PrintStreamSrc::flush()
{
    return true;
}

} // namespace pead
