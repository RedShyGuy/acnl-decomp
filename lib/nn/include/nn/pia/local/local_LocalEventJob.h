#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local13LocalEventJobE @ 0x008CFAE0
// vtable 0x00900894 (vptr 0x0090089C), offset_to_top 0, 6 entries
//
// Processes the update events of the network in the foreground once LocalEventCheckBackgroundJob
// saw one (LocalNetworkManager::m_IsEventSignaled).
class LocalEventJob : public ::nn::pia::common::StepSequenceJob
{
public:
    LocalEventJob(); // 0x00416700
    virtual ~LocalEventJob(); // 0x00416728 slot 0x00
    // 0x00416718 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x00730080 slot 0x14

    nn::Result Startup(); // 0x004166C0 (name is ours)
    common::ExecuteResult WatchUpdateEvent(); // 0x00416644
};
ASSERT_SIZE(LocalEventJob, 0x40);
} // namespace local
} // namespace pia
} // namespace nn
