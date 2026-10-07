#pragma once

#include "decomp.h"
#include "nn/pia/session/session_KickoutManageJob.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local21LocalKickoutManageJobE @ 0x008CFBF8
// vtable 0x00900E18 (vptr 0x00900E20), offset_to_top 0, 8 entries
//
// The kickout of the local network: the host ejects the station from the network.
class LocalKickoutManageJob : public ::nn::pia::session::KickoutManageJob
{
public:
    LocalKickoutManageJob(); // 0x0041C388
    // (a nop that falls into the destructor of session::KickoutManageJob)
    virtual ~LocalKickoutManageJob(); // 0x004389B0 slot 0x00
    // 0x0041C3A0 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x007311E4 slot 0x14
    virtual void StartupImpl(); // 0x0041C360 slot 0x18
    virtual void OnKickout(const nn::pia::common::StationAddress& address); // 0x0041C378 slot 0x1C
};
} // namespace local
} // namespace pia
} // namespace nn
