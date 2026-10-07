#include "nn/pia/local/local_LocalAroundNetworkSearchManager_LocalAroundNetworkSearchCommandAckMessage.h"

namespace nn {
namespace pia {
namespace local {
// 0x00424380
bool nn::pia::local::LocalAroundNetworkSearchManager::LocalAroundNetworkSearchCommandAckMessage::ParseMessageHeader()
{
    if (!LocalMessage::ParseMessageHeader()) {
        return false;
    }
    m_Value = *reinterpret_cast<u32*>(m_pBuffer + HEADER_SIZE);
    return true;
}

// 0x004243A8
void nn::pia::local::LocalAroundNetworkSearchManager::LocalAroundNetworkSearchCommandAckMessage::UpdateMessageHeader()
{
    LocalMessage::UpdateMessageHeader();
    *reinterpret_cast<u32*>(m_pBuffer + HEADER_SIZE) = m_Value;
}

// 0x004243C8
// 0x004243C4 (deleting dtor)
nn::pia::local::LocalAroundNetworkSearchManager::LocalAroundNetworkSearchCommandAckMessage::~LocalAroundNetworkSearchCommandAckMessage()
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn
