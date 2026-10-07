#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local35LocalSendSystemMessageBackgroundJobE @ 0x008CFDCC
// vtable 0x00901490 (vptr 0x00901498), offset_to_top 0, 7 entries
//
// Sends the queued system messages in the background.
class LocalSendSystemMessageBackgroundJob : public ::nn::pia::common::StepSequenceJob
{
public:
    LocalSendSystemMessageBackgroundJob(); // 0x0042589C
    virtual ~LocalSendSystemMessageBackgroundJob(); // 0x004258C4 slot 0x00
    // 0x004258B4 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x00731800 slot 0x14
    virtual nn::Result Startup(); // 0x00425820 slot 0x18 (name is ours)

    common::ExecuteResult SendSystemMessage(); // 0x004257F4
};
ASSERT_SIZE(LocalSendSystemMessageBackgroundJob, 0x40);
} // namespace local
} // namespace pia
} // namespace nn
