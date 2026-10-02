#include "nn/pia/inet/inet_NatDetecter.h"
#include "nn/pia/inet/inet_NatPropertyDetecter.h"

namespace nn {
namespace pia {
namespace inet {
// 0x003F929C slot 0x00 | fefates:bytes
nn::pia::inet::NatPropertyDetecter::~NatPropertyDetecter()
{
}

// 0x003F9248 slot 0x04 | virtual slot, introduced by nn::pia::inet::NatDetecter
void nn::pia::inet::NatPropertyDetecter::vf_0x04()
{
}

// 0x003F9164 slot 0x08 | fefates:callseq
void nn::pia::inet::NatPropertyDetecter::Startup(nn::pia::common::CallContext*, const nn::pia::common::InetAddress&)
{
}

// 0x003F9100 slot 0x0C | fefates:bytes
void nn::pia::inet::NatPropertyDetecter::Cleanup()
{
}

// 0x003F8F6C slot 0x14 | slot vf_0x14 of nn::pia::inet::NatDetecter
void nn::pia::inet::NatPropertyDetecter::StartSendingMessage()
{
}

// 0x003F8F40 slot 0x18 | fefates:bytes
void nn::pia::inet::NatPropertyDetecter::CheckAllMessage()
{
}

// 0x003F8D5C slot 0x1C | slot vf_0x1C of nn::pia::inet::NatDetecter
void nn::pia::inet::NatPropertyDetecter::HandleResult()
{
}

// 0x003F8D10 slot 0x20 | fefates:bytes
void nn::pia::inet::NatPropertyDetecter::CheckRetry()
{
}

// 0x0072F168 slot 0x24 | slot vf_0x24 of nn::pia::inet::NatDetecter
void nn::pia::inet::NatPropertyDetecter::GetDetectionTimeout() const
{
}

// 0x003F8F84 | fefates:bytes [tier B]
void nn::pia::inet::NatPropertyDetecter::sendNatPropertyDetectionMessage()
{
}

// 0x003F91FC | fefates:bytes [tier B]
nn::pia::inet::NatPropertyDetecter::NatPropertyDetecter()
{
}

} // namespace inet
} // namespace pia
} // namespace nn
