#pragma once

#include "decomp.h"
#include "sead/seadStreamSrc.h"

namespace sead {
// RTTI N4sead14PrintStreamSrcE @ 0x008D1744
// vtable 0x009058EC (vptr 0x009058F4), offset_to_top 0, 6 entries
//
// A stream source that accepts all writes (the debug output is not in the binary) and has nothing to
// read.
class PrintStreamSrc : public ::sead::StreamSrc
{
public:
    virtual u32 read(void* dst, u32 size); // 0x00545B44 slot 0x00
    virtual u32 write(const void* src, u32 size); // 0x00545B5C slot 0x04
    virtual u32 skip(s32 size); // 0x00545B4C slot 0x08
    virtual void rewind(); // 0x00545B64 slot 0x0C
    virtual bool isEOF(); // 0x00545B54 slot 0x10
    virtual bool flush(); // 0x00562A54 slot 0x14
};
} // namespace sead
