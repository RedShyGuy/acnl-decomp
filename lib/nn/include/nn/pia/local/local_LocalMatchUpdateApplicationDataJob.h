#pragma once

#include "decomp.h"
#include "nn/pia/session/session_UpdateApplicationDataJob.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local34LocalMatchUpdateApplicationDataJobE @ 0x008CFDC0
// vtable 0x00901468 (vptr 0x00901470), offset_to_top 0, 8 entries
//
// Changes the application data of the local network (the beacon) through the matchmake
// session; the change is done at once. The step name is from the strings.
class LocalMatchUpdateApplicationDataJob : public ::nn::pia::session::UpdateApplicationDataJob
{
public:
    LocalMatchUpdateApplicationDataJob(); // 0x004257C8
    virtual ~LocalMatchUpdateApplicationDataJob(); // 0x004257F0 slot 0x00
    // 0x004257E0 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x007317FC slot 0x14
    // (armlink placed it in front of the one of the base)
    virtual void Cleanup(); // 0x00442C80 slot 0x18
    virtual nn::Result vf_0x1C(const void* pData, u32 size); // 0x00425654 slot 0x1C

    // the step
    common::ExecuteResult SignalProcess(); // 0x004256D8
};
} // namespace local
} // namespace pia
} // namespace nn
