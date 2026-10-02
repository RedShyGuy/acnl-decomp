#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/inet/inet_NatServerAddressResolveJob.h"

namespace nn {
namespace pia {
namespace inet {
// 0x0040C648 slot 0x00 | fefates:callgraph
nn::pia::inet::NatServerAddressResolveJob::~NatServerAddressResolveJob()
{
}

// 0x0040C524 slot 0x10 | fefates:bytes
void nn::pia::inet::NatServerAddressResolveJob::CancelCleanup()
{
}

// 0x0040C4A0 | fefates:bytes [tier B]
void nn::pia::inet::NatServerAddressResolveJob::StepComplete()
{
}

// 0x0040C5FC | fefates:bytes [tier B]
nn::pia::inet::NatServerAddressResolveJob::NatServerAddressResolveJob()
{
}

} // namespace inet
} // namespace pia
} // namespace nn
