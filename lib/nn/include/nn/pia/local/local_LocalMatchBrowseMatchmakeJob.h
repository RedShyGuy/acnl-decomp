#pragma once

#include "decomp.h"
#include "nn/pia/session/session_BrowseMatchmakeJob.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local28LocalMatchBrowseMatchmakeJobE @ 0x008CFD30
// vtable 0x00901284 (vptr 0x0090128C), offset_to_top 0, 8 entries
class LocalMatchBrowseMatchmakeJob : public ::nn::pia::session::BrowseMatchmakeJob
{
public:
    LocalMatchBrowseMatchmakeJob(); // ctor address unknown
    virtual ~LocalMatchBrowseMatchmakeJob(); // 0x004232D4 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x004232B0 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x007316CC slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void vf_0x18(); // 0x0042328C slot 0x18 | virtual slot, introduced by nn::pia::session::BrowseMatchmakeJob
    virtual void vf_0x1C(); // 0x0042304C slot 0x1C | virtual slot, introduced by nn::pia::session::BrowseMatchmakeJob
};
} // namespace local
} // namespace pia
} // namespace nn
