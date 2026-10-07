#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/common/common_Time.h"

namespace nn {
namespace pia {
namespace common {
class CallContext;
} // namespace common
namespace inet {
class NatTraverser;

// RTTI N2nn3pia4inet26NatServerAddressResolveJobE @ 0x008CFA08
// vtable 0x009005E0 (vptr 0x009005E8), offset_to_top 0, 6 entries
//
// Resolves the addresses of the NAT check servers (NatTraverser::UpdateNatServerAddress) and
// starts the NAT traversal. The member names are ours.
class NatServerAddressResolveJob : public ::nn::pia::common::StepSequenceJob
{
public:
    NatServerAddressResolveJob(); // 0x0040C5FC | fefates:bytes [tier B]
    virtual ~NatServerAddressResolveJob(); // 0x0040C648 slot 0x00 | fefates:callgraph
    // 0x0040C638 slot 0x04 (deleting dtor)
    virtual void CancelCleanup(); // 0x0040C524 slot 0x10 | fefates:bytes

    nn::Result Startup(nn::pia::common::CallContext* pCallContext, nn::pia::inet::NatTraverser* pNatTraverser); // 0x0040C554 | fefates:callgraph [tier C]
    common::ExecuteResult StepResolve(); // 0x0040C388
    common::ExecuteResult StepComplete(); // 0x0040C4A0 | fefates:bytes [tier B]

    NatTraverser* m_pNatTraverser;        // 0x40
    common::CallContext* m_pCallContext;  // 0x44
    common::Time m_Deadline;              // 0x48
    nn::Result m_Result;                  // 0x50
};
ASSERT_OFFSET(NatServerAddressResolveJob, m_Result, 0x50);
ASSERT_SIZE(NatServerAddressResolveJob, 0x58);
} // namespace inet
} // namespace pia
} // namespace nn
