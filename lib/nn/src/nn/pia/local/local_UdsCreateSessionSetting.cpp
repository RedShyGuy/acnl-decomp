#include "nn/pia/local/local_UdsCreateSessionSetting.h"
#include <cstring>

namespace nn {
namespace pia {
namespace local {
// 0x0041F70C
void nn::pia::local::UdsCreateSessionSetting::SetCreateNetworkSetting(const nn::pia::local::LocalCreateNetworkSetting& setting)
{
    std::memcpy(m_CreateNetworkSetting.m_ApplicationData, setting.m_ApplicationData, setting.m_ApplicationDataSize);
    m_CreateNetworkSetting.m_ApplicationDataSize = setting.m_ApplicationDataSize;
    m_CreateNetworkSetting.m_Channel = setting.m_Channel;
    m_CreateNetworkSetting.m_SubId = setting.m_SubId;
    m_CreateNetworkSetting.m_LocalCommunicationId = setting.m_LocalCommunicationId;
    m_CreateNetworkSetting.m_NodeCountMax = setting.m_NodeCountMax;
    std::memcpy(m_CreateNetworkSetting.m_Passphrase, setting.m_Passphrase, setting.m_PassphraseSize);
    m_CreateNetworkSetting.m_PassphraseSize = setting.m_PassphraseSize;
}

// 0x0041F774
nn::pia::local::UdsCreateSessionSetting::UdsCreateSessionSetting() : m_CreateNetworkSetting()
{
}

// 0x004202FC
// 0x0041F7D0 (deleting dtor)
nn::pia::local::UdsCreateSessionSetting::~UdsCreateSessionSetting()
{
    // empty (in the original too)
}

// 0x0073168C
nn::pia::local::LocalCreateNetworkSetting* nn::pia::local::UdsCreateSessionSetting::GetCreateNetworkSetting()
{
    return &m_CreateNetworkSetting;
}

// 0x00731694
void nn::pia::local::UdsCreateSessionSetting::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn
