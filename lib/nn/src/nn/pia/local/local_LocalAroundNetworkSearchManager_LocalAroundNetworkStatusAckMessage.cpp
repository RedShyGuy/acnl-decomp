#include "nn/pia/local/local_LocalAroundNetworkSearchManager_LocalAroundNetworkStatusAckMessage.h"

namespace nn {
namespace pia {
namespace local {
// 0x00423E40
bool nn::pia::local::LocalAroundNetworkSearchManager::LocalAroundNetworkStatusAckMessage::ParseMessageHeader()
{
    if (!LocalMessage::ParseMessageHeader()) {
        return false;
    }
    m_Value = *reinterpret_cast<u32*>(m_pBuffer + HEADER_SIZE);
    return true;
}

// 0x00423E68
void nn::pia::local::LocalAroundNetworkSearchManager::LocalAroundNetworkStatusAckMessage::UpdateMessageHeader()
{
    LocalMessage::UpdateMessageHeader();
    *reinterpret_cast<u32*>(m_pBuffer + HEADER_SIZE) = m_Value;
}

// 0x00423E88
// 0x00423E84 (deleting dtor)
nn::pia::local::LocalAroundNetworkSearchManager::LocalAroundNetworkStatusAckMessage::~LocalAroundNetworkStatusAckMessage()
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn
