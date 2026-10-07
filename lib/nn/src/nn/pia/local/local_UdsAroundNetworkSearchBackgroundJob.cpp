#include "nn/pia/local/local_UdsAroundNetworkSearchBackgroundJob.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/local/local_LocalAroundNetworkSearchManager.h"
#include "nn/pia/local/local_LocalNetwork.h"
#include "nn/pia/local/local_UdsAroundNetworkSearchManager.h"
#include "nn/pia/local/local_UdsNetworkManager.h"
#include "nn/uds/CTR/CTR_Api.h"
#include "nn/uds/CTR/uds_NetworkDescriptionReader.h"
#include "nn/uds/CTR/uds_ScanResultReader.h"
#include <cstring>

namespace nn {
namespace pia {
namespace local {
// 0x004258C8
nn::Result nn::pia::local::UdsAroundNetworkSearchBackgroundJob::ScanNetwork()
{
    common::CriticalSection& networkCriticalSection = LocalNetwork::s_pInstance->m_CriticalSection;
    networkCriticalSection.Lock();
    LocalNetwork* pNetwork = LocalNetwork::s_pInstance;
    if ((!pNetwork->IsHost() && !pNetwork->IsClient()) || LocalNetwork::s_pInstance->IsDuringHostMigration()) {
        networkCriticalSection.Unlock();
        return common::RESULT_INVALID_STATE;
    }
    nn::Result result = nn::uds::CTR::ScanOnConnection(LocalNetwork::s_pInstance->m_pScanBuffer, LocalNetwork::s_pInstance->m_ScanBufferSize, m_Setting.m_SubId,
                                                       m_Setting.m_LocalCommunicationId, 0, m_Setting.m_ScanTime);
    if (result.IsFailure()) {
        result = static_cast<UdsNetworkManager*>(LocalNetwork::s_pInstance->m_pNetworkManager)->ConvertUdsResult(result);
        networkCriticalSection.Unlock();
        return result;
    }
    nn::uds::CTR::ScanResultReader reader(LocalNetwork::s_pInstance->m_pScanBuffer);
    u32 count = reader.GetCount();
    LocalAroundNetworkSearchManager* pManager = LocalNetwork::s_pInstance->m_pAroundNetworkSearchManager;
    common::CriticalSection& criticalSection = pManager->m_CriticalSection;
    criticalSection.Lock();
    if (!LocalNetwork::s_pInstance->m_pAroundNetworkSearchManager->m_IsSearching) {
        criticalSection.Unlock();
        networkCriticalSection.Unlock();
        return common::RESULT_INVALID_STATE;
    }
    for (u32 n = 0; n < count; n++) {
        nn::uds::CTR::NetworkDescriptionReader descriptionReader = reader.GetNextDescription();
        if (descriptionReader.GetNetworkDescription(&m_Description.m_Description).IsFailure()) {
            continue;
        }
        // not the network of the own session
        u32 sessionId = LocalNetwork::GetSessionId(&m_Description);
        if (LocalNetwork::s_pInstance->GetSessionId() == sessionId) {
            continue;
        }
        // the status of the network or a free one
        s32 freeIndex = -1;
        s32 index = -1;
        for (u32 i = 0; i < LocalAroundNetworkSearchManager::AROUND_NETWORK_STATUS_NUM; i++) {
            LocalAroundNetworkSearchManager::AroundNetworkStatus* pStatus = LocalNetwork::s_pInstance->m_pAroundNetworkSearchManager->GetAroundNetworkStatus(i);
            if (pStatus->m_LifeTime == 0) {
                if (freeIndex == -1) {
                    freeIndex = i;
                }
                continue;
            }
            if (pStatus->m_Key == sessionId) {
                index = i;
                break;
            }
        }
        if (index == -1) {
            if (freeIndex == -1) {
                continue;
            }
            index = freeIndex;
        }
        if (descriptionReader.GetNodeInformationList(m_NodeInformations).IsFailure()) {
            continue;
        }
        LocalAroundNetworkSearchManager::AroundNetworkStatus* pStatus = LocalNetwork::s_pInstance->m_pAroundNetworkSearchManager->GetAroundNetworkStatus(index);
        UdsAroundNetworkInfo* pInfo = static_cast<UdsAroundNetworkInfo*>(pStatus->m_pInfo);
        pInfo->m_Description.m_Description = m_Description.m_Description;
        for (u32 i = 0; i < UdsAroundNetworkInfo::STATION_INFO_NUM; i++) {
            LocalStationInfo& stationInfo = pInfo->m_StationInfos[i];
            std::memcpy(stationInfo.m_ScrambledLocalFriendCode, &m_NodeInformations[i].scrambledLocalFriendCode, sizeof(stationInfo.m_ScrambledLocalFriendCode));
            std::memcpy(stationInfo.m_UserName, &m_NodeInformations[i].userName, sizeof(stationInfo.m_UserName));
            stationInfo.m_Role = static_cast<UdsNetworkManager*>(LocalNetwork::s_pInstance->m_pNetworkManager)->ConvertLocalNodeIdToStationInfoRole(m_NodeInformations[i].nodeId);
        }
        pStatus->m_LifeTime = m_Setting.m_LifeTimeMsec;
        pStatus->m_DestinationBitmap = LocalNetwork::s_pInstance->m_pAroundNetworkSearchManager->GetMessageDestBitmap();
        pStatus->m_Version = ++LocalNetwork::s_pInstance->m_pAroundNetworkSearchManager->m_StatusVersion;
        pStatus->m_Key = sessionId;
    }
    criticalSection.Unlock();
    networkCriticalSection.Unlock();
    return nn::Result();
}

// 0x00425BFC
nn::pia::local::UdsAroundNetworkSearchBackgroundJob::UdsAroundNetworkSearchBackgroundJob()
{
    // only the base and the members (in the original too)
}

// 0x00425DD0
// 0x00425C20 (deleting dtor)
nn::pia::local::UdsAroundNetworkSearchBackgroundJob::~UdsAroundNetworkSearchBackgroundJob()
{
    // empty (in the original too)
}

// 0x00731804
void nn::pia::local::UdsAroundNetworkSearchBackgroundJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn
