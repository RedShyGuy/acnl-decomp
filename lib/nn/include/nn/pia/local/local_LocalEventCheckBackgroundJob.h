#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace os {
class Event;
} // namespace os
namespace pia {
namespace local {
// RTTI N2nn3pia5local28LocalEventCheckBackgroundJobE @ 0x008CFD24
// vtable 0x00901264 (vptr 0x0090126C), offset_to_top 0, 6 entries
//
// Polls the status event of uds in the background; when it is signaled, the connection status is
// read and LocalEventJob processes the change. The member name is ours.
class LocalEventCheckBackgroundJob : public ::nn::pia::common::StepSequenceJob
{
public:
    LocalEventCheckBackgroundJob(); // 0x00423018
    virtual ~LocalEventCheckBackgroundJob(); // 0x00423048 slot 0x00
    // 0x00423038 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x007316C8 slot 0x14

    common::ExecuteResult WatchUpdateEvent(); // 0x00422E98
    void Cleanup(); // 0x00422F94 (name is ours)
    nn::Result Startup(nn::os::Event* pEvent); // 0x00422FA0 (name is ours)

    nn::os::Event* m_pEvent; // 0x40
};
ASSERT_SIZE(LocalEventCheckBackgroundJob, 0x48);
} // namespace local
} // namespace pia
} // namespace nn
