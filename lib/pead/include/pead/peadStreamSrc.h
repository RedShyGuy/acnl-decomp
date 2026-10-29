#pragma once

#include "decomp.h"

namespace pead {
// RTTI N4pead9StreamSrcE @ 0x008D132C
//
// Where a stream reads its bytes from and writes them to (an interface without a destructor;
// the slot names are ours).
class StreamSrc
{
public:
    // the number of bytes read / written / skipped
    virtual u32 read(void* dst, u32 size) = 0;
    virtual u32 write(const void* src, u32 size) = 0;
    virtual u32 skip(s32 size) = 0;
    // back to the start
    virtual void rewind() = 0;
    virtual bool isEOF() = 0;
    virtual bool flush() = 0;
};
} // namespace pead
