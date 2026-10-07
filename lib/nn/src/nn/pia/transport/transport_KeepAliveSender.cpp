#include "nn/pia/transport/transport_KeepAliveSender.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_Time.h"
#include "nn/pia/transport/transport_StationManager.h"

namespace nn {
namespace pia {
namespace transport {
// 0x00450054 | fefates:bytes [tier B]
nn::Result nn::pia::transport::KeepAliveSender::SetInterval(int intervalMSec)
{
    if (intervalMSec < 0) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    m_IntervalMSec = intervalMSec;
    return nn::Result();
}

// 0x0045006C | fefates:bytes [tier B]
u32 nn::pia::transport::KeepAliveSender::Update(unsigned int sentBitmap, const nn::pia::common::Time& now)
{
    if (!m_IsEnabled) {
        return 0;
    }
    StationManager* pManager = StationManager::s_pInstance;
    Station* pLocalStation = pManager->m_pLocalStation;
    if (!common::IsValidPointer(pLocalStation)) {
        return 0;
    }
    common::Time limit(now.m_Tick - common::TimeSpan::GetTicksPerMSec().GetTick() * m_IntervalMSec);
    u32 bitmap = 0;
    for (Station** it = pManager->m_ActiveStations.Begin(); it != pManager->m_ActiveStations.End(); it++) {
        Station* pStation = *it;
        if (pStation == pLocalStation || pStation->m_State != Station::STATION_STATE_CONNECTED ||
            pStation->m_StationIndex > STATION_INDEX_MAX) {
            continue;
        }
        u32 bit = 1 << pStation->m_StationIndex;
        if (sentBitmap & bit) {
            pStation->m_LastSendTime = now;
        } else if (pStation->m_LastSendTime < limit) {
            bitmap |= bit;
        }
    }
    return bitmap;
}

// 0x00450160 | fefates:bytes [tier B]
nn::pia::transport::KeepAliveSender::KeepAliveSender() : m_IntervalMSec(1000), m_IsEnabled(true)
{
}

} // namespace transport
} // namespace pia
} // namespace nn
