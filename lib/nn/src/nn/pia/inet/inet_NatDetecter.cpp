#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/inet/inet_NatDetecter.h"

namespace nn {
namespace pia {
namespace inet {
// 0x003E3A08 slot 0x00 | fefates:bytes
nn::pia::inet::NatDetecter::~NatDetecter()
{
}

// 0x003E39C0 slot 0x04 | virtual slot, introduced by nn::pia::inet::NatDetecter
void nn::pia::inet::NatDetecter::vf_0x04()
{
}

// 0x003E375C slot 0x08 | fefates:callseq
void nn::pia::inet::NatDetecter::Startup(nn::pia::common::CallContext*, const nn::pia::common::InetAddress&)
{
}

// 0x003E3698 slot 0x0C | fefates:bytes
void nn::pia::inet::NatDetecter::Cleanup()
{
}

// 0x004296DC slot 0x10 | fefates:bytes
void nn::pia::inet::NatDetecter::Retry()
{
}

// 0x0011C12F slot 0x14 | slot vf_0x00 of ChangeRentalBase
void nn::pia::inet::NatDetecter::StartSendingMessage()
{
}

// 0x0011C12F slot 0x18 | slot vf_0x00 of ChangeRentalBase
void nn::pia::inet::NatDetecter::CheckAllMessage()
{
}

// 0x0011C12F slot 0x1C | slot vf_0x00 of ChangeRentalBase
void nn::pia::inet::NatDetecter::HandleResult()
{
}

// 0x003E2F28 slot 0x20 | slot vf_0x20 of nn::pia::inet::NatDetecter
void nn::pia::inet::NatDetecter::CheckRetry()
{
}

// 0x0011C12F slot 0x24 | slot vf_0x00 of ChangeRentalBase
void nn::pia::inet::NatDetecter::GetDetectionTimeout() const
{
}

// 0x004288B8 slot 0x28 | slot vf_0x28 of nn::pia::inet::NatDetecter
void nn::pia::inet::NatDetecter::StartDetectionJob()
{
}

// 0x00427438 slot 0x2C | fefates:bytes
void nn::pia::inet::NatDetecter::CancelDetectionJob()
{
}

// 0x003E3078 | fefates:bytes [tier B]
void nn::pia::inet::NatDetecter::CloseSocket()
{
}

// 0x003E3148 | fefates:bytes [tier B]
void nn::pia::inet::NatDetecter::AddSendMessage(const nn::pia::inet::NatDetecter::SendNatCheckMessage&, unsigned short)
{
}

// 0x003E3300 | fefates:bytes [tier B]
void nn::pia::inet::NatDetecter::sendDummyMessage()
{
}

// 0x003E3468 | fefates:bytes [tier B]
void nn::pia::inet::NatDetecter::SendAllQueuedMessage()
{
}

// 0x003E3568 | fefates:bytes [tier B]
void nn::pia::inet::NatDetecter::GetDifferentPortNumber(unsigned short)
{
}

// 0x003E35E4 | fefates:bytes [tier B]
void nn::pia::inet::NatDetecter::InitializeReceiveMessage()
{
}

// 0x003E3618 | fefates:bytes [tier B]
void nn::pia::inet::NatDetecter::TraceReceivedMessageArray(unsigned long long)
{
}

// 0x003E362C | fefates:bytes [tier B]
void nn::pia::inet::NatDetecter::GetPrimaryServerPrimaryPortAddress()
{
}

// 0x003E365C | fefates:bytes [tier B]
void nn::pia::inet::NatDetecter::GetPrimaryServerSecondaryPortAddress()
{
}

// 0x003E38D0 | fefates:bytes [tier B]
nn::pia::inet::NatDetecter::NatDetecter()
{
}

} // namespace inet
} // namespace pia
} // namespace nn
