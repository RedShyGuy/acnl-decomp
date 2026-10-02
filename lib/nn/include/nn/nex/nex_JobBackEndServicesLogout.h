#pragma once

#include "decomp.h"
#include "nn/nex/nex_StepSequenceJob.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex24JobBackEndServicesLogoutE @ 0x008CED98
// vtable 0x008FE778 (vptr 0x008FE780), offset_to_top 0, 13 entries
class JobBackEndServicesLogout : public ::nn::nex::StepSequenceJob
{
public:
    JobBackEndServicesLogout(); // ctor address unknown
    virtual ~JobBackEndServicesLogout(); // 0x003B2A6C slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x003B2A48 slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
    void CompleteLogout(); // 0x003B24C0 | fefates:bytes [tier B]
    void CompleteLogoutFailure(); // 0x003B2530 | fefates:bytes [tier B]
    void TerminateStreamManager(); // 0x003B2720 | fefates:bytes [tier B]
    void ProcessAuthDisconnectionResult(); // 0x003B2778 | fefates:bytes [tier B]
    void WaitForStreamManagerTermination(); // 0x003B282C | fefates:bytes [tier B]
    void ProcessSecConnDisconnectionResult(); // 0x003B28B8 | fefates:bytes [tier B]
    void DisconnectFromSecureConnectionService(); // 0x003B2954 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
