#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/transport/transport_ProtocolManager.h"

namespace nn {
namespace pia {
namespace transport {
// 0x007352EC slot 0x00 | virtual slot, introduced by nn::pia::transport::ProtocolManager
void nn::pia::transport::ProtocolManager::vf_0x00()
{
}

// 0x00450858 | fefates:bytes [tier B]
void nn::pia::transport::ProtocolManager::AllocProtocol(unsigned int)
{
}

// 0x004508A4 | fefates:bytes [tier B]
void nn::pia::transport::ProtocolManager::SearchProtocol(nn::pia::transport::ProtocolId, unsigned short)
{
}

// 0x00450904 | fefates:bytes [tier B]
void nn::pia::transport::ProtocolManager::DestroyProtocol(unsigned int)
{
}

// 0x004509BC | fefates:bytes [tier B]
void nn::pia::transport::ProtocolManager::CleanupProtocols()
{
}

// 0x00450A14 | fefates:bytes [tier B]
void nn::pia::transport::ProtocolManager::StartupProtocols(nn::pia::StationIndex)
{
}

// 0x00450E4C | fefates:bytes [tier B]
void nn::pia::transport::ProtocolManager::UpdateProtocolEvent(const nn::pia::transport::ProtocolEvent&)
{
}

// 0x00450ED8 | fefates:bytes [tier B]
void nn::pia::transport::ProtocolManager::Cleanup()
{
}

// 0x00450F30 | fefates:bytes [tier B]
void nn::pia::transport::ProtocolManager::Startup(nn::pia::transport::PacketHandler*)
{
}

// 0x00450FBC | fefates:bytes [tier B]
void nn::pia::transport::ProtocolManager::Dispatch()
{
}

// 0x00451034 | fefates:bytes [tier B]
void nn::pia::transport::ProtocolManager::Finalize()
{
}

// 0x00451090 | fefates:bytes [tier B]
nn::pia::transport::ProtocolManager::ProtocolManager()
{
}

// 0x004510CC | fefates:bytes [tier B]
nn::pia::transport::ProtocolManager::~ProtocolManager()
{
}

} // namespace transport
} // namespace pia
} // namespace nn
