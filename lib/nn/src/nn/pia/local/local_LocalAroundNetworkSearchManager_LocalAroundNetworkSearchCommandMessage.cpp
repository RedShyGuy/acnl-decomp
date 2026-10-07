#include "nn/pia/local/local_LocalAroundNetworkSearchManager_LocalAroundNetworkSearchCommandMessage.h"

namespace nn {
namespace pia {
namespace local {
// 0x004240E4
bool nn::pia::local::LocalAroundNetworkSearchManager::LocalAroundNetworkSearchCommandMessage::ParseMessageHeader()
{
    if (!LocalMessage::ParseMessageHeader()) {
        return false;
    }
    m_Value = *reinterpret_cast<u32*>(m_pBuffer + HEADER_SIZE);
    return true;
}

// 0x0042410C
void nn::pia::local::LocalAroundNetworkSearchManager::LocalAroundNetworkSearchCommandMessage::UpdateMessageHeader()
{
    LocalMessage::UpdateMessageHeader();
    *reinterpret_cast<u32*>(m_pBuffer + HEADER_SIZE) = m_Value;
}

// 0x0042412C
// 0x00424128 (deleting dtor)
nn::pia::local::LocalAroundNetworkSearchManager::LocalAroundNetworkSearchCommandMessage::~LocalAroundNetworkSearchCommandMessage()
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn
