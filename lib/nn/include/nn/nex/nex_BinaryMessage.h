#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_BinaryMessage.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex13BinaryMessageE @ 0x008CE20C
// vtable 0x008FC7A4 (vptr 0x008FC7AC), offset_to_top 0, 9 entries
class BinaryMessage : public ::nn::nex::_DDL_BinaryMessage
{
public:
    // (inline)
    BinaryMessage() {}
    virtual ~BinaryMessage(); // 0x00361C40 slot 0x00
    // 0x00361C00 slot 0x04 (deleting dtor)
    // the part of UserMessage, then the data as hexadecimal bytes in lines of 64
    virtual void Trace() const; // 0x00361B44 slot 0x20

    // the bytes in a line of the trace
    static const u32 TRACE_LINE_LENGTH = 64;
};
ASSERT_SIZE(BinaryMessage, 0x68);
} // namespace nex
} // namespace nn
