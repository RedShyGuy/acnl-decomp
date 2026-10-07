#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_MonitoringData.h"
#include "nn/pia/common/common_SessionBeginMonitoringContent.h"
#include "nn/pia/common/common_SessionStateMonitoringContent.h"

namespace nn {
namespace pia {
namespace common {
// The monitoring data sent when a session ends: a header, the begin content and the state
// content. Layout from Initialize; the member names are ours.
class SessionEndMonitoringData : public MonitoringData
{
public:
    // the header (with the given type) and the contents (names are ours)
    void Initialize(u8 type); // 0x00428680
    nn::Result Serialize(unsigned char* pBuffer, unsigned int* pSize, unsigned int bufferSize) const; // 0x00731C90 | fefates:bytes-fuzzy [tier B]

    SessionBeginMonitoringContent m_BeginContent; // 0x010
    SessionStateMonitoringContent m_StateContent; // 0x4C0
};
ASSERT_SIZE(SessionEndMonitoringData, 0x8B0);

// the block inet::NexMonitoringDataSender sends (name is ours)
extern SessionEndMonitoringData g_SessionEndMonitoringData;
} // namespace common
} // namespace pia
} // namespace nn
