#pragma once

#include "decomp.h"
#include "nn/nex/nex_RefCountedObject.h"
#include "nn/nex/nex_qResult.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex11CallContextE @ 0x008CDFF4
// vtable 0x008FC254 (vptr 0x008FC25C), offset_to_top 0, 5 entries
class CallContext : public ::nn::nex::RefCountedObject
{
public:
    struct State { u32 _unknown; }; // TODO: real type unknown (placeholder)
    virtual ~CallContext(); // 0x003573B0 slot 0x00 | fefates:callgraph
    // 0x00357380 slot 0x04 | fefates:callseq (deleting dtor)
    virtual void vf_0x08(); // 0x0072A0D8 slot 0x08 | virtual slot, introduced by nn::nex::CallContext
    virtual void BeginTransition(nn::nex::CallContext::State, nn::nex::qResult, bool); // 0x00356D18 slot 0x0C | slot vf_0x0C of nn::nex::CallContext
    virtual void ProcessCallCompletion(); // 0x00356D1C slot 0x10 | slot vf_0x10 of nn::nex::CallContext
    void CancelImpl(nn::nex::CallContext::State); // 0x00356988 | fefates:bytes-fuzzy [tier B]
    void SignalFailure(nn::nex::qResult); // 0x00356CB8 | fefates:bytes [tier B]
    void SignalSuccess(nn::nex::qResult); // 0x00356CE8 | fefates:bytes [tier B]
    void RegisterCompletionCallback(void (*)(nn::nex::CallContext*,const nn::nex::UserContext*), const nn::nex::UserContext&, bool); // 0x00356DE4 | fefates:bytes [tier B]
    void Wait(unsigned int); // 0x00356F58 | fefates:bytes-fuzzy [tier B]
    void Reset(); // 0x00357078 | fefates:bytes-fuzzy [tier B]
    CallContext(); // 0x00357284 | mk7dlp:callseq-callee [tier A]
};
} // namespace nex
} // namespace nn
