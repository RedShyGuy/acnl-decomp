#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/transport/transport_ResendingMessageManager.h"

namespace nn {
namespace pia {
namespace transport {
// ctor candidate(s) 0x0045CA08, 0x0045CBEC (unverified)
nn::pia::transport::ResendingMessageManager::ResendingMessageManager()
{
}

// 0x0073676C slot 0x00 | virtual slot, introduced by nn::pia::transport::ResendingMessageManager
void nn::pia::transport::ResendingMessageManager::vf_0x00()
{
}

// 0x0045C6B0 | fefates:bytes [tier B]
void nn::pia::transport::ResendingMessageManager::Initialize(unsigned int)
{
}

// 0x0045C988 | fefates:bytes [tier B]
void nn::pia::transport::ResendingMessageManager::StopResending(unsigned int)
{
}

// 0x0045CA08 | fefates:bytes [tier B]
void nn::pia::transport::ResendingMessageManager::CreateInstance()
{
}

// 0x0045CA80 | fefates:bytes [tier B]
void nn::pia::transport::ResendingMessageManager::SetSendMessage(unsigned int*, const unsigned char*, unsigned int, nn::pia::StationIndex, const nn::pia::common::StationAddress&, nn::pia::transport::ProtocolId, long long)
{
}

// 0x0045CBEC | fefates:bytes [tier B]
void nn::pia::transport::ResendingMessageManager::DestroyInstance()
{
}

// 0x0045CC30 | fefates:bytes [tier B]
void nn::pia::transport::ResendingMessageManager::Cleanup()
{
}

// 0x0045CC44 | fefates:bytes [tier B]
void nn::pia::transport::ResendingMessageManager::Startup(nn::pia::transport::PacketHandler*)
{
}

// 0x0045CC68 | fefates:bytes [tier B]
void nn::pia::transport::ResendingMessageManager::Dispatch()
{
}

// 0x0045CE04 | fefates:bytes [tier B]
void nn::pia::transport::ResendingMessageManager::Finalize()
{
}

// 0x007366C0 | fefates:bytes [tier B]
void nn::pia::transport::ResendingMessageManager::CheckNowResending(unsigned int) const
{
}

// 0x00736750 | fefates:bytes [tier B]
void nn::pia::transport::ResendingMessageManager::ExtractAckIdFromMessage(const unsigned char*, unsigned int) const
{
}

} // namespace transport
} // namespace pia
} // namespace nn
