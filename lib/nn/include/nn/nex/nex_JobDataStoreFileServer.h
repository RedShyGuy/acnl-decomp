#pragma once

#include "decomp.h"
#include "nn/nex/nex_HttpEventListener.h"
#include "nn/nex/nex_NonCopyable.h"
#include "nn/nex/nex_StepSequenceJob.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex22JobDataStoreFileServerE @ 0x008CEBBC
// vtable 0x008FE024 (vptr 0x008FE02C), offset_to_top 0, 18 entries
// vtable 0x008FE074 (vptr 0x008FE07C), offset_to_top -104, 6 entries
class JobDataStoreFileServer : public ::nn::nex::StepSequenceJob, public ::nn::nex::NonCopyable, public ::nn::nex::HttpEventListener
{
public:
    JobDataStoreFileServer(); // ctor candidate(s) 0x003B5FA8 (unverified)
    virtual ~JobDataStoreFileServer(); // 0x0039CEB0 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x0039CE40 slot 0x04 | fefates:bytes (deleting dtor)
    virtual void vf_0x34(); // 0x0039C4D4 slot 0x34 | fefates:callseq
    virtual void vf_0x38(); // 0x0039C6B8 slot 0x38 | virtual slot, introduced by nn::nex::JobDataStoreFileServer
    virtual void vf_0x3C(); // 0x0039C88C slot 0x3C | virtual slot, introduced by nn::nex::JobDataStoreFileServer
    virtual void vf_0x40(); // 0x0039CC00 slot 0x40 | virtual slot, introduced by nn::nex::JobDataStoreFileServer
    virtual void vf_0x44(); // 0x0039C79C slot 0x44 | virtual slot, introduced by nn::nex::JobDataStoreFileServer
    void CompleteJob(const nn::nex::qResult&); // 0x0039C638 | fefates:bytes [tier B]
    void StepWaitingForHttp(); // 0x0039C924 | fefates:bytes [tier B]
    void StepHttp(); // 0x0039CCBC | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
