#include "nn/pia/transport/transport_ConnectionAnalyzer.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/transport/transport_Station.h"
#include "nn/pia/transport/transport_StationManager.h"
#include "nn/pia/transport/transport_Transport.h"

namespace nn {
namespace pia {
namespace transport {
// 0x0045794C (name is ours)
nn::Result nn::pia::transport::ConnectionAnalyzer::Update()
{
    if (!m_IsStarted) {
        return common::RESULT_INVALID_STATE;
    }
    StationManager* pManager = StationManager::s_pInstance;
    if (!common::IsValidPointer(pManager)) {
        return common::RESULT_INVALID_STATE;
    }
    bool isUpdated[STATION_INDEX_MAX + 1] = {};
    Station* pLocalStation = pManager->m_pLocalStation;
    for (Station** it = pManager->m_ActiveStations.Begin(); it != pManager->m_ActiveStations.End(); it++) {
        Station* pStation = *it;
        if (pStation == pLocalStation) {
            continue;
        }
        StationIndex stationIndex = pStation->m_StationIndex;
        if (stationIndex >= ConnectionAnalysisData::STATION_NUM) {
            continue;
        }
        const SequenceIdController* pController = pStation->GetSequenceIdController();
        if (!common::IsValidPointer(pController)) {
            continue;
        }
        isUpdated[stationIndex] = true;
        ConnectionAnalysisData::StationData& data = m_Data.m_StationData[stationIndex];
        data.m_Rtt = pStation->GetRtt();
        if (data.m_IsValid) {
            data.m_LastReceivedNum = data.m_ReceivedNum;
            data.m_ReceivedNum = pController->m_TotalReceivedNum;
            data.m_LastLostNum = data.m_LostNum;
            data.m_LostNum = pController->m_TotalLostNum;
        } else {
            data.m_ReceivedNum = pController->m_TotalReceivedNum;
            data.m_LostNum = pController->m_TotalLostNum;
            data.m_LastReceivedNum = data.m_ReceivedNum;
            data.m_IsValid = true;
            data.m_LastLostNum = data.m_LostNum;
        }
    }
    // the original also clears the flag of index 11, behind the 11 entries (in the padding of
    // the analyzer)
    for (u32 i = 0; i <= STATION_INDEX_MAX; i++) {
        if (!isUpdated[i]) {
            reinterpret_cast<ConnectionAnalysisData::StationData*>(this)[i].m_IsValid = false;
        }
    }
    return nn::Result();
}

// 0x00457A9C | fefates:bytes [tier B]
void nn::pia::transport::ConnectionAnalyzer::Cleanup()
{
    if (m_IsStarted) {
        m_IsStarted = false;
    }
}

// 0x00457AB0 (name is ours)
nn::Result nn::pia::transport::ConnectionAnalyzer::Startup()
{
    if (m_IsStarted) {
        return common::RESULT_INVALID_STATE;
    }
    m_Data.Clear();
    m_StartTime = Transport::s_pInstance->m_DispatchTime;
    m_IsStarted = true;
    return nn::Result();
}

// 0x00457AFC | fefates:bytes [tier B]
nn::pia::transport::ConnectionAnalyzer::ConnectionAnalyzer() : m_Data(), m_IsStarted(false)
{
}

// 0x00735B3C | fefates:callseq [tier C]
nn::Result nn::pia::transport::ConnectionAnalyzer::GetConnectionAnalysisData(nn::pia::transport::ConnectionAnalysisData* pData) const
{
    if (!common::IsValidPointer(pData)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    *pData = m_Data;
    pData->m_ElapsedMSec = static_cast<s32>((Transport::GetCurrentTime() - m_StartTime).GetTick() /
                                            common::TimeSpan::GetTicksPerMSec().GetTick());
    return nn::Result();
}

} // namespace transport
} // namespace pia
} // namespace nn
