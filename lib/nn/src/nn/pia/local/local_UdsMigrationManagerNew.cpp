#include "nn/pia/local/local_UdsMigrationManagerNew.h"
#include "nn/pia/common/common_NewArray.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/local/local_LocalNetwork.h"
#include "nn/pia/local/local_LocalNetworkManager.h"
#include "nn/pia/local/local_UdsNetworkDescription.h"
#include "nn/uds/CTR/CTR_Api.h"
#include "pead/peadHeapMgr.h"
#include <cstring>

namespace nn {
namespace pia {
namespace local {
namespace {
const u32 PASSPHRASE_SIZE_MIN = 8;
const u32 PASSPHRASE_SIZE_MAX = 255;
const u32 BSSID_SIZE = 6;

inline UdsNetworkDescription* GetNextNetworkDescription(LocalNetworkDescription* pDescription)
{
    return static_cast<UdsNetworkDescription*>(pDescription);
}
} // namespace

// 0x0041E4F4 | fefates:bytes
void nn::pia::local::UdsMigrationManagerNew::SetupParams()
{
    m_SystemDataSize = SYSTEM_DATA_SIZE;
    m_ApplicationDataSizeMax = APPLICATION_DATA_SIZE_MAX;
    m_PassphraseSizeMin = PASSPHRASE_SIZE_MIN;
    m_PassphraseSizeMax = PASSPHRASE_SIZE_MAX;
    m_InvalidNodeKey = 0;
}

// 0x0041E51C
bool nn::pia::local::UdsMigrationManagerNew::IsNextNetwork(const nn::pia::local::LocalNetworkDescription* pDescription)
{
    common::CriticalSection& criticalSection = LocalNetwork::s_pInstance->m_pNetworkManager->m_SystemDataCriticalSection;
    criticalSection.Lock();
    const LocalBeaconSystemData* pSystemData = LocalNetwork::s_pInstance->m_pNetworkManager->GetSystemData(pDescription);
    if (pSystemData == nullptr || pSystemData->m_Unknown0x0 != LocalNetwork::s_pInstance->m_pNetworkManager->m_Unknown0x123C) {
        criticalSection.Unlock();
        return false;
    }
    // the network keeps the BSSID of the network of the old host
    const BeaconSystemData* pData = reinterpret_cast<const BeaconSystemData*>(pSystemData);
    const u8* pBssid = GetNextNetworkDescription(m_pNextNetworkDescription)->m_Description.m_Bssid;
    for (u32 i = 0; i < BSSID_SIZE; i++) {
        if (pBssid[i] != pData->m_Bssid[i]) {
            criticalSection.Unlock();
            return false;
        }
    }
    criticalSection.Unlock();
    return true;
}

// 0x0041E5D0 | fefates:bytes
nn::Result nn::pia::local::UdsMigrationManagerNew::SetNetworkInfo(const nn::pia::local::LocalConnectNetworkSetting& setting)
{
    const nn::uds::CTR::NetworkDescription& description = static_cast<const UdsNetworkDescription*>(setting.m_pDescription)->m_Description;
    u32 size = description.GetApplicationData(m_pBeacon, m_SystemDataSize + m_ApplicationDataSizeMax);
    if (m_SystemDataSize > size) {
        return common::RESULT_NO_DATA;
    }
    m_ApplicationDataSize = size - m_SystemDataSize;
    GetNextNetworkDescription(m_pNextNetworkDescription)->m_Description = description;
    m_PassphraseSize = setting.m_PassphraseSize;
    std::memmove(m_pPassphrase, setting.m_Passphrase, setting.m_PassphraseSize);
    return nn::Result();
}

// 0x0041E64C | fefates:callseq
void nn::pia::local::UdsMigrationManagerNew::SetSystemDataToBeacon(void* pBeacon)
{
    BeaconSystemData data;
    data.m_Unknown0x0 = LocalNetwork::s_pInstance->m_pNetworkManager->m_Unknown0x1238;
    data.m_SessionId = LocalNetwork::s_pInstance->m_pNetworkManager->m_SessionId;
    data.m_ApplicationVersion = LocalNetwork::s_pInstance->m_pNetworkManager->GetApplicationVersion();
    std::memcpy(data.m_Bssid, GetNextNetworkDescription(m_pNextNetworkDescription)->m_Description.m_Bssid, BSSID_SIZE);
    std::memcpy(pBeacon, &data, sizeof(data));
}

// 0x0041E6F4 | fefates:bytes
nn::pia::local::LocalCreateNetworkSetting* nn::pia::local::UdsMigrationManagerNew::GetCreateNetworkSetting()
{
    const nn::uds::CTR::NetworkDescription& description = GetNextNetworkDescription(m_pNextNetworkDescription)->m_Description;
    LocalCreateNetworkSetting* pSetting = m_pCreateNetworkSetting;
    pSetting->m_SubId = description.GetSubId();
    pSetting->m_NodeCountMax = description.GetNodeCountMax();
    pSetting->m_LocalCommunicationId = description.GetLocalCommunicationId();
    std::memcpy(pSetting->m_Passphrase, m_pPassphrase, m_PassphraseSize);
    pSetting->m_PassphraseSize = m_PassphraseSize;
    pSetting->m_Channel = static_cast<u8>(description.GetChannel());
    std::memcpy(pSetting->m_ApplicationData, m_pBeacon + m_SystemDataSize, m_ApplicationDataSize);
    pSetting->m_ApplicationDataSize = m_ApplicationDataSize;
    return pSetting;
}

// 0x0041E790 | fefates:bytes
nn::pia::local::LocalConnectNetworkSetting* nn::pia::local::UdsMigrationManagerNew::GetConnectNetworkSetting(nn::pia::local::LocalNetworkDescription* pDescription)
{
    LocalConnectNetworkSetting* pSetting = m_pConnectNetworkSetting;
    pSetting->m_pDescription = pDescription;
    std::memcpy(pSetting->m_Passphrase, m_pPassphrase, m_PassphraseSize);
    pSetting->m_PassphraseSize = static_cast<u8>(m_PassphraseSize);
    return pSetting;
}

// 0x0041E7C0 | fefates:bytes [tier B]
nn::pia::local::UdsMigrationManagerNew::UdsMigrationManagerNew()
{
    m_pNextNetworkDescription = common::NewObject<UdsNetworkDescription>();
    m_pCreateNetworkSetting = common::NewObject<LocalCreateNetworkSetting>();
    m_pConnectNetworkSetting = common::NewObject<LocalConnectNetworkSetting>();
}

// 0x0041E908 | fefates:bytes
// 0x0041E89C (deleting dtor)
nn::pia::local::UdsMigrationManagerNew::~UdsMigrationManagerNew()
{
    if (m_pNextNetworkDescription != nullptr) {
        pead::FreeMemory(m_pNextNetworkDescription);
        m_pNextNetworkDescription = nullptr;
    }
    if (m_pCreateNetworkSetting != nullptr) {
        pead::FreeMemory(m_pCreateNetworkSetting);
        m_pCreateNetworkSetting = nullptr;
    }
    if (m_pConnectNetworkSetting != nullptr) {
        pead::FreeMemory(m_pConnectNetworkSetting);
        m_pConnectNetworkSetting = nullptr;
    }
}

// 0x007315C8 | fefates:bytes
u64 nn::pia::local::UdsMigrationManagerNew::GetLocalNodeKey(u16 nodeId) const
{
    nn::uds::CTR::NodeInformation information;
    if (nn::uds::CTR::GetNodeInformation(&information, nodeId).IsFailure()) {
        return m_InvalidNodeKey;
    }
    // the scrambled friend code of the station
    const u16* pCode = information.scrambledLocalFriendCode.code;
    return static_cast<u64>(pCode[0]) | static_cast<u64>(pCode[1]) << 16 | static_cast<u64>(pCode[2]) << 32 | static_cast<u64>(pCode[3]) << 48;
}

// 0x0073163C | fefates:bytes
u32 nn::pia::local::UdsMigrationManagerNew::GetLocalCommunicationId() const
{
    return GetNextNetworkDescription(m_pNextNetworkDescription)->m_Description.GetLocalCommunicationId();
}

// 0x0073165C
u8 nn::pia::local::UdsMigrationManagerNew::GetSubId() const
{
    return m_pNextNetworkDescription->GetSubId();
}

} // namespace local
} // namespace pia
} // namespace nn
