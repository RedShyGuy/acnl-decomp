#include "nn/pia/local/local_UdsAroundNetworkSearchManager.h"
#include "nn/pia/common/common_NewArray.h"
#include "nn/pia/local/local_LocalAroundNetworkSearchManager_LocalAroundNetworkStatusMessage.h"
#include "nn/pia/local/local_UdsAroundNetworkSearchBackgroundJob.h"
#include <cstring>

namespace nn {
namespace pia {
namespace local {
namespace {
// the received network is taken into a status
inline void CopyAroundNetworkInfo(UdsAroundNetworkInfo* pInfo, const UdsAroundNetworkInfo& source)
{
    pInfo->m_Description.m_Description = source.m_Description.m_Description;
    std::memcpy(pInfo->m_StationInfos, source.m_StationInfos, sizeof(pInfo->m_StationInfos));
}
} // namespace

// 0x0042343C
nn::pia::local::LocalAroundNetworkInfo* nn::pia::local::UdsAroundNetworkSearchManager::CreateLocalAroundNetworkInfo()
{
    return common::NewObject<UdsAroundNetworkInfo>();
}

// 0x00423484
void nn::pia::local::UdsAroundNetworkSearchManager::DeserializeAroundNetworkStatus(const nn::pia::local::LocalAroundNetworkSearchManager::LocalAroundNetworkStatusMessage* pMessage)
{
    u32 key = 0;
    pMessage->GetData(&key, 0, sizeof(key));
    pMessage->GetData(&m_ReceivedInfo, sizeof(key), sizeof(m_ReceivedInfo));
    // the status of the network or a free one
    s32 freeIndex = -1;
    for (u32 i = 0; i < AROUND_NETWORK_STATUS_NUM; i++) {
        AroundNetworkStatus& status = m_Statuses[i];
        if (status.m_LifeTime == 0) {
            if (freeIndex == -1) {
                freeIndex = i;
            }
            continue;
        }
        if (status.m_Key == key) {
            CopyAroundNetworkInfo(static_cast<UdsAroundNetworkInfo*>(status.m_pInfo), m_ReceivedInfo);
            status.m_LifeTime = m_Setting.m_LifeTimeMsec;
            return;
        }
    }
    if (freeIndex == -1) {
        return;
    }
    AroundNetworkStatus& status = m_Statuses[freeIndex];
    CopyAroundNetworkInfo(static_cast<UdsAroundNetworkInfo*>(status.m_pInfo), m_ReceivedInfo);
    status.m_LifeTime = m_Setting.m_LifeTimeMsec;
    status.m_Key = key;
}

// 0x00423588
nn::pia::local::LocalAroundNetworkSearchBackgroundJob* nn::pia::local::UdsAroundNetworkSearchManager::CreateLocalAroundNetworkSearchBackgroundJob()
{
    return common::NewObject<UdsAroundNetworkSearchBackgroundJob>();
}

// 0x004235B0
nn::pia::local::UdsAroundNetworkSearchManager::UdsAroundNetworkSearchManager()
{
    // only the members (in the original too)
}

// 0x004249BC
// 0x00423604 (deleting dtor)
nn::pia::local::UdsAroundNetworkSearchManager::~UdsAroundNetworkSearchManager()
{
    // empty (in the original too)
}

// 0x004149FC
void nn::pia::local::UdsAroundNetworkSearchManager::SerializeAroundNetworkStatus(nn::pia::local::LocalAroundNetworkSearchManager::LocalAroundNetworkStatusMessage* pMessage, const nn::pia::local::LocalAroundNetworkSearchManager::AroundNetworkStatus* pStatus) const
{
    pMessage->SetData(&pStatus->m_Key, sizeof(pStatus->m_Key));
    pMessage->SetData(pStatus->m_pInfo, sizeof(UdsAroundNetworkInfo));
}

// 0x007316E4
nn::Result nn::pia::local::UdsAroundNetworkSearchManager::GetAroundNetworkInfoList(void* pBuffer, u32* pNum, u32 bufferNum)
{
    m_CriticalSection.Lock();
    u32 num = 0;
    for (u32 i = 0; i < AROUND_NETWORK_STATUS_NUM && num < bufferNum; i++) {
        if (m_Statuses[i].m_LifeTime == 0) {
            continue;
        }
        std::memcpy(static_cast<UdsAroundNetworkInfo*>(pBuffer) + num, m_Statuses[i].m_pInfo, sizeof(UdsAroundNetworkInfo));
        num++;
    }
    *pNum = num;
    m_CriticalSection.Unlock();
    return nn::Result();
}

// 0x00731768
void nn::pia::local::UdsAroundNetworkSearchManager::vf_0x24()
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn
