#pragma once

#include "decomp.h"
#include "nn/nex/nex_StepSequenceJob.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex27JobBackEndServicesTerminateE @ 0x008CF03C
// vtable 0x008FEF6C (vptr 0x008FEF74), offset_to_top 0, 13 entries
class JobBackEndServicesTerminate : public ::nn::nex::StepSequenceJob
{
public:
    JobBackEndServicesTerminate(); // ctor address unknown
    virtual ~JobBackEndServicesTerminate(); // 0x003BD298 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x003BD274 slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
    void StepLogoutComplete(); // 0x003BCFEC | fefates:bytes [tier B]
    void StepWaitOnLoginCancel(); // 0x003BD060 | fefates:bytes [tier B]
    void StepWaitOnPendingJobs(); // 0x003BD0CC | fefates:bytes [tier B]
    void StepInitLogoutIfRequired(); // 0x003BD190 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
