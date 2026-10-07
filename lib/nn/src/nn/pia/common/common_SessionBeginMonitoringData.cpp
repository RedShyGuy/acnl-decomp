#include "nn/pia/common/common_SessionBeginMonitoringData.h"
#include "nn/pia/common/common_Result.h"
#include <string.h>

namespace nn {
namespace pia {
namespace common {
// 0x00AE7528
SessionBeginMonitoringData g_SessionBeginMonitoringData;

// 0x004283F8 (name is ours)
void nn::pia::common::SessionBeginMonitoringData::Initialize()
{
    memset(&m_Header, 0xFF, sizeof(m_Header));
    m_Header.m_Version = MonitoringHeader::VERSION;
    m_Header.m_Type = 0;
    m_Header.m_Size = sizeof(MonitoringHeader) + SessionBeginMonitoringContent::GetSerializedSize();
    m_BeginContent.Initialize();
}

// 0x00731D84 | fefates:bytes [tier B]
nn::Result nn::pia::common::SessionBeginMonitoringData::Serialize(unsigned char* pBuffer, unsigned int* pSize, unsigned int bufferSize) const
{
    if (!IsValidPointer(pBuffer) || !IsValidPointer(pSize) ||
        bufferSize < sizeof(MonitoringHeader) + SessionBeginMonitoringContent::GetSerializedSize()) {
        return RESULT_INVALID_ARGUMENT;
    }
    unsigned int size = 0;
    nn::Result result = SerializeHeader(pBuffer, &size, bufferSize);
    if (result.IsFailure()) {
        return result;
    }
    unsigned char* p = pBuffer + size;
    result = m_BeginContent.Serialize(p, &size, bufferSize - size);
    if (result.IsFailure()) {
        return result;
    }
    *pSize = p + size - pBuffer;
    return nn::Result();
}

} // namespace common
} // namespace pia
} // namespace nn
