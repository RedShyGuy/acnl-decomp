#pragma once

#include "decomp.h"
#include "nn/pia/session/session_DestroySessionJob.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local27LocalMatchDestroySessionJobE @ 0x008CFD18
// vtable 0x00901234 (vptr 0x0090123C), offset_to_top 0, 10 entries
//
// Destroys the session of the local network: after the mesh (DestroySessionJob::MeshCleanup)
// the matchmake session destroys the network. The step names are from the strings.
class LocalMatchDestroySessionJob : public ::nn::pia::session::DestroySessionJob
{
public:
    LocalMatchDestroySessionJob(); // 0x00422E6C
    virtual ~LocalMatchDestroySessionJob(); // 0x00422E94 slot 0x00
    // 0x00422E84 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x007316C4 slot 0x14
    virtual void Cleanup(); // 0x00422E68 slot 0x18
    virtual nn::Result vf_0x1C(); // 0x00422C44 slot 0x1C
    virtual void vf_0x20(); // 0x00422E20 slot 0x20
    virtual void vf_0x24(); // 0x00422DC4 slot 0x24

    // the steps
    common::ExecuteResult DestroyLocalNetwork(); // 0x00422C4C
    common::ExecuteResult WaitDestroyLocalNetwork(); // 0x00422D10
};
} // namespace local
} // namespace pia
} // namespace nn
