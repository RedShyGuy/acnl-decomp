#include "nn/nex/nex_qResult.h"
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_ProtocolCallContext.h"

namespace nn {
namespace nex {
// 0x00394A98 slot 0x00 | fefates:callgraph
nn::nex::ProtocolCallContext::~ProtocolCallContext()
{
}

// 0x003947F4 slot 0x0C | fefates:bytes
void nn::nex::ProtocolCallContext::BeginTransition(nn::nex::CallContext::State, nn::nex::qResult, bool)
{
}

// 0x00394768 | fefates:bytes [tier B]
void nn::nex::ProtocolCallContext::SetCredentials(nn::nex::Credentials*)
{
}

// 0x00394A28 | mk7dlp:callseq-callee [tier A]
nn::nex::ProtocolCallContext::ProtocolCallContext()
{
}

} // namespace nex
} // namespace nn
