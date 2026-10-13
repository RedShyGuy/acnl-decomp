#pragma once

#include "decomp.h"
#include "nn/nex/nex_Log.h"

namespace nn {
namespace nex {
// The log of the traces (by flags). The member names are ours.
// RTTI N2nn3nex8TraceLogE @ 0x008CF720
// vtable 0x008FFCC8 (vptr 0x008FFCD0), offset_to_top 0, 5 entries
class TraceLog : public ::nn::nex::Log
{
public:
    TraceLog() : m_Unknown0x10(1), m_Unknown0x14(0) {} // (inline)
    virtual ~TraceLog(); // 0x003D5FD4 slot 0x00
    // 0x003D5F58 slot 0x04 (deleting dtor)

    u32 m_Unknown0x10; // 0x10
    u32 m_Unknown0x14; // 0x14
};
ASSERT_SIZE(TraceLog, 0x18);
} // namespace nex
} // namespace nn
