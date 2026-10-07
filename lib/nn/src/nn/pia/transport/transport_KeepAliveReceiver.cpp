#include "nn/pia/transport/transport_KeepAliveReceiver.h"
#include "nn/pia/common/common_Time.h"
#include "nn/pia/transport/transport_StationManager.h"

namespace nn {
namespace pia {
namespace transport {
// 0x0045419C | fefates:bytes [tier B]
void nn::pia::transport::KeepAliveReceiver::Update(unsigned int receivedBitmap, const nn::pia::common::Time& now)
{
    if (!m_IsEnabled || receivedBitmap == 0) {
        return;
    }
    StationManager* pManager = StationManager::s_pInstance;
    for (s32 i = 0; i <= STATION_INDEX_MAX; i++) {
        if (receivedBitmap & (1 << i)) {
            Station* pStation = pManager->GetStation(static_cast<StationIndex>(i));
            if (common::IsValidPointer(pStation)) {
                pStation->m_LastReceiveTime = now;
            }
        }
    }
}

// 0x00454210 | fefates:bytes [tier B]
nn::pia::transport::KeepAliveReceiver::KeepAliveReceiver() : m_IsEnabled(true)
{
}

} // namespace transport
} // namespace pia
} // namespace nn
