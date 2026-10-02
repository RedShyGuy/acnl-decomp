#pragma once

#include "decomp.h"
#include "nn/nex/nex_RefCountedObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex3JobE @ 0x008CF458
// vtable 0x008FF85C (vptr 0x008FF864), offset_to_top 0, 12 entries
class Job : public ::nn::nex::RefCountedObject
{
public:
    struct JobType { u32 _unknown; }; // TODO: real type unknown (placeholder)
    struct State { u32 _unknown; }; // TODO: real type unknown (placeholder)
    Job(); // ctor candidate(s) 0x003CC778 (unverified)
    virtual ~Job(); // 0x003CC80C slot 0x00 | fefates:callgraph
    // 0x003CC7E0 slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
    virtual void DecoratedExecute(); // 0x003CC644 slot 0x08 | slot vf_0x08 of nn::nex::Job
    virtual void Execute(); // 0x0011C12F slot 0x0C | slot vf_0x00 of ChangeRentalBase
    virtual void TestSuspendedJobState(); // 0x003CC704 slot 0x10 | slot vf_0x10 of nn::nex::Job
    virtual void AddActivity(const wchar_t*); // 0x003CC588 slot 0x14 | slot vf_0x14 of nn::nex::Job
    virtual void vf_0x18(); // 0x0072DF28 slot 0x18 | virtual slot, introduced by nn::nex::Job
    virtual void SetDefaultPostExecutionState(); // 0x003CC710 slot 0x1C | slot vf_0x1C of nn::nex::Job
    virtual void SkipWaitDelayAtTermination(); // 0x003CC708 slot 0x20 | slot vf_0x20 of nn::nex::Job
    virtual void CancelJob(); // 0x003CC774 slot 0x24 | slot vf_0x24 of nn::nex::Job
    virtual void vf_0x28(); // 0x003CC700 slot 0x28 | virtual slot, introduced by nn::nex::Job
    virtual void vf_0x2C(); // 0x003CC638 slot 0x2C | virtual slot, introduced by nn::nex::Job
    void SetToWaiting(int); // 0x003CC594 | fefates:bytes [tier B]
    void SetToComplete(); // 0x003CC630 | mk7dlp:callseq-callee [tier A]
    void SetState(nn::nex::Job::State); // 0x003CC750 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
