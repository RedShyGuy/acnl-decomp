#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_MonitoringData.h"
#include "nn/pia/common/common_SessionBeginMonitoringContent.h"

namespace nn {
namespace pia {
namespace common {
// The monitoring data sent when a session begins: a header and the begin content. Layout from
// Initialize; the member names are ours.
class SessionBeginMonitoringData : public MonitoringData
{
public:
    // the header and the content (name is ours)
    void Initialize(); // 0x004283F8
    nn::Result Serialize(unsigned char* pBuffer, unsigned int* pSize, unsigned int bufferSize) const; // 0x00731D84 | fefates:bytes [tier B]

    SessionBeginMonitoringContent m_BeginContent; // 0x010
};
ASSERT_SIZE(SessionBeginMonitoringData, 0x4C0);

// the block inet::NexMonitoringDataSender sends (name is ours)
extern SessionBeginMonitoringData g_SessionBeginMonitoringData;
} // namespace common
} // namespace pia
} // namespace nn
