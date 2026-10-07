#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local19LocalReceiveFromJobE @ 0x008CFBA4
// vtable 0x00900CB8 (vptr 0x00900CC0), offset_to_top 0, 7 entries
//
// Receives the system messages in the background while LocalInputStream does not read (the data
// of the stations is read by the input stream).
class LocalReceiveFromJob : public ::nn::pia::common::StepSequenceJob
{
public:
    LocalReceiveFromJob(); // 0x0041A0B8
    virtual ~LocalReceiveFromJob(); // 0x0041A0E0 slot 0x00
    // 0x0041A0D0 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x007311C8 slot 0x14
    virtual nn::Result Startup(); // 0x0041A048 slot 0x18 (name is ours)

    common::ExecuteResult ReceiveFrom(); // 0x00419F20
    common::ExecuteResult CheckLocalInputStream(); // 0x00419FB8 | fefates:bytes [tier B]
};
ASSERT_SIZE(LocalReceiveFromJob, 0x40);
} // namespace local
} // namespace pia
} // namespace nn
