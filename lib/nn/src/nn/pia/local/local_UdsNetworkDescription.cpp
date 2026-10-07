#include "nn/pia/local/local_UdsNetworkDescription.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_RootObject.h"
#include <cstring>

namespace nn {
namespace pia {
namespace local {
namespace {
// NetworkDescriptionElement::attribute (big endian): no more clients
const u16 ATTRIBUTE_CLIENTS_DISALLOWED = 2;
const u32 BSSID_SIZE = 6;
} // namespace

// 0x0041D5B4 (name is ours)
void nn::pia::local::UdsNetworkDescription::Copy(const nn::pia::local::LocalNetworkDescription* pDescription)
{
    m_Description = static_cast<const UdsNetworkDescription*>(pDescription)->m_Description;
}

// 0x00731508 (name is ours)
u16 nn::pia::local::UdsNetworkDescription::GetChannel() const
{
    return m_Description.GetChannel();
}

// 0x0073151C | fefates:bytes
u8 nn::pia::local::UdsNetworkDescription::GetMaxParticipants() const
{
    return m_Description.GetNodeCountMax();
}

// 0x00731530 | fefates:bytes
u8 nn::pia::local::UdsNetworkDescription::GetCurrentParticipants() const
{
    return m_Description.GetNodeCount();
}

// 0x00731544 | fefates:bytes
u32 nn::pia::local::UdsNetworkDescription::GetLocalCommunicationId() const
{
    return m_Description.GetLocalCommunicationId();
}

// 0x0073155C (name is ours)
void nn::pia::local::UdsNetworkDescription::GetBssid(u8* pBssid) const
{
    if (!common::IsValidPointer(pBssid)) {
        return;
    }
    std::memcpy(pBssid, m_Description.m_Bssid, BSSID_SIZE);
}

// 0x00731584 | fefates:bytes
u8 nn::pia::local::UdsNetworkDescription::GetSubId() const
{
    return m_Description.GetSubId();
}

// 0x00731598 | fefates:bytes
bool nn::pia::local::UdsNetworkDescription::IsOpened() const
{
    if (!m_Description.m_IsInitialized) {
        return false;
    }
    return (__builtin_bswap16(m_Description.m_Element.attribute) & ATTRIBUTE_CLIENTS_DISALLOWED) == 0;
}

} // namespace local
} // namespace pia
} // namespace nn
