#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local21LocalHostMigrationJobE @ 0x008CFBEC
// vtable 0x00900DF8 (vptr 0x00900E00), offset_to_top 0, 6 entries
class LocalHostMigrationJob : public ::nn::pia::common::StepSequenceJob
{
public:
    virtual ~LocalHostMigrationJob(); // 0x0041C324 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x0041C2E0 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x007311E0 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    void ScanNetwork(); // 0x0041B000 | fefates:bytes [tier B]
    void CreateNetwork(); // 0x0041B1EC | fefates:bytes [tier B]
    void WaitForCancel(); // 0x0041B310 | fefates:bytes [tier B]
    void ConnectNetwork(); // 0x0041B360 | fefates:bytes [tier B]
    void WaitAllClientsAck(); // 0x0041B760 | fefates:bytes [tier B]
    void WaitCreateNetwork(); // 0x0041B83C | fefates:bytes [tier B]
    void WaitConnectNetwork(); // 0x0041B9B0 | fefates:bytes [tier B]
    void SearchNewHostNetwork(); // 0x0041BAF8 | fefates:bytes [tier B]
    void WaitDisconnectNetwork(); // 0x0041BCA0 | fefates:bytes [tier B]
    void HostMigrationFailureProcess(); // 0x0041BE44 | fefates:bytes [tier B]
    void Cleanup(); // 0x0041C18C | fefates:bytes [tier B]
    void Startup(nn::pia::common::CallContext*, bool); // 0x0041C1E0 | fefates:bytes [tier B]
    LocalHostMigrationJob(); // 0x0041C284 | fefates:bytes [tier B]
};
} // namespace local
} // namespace pia
} // namespace nn
