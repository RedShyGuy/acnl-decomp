#include "nn/cec/CTR/cec_CecControl.h"

namespace nn {
namespace cec {
namespace CTR {
// 0x001407C0 | nintendogs:bytes [tier A]
void nn::cec::CTR::CecControl::StartScanning(bool)
{
}

// 0x0034D358 | fefates:bytes [tier B]
void nn::cec::CTR::CecControl::Initialize()
{
}

// 0x0034D3B8 | nintendogs:callgraph [tier A]
void nn::cec::CTR::CecControl::StopScanning(bool, bool)
{
}

// 0x0034D574 | nintendogs:bytes [tier A]
void nn::cec::CTR::CecControl::Suspend()
{
}

// 0x0034D604 | nintendogs:callseq [tier A]
void nn::cec::CTR::CecControl::Finalize()
{
}

} // namespace CTR
} // namespace cec
} // namespace nn
