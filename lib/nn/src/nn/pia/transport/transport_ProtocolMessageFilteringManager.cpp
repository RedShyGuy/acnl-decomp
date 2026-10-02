#include "nn/pia/transport/transport_ProtocolMessageFilteringManager.h"

namespace nn {
namespace pia {
namespace transport {
// 0x0045F1C0 | fefates:bytes [tier B]
void nn::pia::transport::ProtocolMessageFilteringManager::CreateInstance()
{
}

// 0x0045F294 | fefates:bytes [tier B]
void nn::pia::transport::ProtocolMessageFilteringManager::AddNoFilteringProtocolType(unsigned short)
{
}

// 0x0045F2BC | fefates:bytes [tier B]
void nn::pia::transport::ProtocolMessageFilteringManager::RemoveNoFilteringProtocolType(unsigned short)
{
}

// 0x00736D10 | fefates:bytes [tier B]
void nn::pia::transport::ProtocolMessageFilteringManager::IsFilteringEnabled(unsigned short) const
{
}

} // namespace transport
} // namespace pia
} // namespace nn
