#pragma once

#include "decomp.h"
#include "nn/pia/common/common_Time.h"
#include "nn/pia/session/session_JoinMeshJob.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet11JoinMeshJobE @ 0x008CF7F4
// vtable 0x008FFE2C (vptr 0x008FFE34), offset_to_top 0, 10 entries
//
// The join of inet: after the connections a bandwidth check with one of the stations, then the
// kickout check (or the connection to the host with host migration).
class JoinMeshJob : public ::nn::pia::session::JoinMeshJob
{
public:
    // (implicit; NexNetworkFactory::CreateJoinMeshJob value-initializes it)
    // (armlink placed it in front of the destructor of the base)
    virtual ~JoinMeshJob(); // 0x0042C300 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x004266A4 slot 0x04 (deleting dtor)
    virtual bool StartupImpl(); // 0x003E26C0 slot 0x18 | slot vf_0x18 of nn::pia::session::JoinMeshJob
    virtual void CleanupImpl(); // 0x003E26BC slot 0x1C | slot vf_0x1C of nn::pia::session::JoinMeshJob
    virtual void SetHostInfoToMonitoringData(const nn::pia::transport::StationConnectionInfo& info); // 0x003E2E4C slot 0x20 | slot vf_0x20 of nn::pia::session::JoinMeshJob
    virtual nn::pia::common::ExecuteResult ProceedToCompleteProcess(); // 0x003E2E68 slot 0x24 | virtual slot, introduced by nn::pia::session::JoinMeshJob

    // (the names are from the step strings)
    common::ExecuteResult WaitBandWidthCheck(); // 0x003E276C
    common::ExecuteResult StartBandWidthCheck(); // 0x003E2960
    common::ExecuteResult WaitCheckKickoutJob(); // 0x003E2B1C
    common::ExecuteResult WaitCheckHostConnection(); // 0x003E2C88

    // the leaving after a cancel (inline; name is ours)
    void SetLeaveStep();

    // (only set by the constructor in the code seen so far)
    common::Time m_UnknownTime0xA0;   // 0xA0
    common::Time m_UnknownTime0xA8;   // 0xA8
};
ASSERT_SIZE(JoinMeshJob, 0xB0);
} // namespace inet
} // namespace pia
} // namespace nn
