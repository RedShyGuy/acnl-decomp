#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace transport {
// RTTI N2nn3pia9transport17ConnectStationJobE @ 0x008D01DC
// vtable 0x00901E74 (vptr 0x00901E7C), offset_to_top 0, 8 entries
class ConnectStationJob : public ::nn::pia::common::StepSequenceJob
{
public:
    virtual ~ConnectStationJob(); // 0x0045414C slot 0x00 | fefates:callgraph
    // 0x0045413C slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x007356D4 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void StartupImpl(nn::pia::common::CallContext*, nn::pia::transport::Station*, const nn::pia::transport::StationConnectionInfo&, bool); // 0x004534A8 slot 0x18 | fefates:bytes
    virtual void CleanupImpl(); // 0x004534A4 slot 0x1C | slot vf_0x1C of nn::pia::transport::ConnectStationJob
    void WaitRequestAck(); // 0x00453604 | fefates:bytes [tier B]
    void ConnectionFailed(); // 0x00453898 | fefates:bytes [tier B]
    void WaitForConnection(); // 0x004538FC | fefates:bytes [tier B]
    void ConnectionSucceeded(); // 0x00453A50 | fefates:bytes [tier B]
    void SendConnectionRequest(); // 0x00453AF4 | fefates:bytes [tier B]
    void StartupRelayConnection(nn::pia::common::CallContext*, nn::pia::transport::Station*, const nn::pia::transport::StationConnectionInfo&, bool, int); // 0x00453CD0 | fefates:bytes [tier B]
    void SendRelayConnectionRequest(); // 0x00453D60 | fefates:bytes [tier B]
    void SendConnectionRequestMessage(); // 0x00453F18 | fefates:bytes [tier B]
    void Cleanup(); // 0x00454024 | fefates:bytes [tier B]
    void Startup(nn::pia::common::CallContext*, nn::pia::transport::Station*, const nn::pia::transport::StationConnectionInfo&, bool, int); // 0x004540D0 | fefates:bytes [tier B]
    ConnectStationJob(); // 0x004540F4 | fefates:bytes [tier B]
};
} // namespace transport
} // namespace pia
} // namespace nn
