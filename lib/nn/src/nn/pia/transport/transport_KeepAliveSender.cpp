#include "nn/pia/transport/transport_KeepAliveSender.h"

namespace nn {
namespace pia {
namespace transport {
// 0x00450054 | fefates:bytes [tier B]
void nn::pia::transport::KeepAliveSender::SetInterval(int)
{
}

// 0x0045006C | fefates:bytes [tier B]
void nn::pia::transport::KeepAliveSender::Update(unsigned int, const nn::pia::common::Time&)
{
}

// 0x00450160 | fefates:bytes [tier B]
nn::pia::transport::KeepAliveSender::KeepAliveSender()
{
}

} // namespace transport
} // namespace pia
} // namespace nn
