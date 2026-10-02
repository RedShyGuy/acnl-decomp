#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/inet/inet_NexFacade.h"

namespace nn {
namespace pia {
namespace inet {
// ctor candidate(s) 0x0041307C (unverified)
nn::pia::inet::NexFacade::NexFacade()
{
}

// 0x00413624 slot 0x00 | fefates:bytes
void nn::pia::inet::NexFacade::Bind(nn::pia::inet::NexFacade::LoginInfo*)
{
}

// 0x00413694 slot 0x04 | slot vf_0x04 of nn::pia::inet::NexFacade
void nn::pia::inet::NexFacade::Unbind()
{
}

// 0x00413748 slot 0x08 | slot vf_0x08 of nn::pia::inet::NexFacade
void nn::pia::inet::NexFacade::Startup(nn::nex::MatchMakingClient*)
{
}

// 0x004136A4 slot 0x0C | fefates:bytes
void nn::pia::inet::NexFacade::Cleanup()
{
}

// 0x004131B0 slot 0x10 | virtual slot, introduced by nn::pia::inet::NexFacade
void nn::pia::inet::NexFacade::vf_0x10()
{
}

// 0x00413268 slot 0x14 | slot vf_0x14 of nn::pia::inet::NexFacade
void nn::pia::inet::NexFacade::StartNatSessionAsync()
{
}

// 0x004133B0 slot 0x18 | fefates:bytes
void nn::pia::inet::NexFacade::IsCompletedStartNatSession()
{
}

// 0x00413360 slot 0x1C | fefates:bytes
void nn::pia::inet::NexFacade::GetStartNatSessionResult()
{
}

// 0x00413388 slot 0x20 | virtual slot, introduced by nn::pia::inet::NexFacade
void nn::pia::inet::NexFacade::vf_0x20()
{
}

// 0x004268B8 slot 0x24 | slot vf_0x24 of nn::pia::inet::NexFacade
void nn::pia::inet::NexFacade::StopNatSession()
{
}

// 0x0072FAD0 slot 0x28 | virtual slot, introduced by nn::pia::inet::NexFacade
void nn::pia::inet::NexFacade::vf_0x28()
{
}

// 0x0041395C slot 0x2C | virtual slot, introduced by nn::pia::inet::NexFacade
void nn::pia::inet::NexFacade::vf_0x2C()
{
}

// 0x0041393C slot 0x30 | slot vf_0x30 of nn::pia::inet::NexFacade
nn::pia::inet::NexFacade::~NexFacade()
{
}

// 0x00412FB4 slot 0x34 | fefates:bytes
void nn::pia::inet::NexFacade::initialize()
{
}

// 0x004138B0 slot 0x38 | fefates:bytes
void nn::pia::inet::NexFacade::finalize()
{
}

// 0x004131BC slot 0x3C | fefates:bytes
void nn::pia::inet::NexFacade::startNatSessionCore(nn::pia::common::CallContext*)
{
}

// 0x00357848 | fefates:bytes [tier B]
void nn::pia::inet::NexFacade::ConvertInetAddressToNexInetAddress(const nn::pia::common::InetAddress&, nn::nex::InetAddress*)
{
}

// 0x00413058 | fefates:bytes [tier B]
void nn::pia::inet::NexFacade::IsEdmMapping(const nn::pia::transport::StationLocation&)
{
}

// 0x0041307C | fefates:bytes [tier B]
void nn::pia::inet::NexFacade::CreateInstance()
{
}

// 0x0041314C | fefates:bytes [tier B]
void nn::pia::inet::NexFacade::CreateProtocols()
{
}

// 0x00413168 | fefates:bytes [tier B]
void nn::pia::inet::NexFacade::DestroyInstance()
{
}

// 0x004132B8 | fefates:bytes [tier B]
void nn::pia::inet::NexFacade::CompleteStartNatSession(unsigned short)
{
}

// 0x004133D4 | fefates:bytes [tier B]
void nn::pia::inet::NexFacade::ConvertNexStationUrlToStationLocation(const nn::nex::StationURL&, nn::pia::transport::StationLocation*)
{
}

// 0x004134A0 | fefates:bytes [tier B]
void nn::pia::inet::NexFacade::RegisterNexNotificationEventHandler4Pia(nn::nex::NotificationEventHandler*)
{
}

// 0x00413554 | fefates:bytes [tier B]
void nn::pia::inet::NexFacade::UnregisterNexNotificationEventHandler4Pia(nn::nex::NotificationEventHandler*)
{
}

// 0x0041387C | fefates:bytes [tier B]
void nn::pia::inet::NexFacade::IsGlobal(const nn::pia::transport::StationLocation&)
{
}

} // namespace inet
} // namespace pia
} // namespace nn
