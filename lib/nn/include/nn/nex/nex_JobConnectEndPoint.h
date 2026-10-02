#pragma once

#include "decomp.h"
#include "nn/nex/nex_StepSequenceJob.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex18JobConnectEndPointE @ 0x008CE70C
// vtable 0x008FD4B0 (vptr 0x008FD4B8), offset_to_top 0, 14 entries
class JobConnectEndPoint : public ::nn::nex::StepSequenceJob
{
public:
    JobConnectEndPoint(); // ctor address unknown
    virtual ~JobConnectEndPoint(); // 0x00389A60 slot 0x00 | fefates:bytes
    // 0x00389A30 slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
    virtual void TestSuspendedJobState(); // 0x00388F8C slot 0x10 | fefates:bytes
    virtual void CancelJob(); // 0x00389644 slot 0x24 | fefates:bytes
    virtual void vf_0x28(); // 0x00388E20 slot 0x28 | virtual slot, introduced by nn::nex::Job
    virtual void CheckExceptions(); // 0x003884C8 slot 0x30 | fefates:bytes
    virtual void Trace(unsigned long long); // 0x00389530 slot 0x34 | fefates:bytes
    void TryConnectViaRouting(); // 0x00388E2C | fefates:bytes [tier B]
    void WaitForURLResolution(); // 0x00388E90 | fefates:bytes [tier B]
    void ReportNATTraversalResult(bool); // 0x0038934C | fefates:bytes-fuzzy [tier B]
    void SelectConnectionTechnique(); // 0x00389450 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
