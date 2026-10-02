#include "nn/pia/inet/inet_NatDetecter.h"
#include "nn/pia/inet/inet_NatPortDetecter.h"

namespace nn {
namespace pia {
namespace inet {
// 0x003E7E74 slot 0x00 | fefates:bytes
nn::pia::inet::NatPortDetecter::~NatPortDetecter()
{
}

// 0x003E7E48 slot 0x04 | virtual slot, introduced by nn::pia::inet::NatDetecter
void nn::pia::inet::NatPortDetecter::vf_0x04()
{
}

// 0x003E3730 slot 0x08 | fefates:bytes
void nn::pia::inet::NatPortDetecter::Startup(nn::pia::common::CallContext*, const nn::pia::common::InetAddress&)
{
}

// 0x003E3694 slot 0x0C | slot vf_0x0C of nn::pia::inet::NatDetecter
void nn::pia::inet::NatPortDetecter::Cleanup()
{
}

// 0x003E7C60 slot 0x14 | fefates:bytes
void nn::pia::inet::NatPortDetecter::StartSendingMessage()
{
}

// 0x003E7C50 slot 0x18 | slot vf_0x18 of nn::pia::inet::NatDetecter
void nn::pia::inet::NatPortDetecter::CheckAllMessage()
{
}

// 0x003E7A7C slot 0x1C | slot vf_0x1C of nn::pia::inet::NatDetecter
void nn::pia::inet::NatPortDetecter::HandleResult()
{
}

// 0x003E79E8 slot 0x20 | fefates:bytes
void nn::pia::inet::NatPortDetecter::CheckRetry()
{
}

// 0x0072F0F8 slot 0x24 | slot vf_0x24 of nn::pia::inet::NatDetecter
void nn::pia::inet::NatPortDetecter::GetDetectionTimeout() const
{
}

// 0x003E7D68 slot 0x30 | fefates:callseq
void nn::pia::inet::NatPortDetecter::vf_0x30()
{
}

// 0x003E7E10 | fefates:bytes [tier B]
nn::pia::inet::NatPortDetecter::NatPortDetecter()
{
}

} // namespace inet
} // namespace pia
} // namespace nn
