#include "nn/pia/common/common_CallContext.h"

namespace nn {
namespace pia {
namespace common {
// 0x004267CC | fefates:bytes [tier B]
void nn::pia::common::CallContext::InitiateCall()
{
}

// 0x004267E4 | fefates:bytes [tier B]
void nn::pia::common::CallContext::SignalCancel()
{
}

// 0x00426814 | fefates:bytes [tier B]
void nn::pia::common::CallContext::SignalFailure(nn::Result)
{
}

// 0x00426860 | fefates:bytes [tier B]
void nn::pia::common::CallContext::SignalSuccess(nn::Result)
{
}

} // namespace common
} // namespace pia
} // namespace nn
