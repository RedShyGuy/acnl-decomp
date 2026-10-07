#pragma once

#include "decomp.h"
#include "nn/pia/session/session_LeaveSessionJob.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local25LocalMatchLeaveSessionJobE @ 0x008CFCB8
// vtable 0x00901128 (vptr 0x00901130), offset_to_top 0, 16 entries
//
// Leaves the session of the local network: after the mesh the matchmake session leaves the
// network; while the local station is still in the network, the leave is retried. The step
// names are from the strings.
class LocalMatchLeaveSessionJob : public ::nn::pia::session::LeaveSessionJob
{
public:
    LocalMatchLeaveSessionJob(); // 0x00420A24
    virtual ~LocalMatchLeaveSessionJob(); // 0x00420A4C slot 0x00
    // 0x00420A3C slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x007316A8 slot 0x14
    virtual void vf_0x1C(); // 0x00420820 slot 0x1C
    virtual void vf_0x20(); // 0x00420968 slot 0x20
    virtual void vf_0x24(); // 0x004207C8 slot 0x24
    virtual void vf_0x28(); // 0x004209BC slot 0x28
    virtual bool vf_0x2C(); // 0x00420960 slot 0x2C

    // the steps
    common::ExecuteResult CompleteProcess(); // 0x00420784
    common::ExecuteResult RetryLeaveCurrentMatchmakeSession(); // 0x00420868
};
} // namespace local
} // namespace pia
} // namespace nn
