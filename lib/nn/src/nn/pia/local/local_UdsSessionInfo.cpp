#include "nn/pia/local/local_UdsSessionInfo.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/local/local_LocalNetwork.h"
#include "nn/pia/session/session_SessionInfoList.h"
#include <cstring>

namespace nn {
namespace pia {
namespace local {
namespace {
const u32 BSSID_SIZE = 6;
} // namespace

// 0x0041672C
nn::Result nn::pia::local::UdsSessionInfo::GetApplicationData(void* pBuffer, u32 bufferSize) const
{
    LocalNetwork* pNetwork = LocalNetwork::s_pInstance;
    if (!common::IsValidPointer(pNetwork) || !m_IsValid) {
        return common::RESULT_INVALID_STATE;
    }
    if (!common::IsValidPointer(pBuffer)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    u32 size = 0;
    return pNetwork->GetApplicationData(pBuffer, &size, bufferSize, &m_Description);
}

// 0x004167A8
nn::Result nn::pia::local::UdsSessionInfo::UpdateStationInfos(u32 index)
{
    return LocalNetwork::s_pInstance->GetStationInfoList(m_StationInfos, STATION_INFO_NUM, index);
}

// 0x004167C4
nn::Result nn::pia::local::UdsSessionInfo::GetApplicationDataSize(u32* pSize) const
{
    LocalNetwork* pNetwork = LocalNetwork::s_pInstance;
    if (!common::IsValidPointer(pNetwork) || !m_IsValid) {
        return common::RESULT_INVALID_STATE;
    }
    if (!common::IsValidPointer(pSize)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    nn::Result result = pNetwork->GetApplicationDataSize(pSize, &m_Description);
    // a network without application data
    if (result == common::RESULT_NO_DATA) {
        *pSize = 0;
        return nn::Result();
    }
    return result;
}

// 0x00416844
void nn::pia::local::UdsSessionInfo::SetNetworkDescription(const nn::pia::local::LocalNetworkDescription* pDescription)
{
    m_Description.m_Description = static_cast<const UdsNetworkDescription*>(pDescription)->m_Description;
    m_IsValid = true;
}

// 0x00416868
void nn::pia::local::UdsSessionInfo::Clear()
{
    LocalSessionInfo::Clear();
    for (u32 i = 0; i < STATION_INFO_NUM; i++) {
        m_StationInfos[i].m_Role = LocalStationInfo::ROLE_NONE;
    }
}

// 0x004168B0
void nn::pia::local::UdsSessionInfo::Trace(u64) const
{
    // empty (in the original too)
}

// 0x004168B4
nn::pia::local::UdsSessionInfo::UdsSessionInfo()
{
    Clear();
}

// 0x00416BDC
// 0x00416938 (deleting dtor)
nn::pia::local::UdsSessionInfo::~UdsSessionInfo()
{
    // empty (in the original too)
}

// 0x00730084
u8 nn::pia::local::UdsSessionInfo::GetSubId() const
{
    if (!m_IsValid) {
        return 0;
    }
    return m_Description.m_Description.GetSubId();
}

// 0x007300A0
u32 nn::pia::local::UdsSessionInfo::GetSessionId() const
{
    if (!common::IsValidPointer(LocalNetwork::s_pInstance) || !m_IsValid) {
        return 0;
    }
    return LocalNetwork::GetSessionId(&m_Description);
}

// 0x007300E0
u8 nn::pia::local::UdsSessionInfo::GetMaxParticipants() const
{
    if (!m_IsValid) {
        return 0;
    }
    return m_Description.m_Description.GetNodeCountMax();
}

// 0x007300FC
nn::Result nn::pia::local::UdsSessionInfo::GetStationInfos(nn::pia::local::LocalStationInfoBuffer* pBuffer) const
{
    if (!m_IsValid) {
        return common::RESULT_INVALID_STATE;
    }
    if (!common::IsValidPointer(pBuffer)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    for (u32 i = 0; i < pBuffer->m_InfoNum; i++) {
        LocalStationInfo& info = pBuffer->m_pInfos[i];
        info.m_Role = m_StationInfos[i].m_Role;
        std::memcpy(info.m_UserName, m_StationInfos[i].m_UserName, sizeof(info.m_UserName));
        std::memcpy(info.m_ScrambledLocalFriendCode, m_StationInfos[i].m_ScrambledLocalFriendCode, sizeof(info.m_ScrambledLocalFriendCode));
    }
    return nn::Result();
}

// 0x007301A0
u8 nn::pia::local::UdsSessionInfo::GetCurrentParticipants() const
{
    if (!m_IsValid) {
        return 0;
    }
    return m_Description.m_Description.GetNodeCount();
}

// 0x007301BC
const nn::pia::local::LocalNetworkDescription* nn::pia::local::UdsSessionInfo::GetNetworkDescription() const
{
    return &m_Description;
}

// 0x007301C4
nn::Result nn::pia::local::UdsSessionInfo::GetBssid(u8* pBssid) const
{
    if (!m_IsValid) {
        return common::RESULT_INVALID_STATE;
    }
    if (!common::IsValidPointer(pBssid)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    std::memcpy(pBssid, m_Description.m_Description.m_Bssid, BSSID_SIZE);
    return nn::Result();
}

// 0x00730214
bool nn::pia::local::UdsSessionInfo::IsOpened() const
{
    if (!m_IsValid) {
        return false;
    }
    return m_Description.IsOpened();
}

} // namespace local
} // namespace pia
} // namespace nn

// the list of UdsNetworkFactory (out of line in the original)

// 0x007E5374
// 0x007E52F8 (deleting dtor)
template nn::pia::session::SessionInfoList<nn::pia::local::UdsSessionInfo>::~SessionInfoList();
// 0x00828028
template nn::pia::session::ISessionInfo** nn::pia::session::SessionInfoList<nn::pia::local::UdsSessionInfo>::Begin() const;
// 0x007E5290
template nn::pia::session::ISessionInfo** nn::pia::session::SessionInfoList<nn::pia::local::UdsSessionInfo>::Begin();
// 0x00828018
template nn::pia::session::ISessionInfo** nn::pia::session::SessionInfoList<nn::pia::local::UdsSessionInfo>::End() const;
// 0x007E5280
template nn::pia::session::ISessionInfo** nn::pia::session::SessionInfoList<nn::pia::local::UdsSessionInfo>::End();
// 0x00828030
template u32 nn::pia::session::SessionInfoList<nn::pia::local::UdsSessionInfo>::GetSize() const;
// 0x00828010
template u32 nn::pia::session::SessionInfoList<nn::pia::local::UdsSessionInfo>::GetCapacity() const;
// 0x007E5298
template void nn::pia::session::SessionInfoList<nn::pia::local::UdsSessionInfo>::Clear();
