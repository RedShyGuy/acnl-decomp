#pragma once

#include "decomp.h"
#include "pead/peadStreamSrc.h"

namespace pead {
// RTTI N4pead14PrintStreamSrcE @ 0x008D1184
// vtable 0x00904A58 (vptr 0x00904A60), offset_to_top 0, 6 entries
//
// A stream source that accepts all writes (the debug output is not in the binary) and has nothing to
// read.
class PrintStreamSrc : public ::pead::StreamSrc
{
public:
    virtual u32 read(void* dst, u32 size); // 0x005387D4 slot 0x00
    virtual u32 write(const void* src, u32 size); // 0x005387EC slot 0x04
    virtual u32 skip(s32 size); // 0x005387DC slot 0x08
    virtual void rewind(); // 0x005387F4 slot 0x0C
    virtual bool isEOF(); // 0x005387E4 slot 0x10
    virtual bool flush(); // 0x0053DC24 slot 0x14
};
} // namespace pead
