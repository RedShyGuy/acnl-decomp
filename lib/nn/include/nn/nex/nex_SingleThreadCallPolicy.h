#pragma once

#include "decomp.h"
#include "nn/nex/nex_CallPolicy.h"

namespace nn {
namespace nex {
// Remembers the thread that is inside the calls (nested calls are counted). The names of the
// members and of BeginCallImpl are ours.
// RTTI N2nn3nex22SingleThreadCallPolicyE @ 0x008CEC68
// vtable 0x008FE1C0 (vptr 0x008FE1C8), offset_to_top 0, 4 entries
class SingleThreadCallPolicy : public ::nn::nex::CallPolicy
{
public:
    SingleThreadCallPolicy(); // 0x0039E264 | mk7dlp:bytes [tier A]
    virtual ~SingleThreadCallPolicy(); // 0x0039E2A4 slot 0x00
    // 0x0039E28C slot 0x04 (deleting dtor)
    virtual void BeginCallImpl(); // 0x0039E21C slot 0x08
    virtual void EndCallImpl(); // 0x0039E200 slot 0x0C | mk7dlp:bytes [tier A]

    u32 m_CallDepth;    // 0x04
    u32 m_ThreadId;     // 0x08 (of the thread inside the calls)
    u32 m_Unknown0x0C;  // 0x0C
    u32 m_Unknown0x10;  // 0x10
};
ASSERT_SIZE(SingleThreadCallPolicy, 0x14);
} // namespace nex
} // namespace nn
