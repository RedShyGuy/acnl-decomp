#include "nn/nex/nex_qResult.h"
#include "nn/nex/nex_RefCountedObject.h"
#include "nn/nex/nex_CallContext.h"

namespace nn {
namespace nex {
// 0x003573B0 slot 0x00 | fefates:callgraph
nn::nex::CallContext::~CallContext()
{
}

// 0x0072A0D8 slot 0x08 | virtual slot, introduced by nn::nex::CallContext
void nn::nex::CallContext::vf_0x08()
{
}

// 0x00356D18 slot 0x0C | slot vf_0x0C of nn::nex::CallContext
void nn::nex::CallContext::BeginTransition(nn::nex::CallContext::State, nn::nex::qResult, bool)
{
}

// 0x00356D1C slot 0x10 | slot vf_0x10 of nn::nex::CallContext
void nn::nex::CallContext::ProcessCallCompletion()
{
}

// 0x00356988 | fefates:bytes-fuzzy [tier B]
void nn::nex::CallContext::CancelImpl(nn::nex::CallContext::State)
{
}

// 0x00356CB8 | fefates:bytes [tier B]
void nn::nex::CallContext::SignalFailure(nn::nex::qResult)
{
}

// 0x00356CE8 | fefates:bytes [tier B]
void nn::nex::CallContext::SignalSuccess(nn::nex::qResult)
{
}

// 0x00356DE4 | fefates:bytes [tier B]
void nn::nex::CallContext::RegisterCompletionCallback(void (*)(nn::nex::CallContext*,const nn::nex::UserContext*), const nn::nex::UserContext&, bool)
{
}

// 0x00356F58 | fefates:bytes-fuzzy [tier B]
void nn::nex::CallContext::Wait(unsigned int)
{
}

// 0x00357078 | fefates:bytes-fuzzy [tier B]
void nn::nex::CallContext::Reset()
{
}

// 0x00357284 | mk7dlp:callseq-callee [tier A]
nn::nex::CallContext::CallContext()
{
}

} // namespace nex
} // namespace nn
