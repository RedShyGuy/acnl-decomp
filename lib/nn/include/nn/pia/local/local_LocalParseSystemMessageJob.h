#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local26LocalParseSystemMessageJobE @ 0x008CFCE8
// vtable 0x009011D0 (vptr 0x009011D8), offset_to_top 0, 7 entries
//
// Processes the received system messages in the foreground.
class LocalParseSystemMessageJob : public ::nn::pia::common::StepSequenceJob
{
public:
    LocalParseSystemMessageJob(); // 0x004213C8
    virtual ~LocalParseSystemMessageJob(); // 0x004213F0 slot 0x00
    // 0x004213E0 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x007316B0 slot 0x14
    virtual nn::Result Startup(); // 0x00421378 slot 0x18 (name is ours)

    common::ExecuteResult ParseSystemMessage(); // 0x0042134C
};
ASSERT_SIZE(LocalParseSystemMessageJob, 0x40);
} // namespace local
} // namespace pia
} // namespace nn
