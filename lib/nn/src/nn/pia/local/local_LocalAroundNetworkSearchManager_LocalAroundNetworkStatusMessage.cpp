#include "nn/pia/local/local_LocalAroundNetworkSearchManager_LocalAroundNetworkStatusMessage.h"

namespace nn {
namespace pia {
namespace local {
// 0x00423CA4
bool nn::pia::local::LocalAroundNetworkSearchManager::LocalAroundNetworkStatusMessage::ParseMessageHeader()
{
    if (!LocalMessage::ParseMessageHeader()) {
        return false;
    }
    m_Value = *reinterpret_cast<u32*>(m_pBuffer + HEADER_SIZE);
    return true;
}

// 0x00423CCC
void nn::pia::local::LocalAroundNetworkSearchManager::LocalAroundNetworkStatusMessage::UpdateMessageHeader()
{
    LocalMessage::UpdateMessageHeader();
    *reinterpret_cast<u32*>(m_pBuffer + HEADER_SIZE) = m_Value;
}

// 0x00423CEC
// 0x00423CE8 (deleting dtor)
nn::pia::local::LocalAroundNetworkSearchManager::LocalAroundNetworkStatusMessage::~LocalAroundNetworkStatusMessage()
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn
