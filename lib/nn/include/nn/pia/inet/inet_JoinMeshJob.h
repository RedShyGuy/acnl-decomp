#pragma once

#include "decomp.h"
#include "nn/pia/session/session_JoinMeshJob.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet11JoinMeshJobE @ 0x008CF7F4
// vtable 0x008FFE2C (vptr 0x008FFE34), offset_to_top 0, 10 entries
class JoinMeshJob : public ::nn::pia::session::JoinMeshJob
{
public:
    JoinMeshJob(); // ctor address unknown
    virtual ~JoinMeshJob(); // 0x0042C300 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x004266A4 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void StartupImpl(); // 0x003E26C0 slot 0x18 | slot vf_0x18 of nn::pia::session::JoinMeshJob
    virtual void CleanupImpl(); // 0x003E26BC slot 0x1C | slot vf_0x1C of nn::pia::session::JoinMeshJob
    virtual void SetHostInfoToMonitoringData(const nn::pia::transport::StationConnectionInfo&); // 0x003E2E4C slot 0x20 | slot vf_0x20 of nn::pia::session::JoinMeshJob
    virtual void vf_0x24(); // 0x003E2E68 slot 0x24 | virtual slot, introduced by nn::pia::session::JoinMeshJob
};
} // namespace inet
} // namespace pia
} // namespace nn
