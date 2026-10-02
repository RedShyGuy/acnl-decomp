#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet15NatDetectionJobE @ 0x008CF884
// vtable 0x008FFFF0 (vptr 0x008FFFF8), offset_to_top 0, 6 entries
class NatDetectionJob : public ::nn::pia::common::StepSequenceJob
{
public:
    virtual ~NatDetectionJob(); // 0x0042759C slot 0x00 | fefates:callgraph
    // 0x003E79D8 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void CancelCleanup(); // 0x003E72B0 slot 0x10 | fefates:bytes
    void StepPreTest(); // 0x003E70C8 | fefates:bytes [tier B]
    void StepComplete(); // 0x003E71BC | fefates:bytes [tier B]
    void stopReceivingMessage(); // 0x003E72EC | fefates:bytes [tier B]
    void StepEnd(); // 0x003E73F0 | fefates:bytes [tier B]
    void StepSend(); // 0x003E7464 | fefates:bytes [tier B]
    void StepWait(); // 0x003E7754 | fefates:bytes [tier B]
    void StepStart(); // 0x003E785C | fefates:bytes [tier B]
    NatDetectionJob(); // 0x003E7990 | fefates:bytes [tier B]
};
} // namespace inet
} // namespace pia
} // namespace nn
