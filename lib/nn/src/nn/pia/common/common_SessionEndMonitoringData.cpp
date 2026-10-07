#include "nn/pia/common/common_SessionEndMonitoringData.h"
#include "nn/pia/common/common_Result.h"
#include <string.h>

namespace nn {
namespace pia {
namespace common {
// 0x00AE79E8
SessionEndMonitoringData g_SessionEndMonitoringData;

// 0x00428680 (name is ours)
void nn::pia::common::SessionEndMonitoringData::Initialize(u8 type)
{
    memset(&m_Header, 0xFF, sizeof(m_Header));
    m_Header.m_Version = MonitoringHeader::VERSION;
    m_Header.m_Type = type;
    m_Header.m_Size = sizeof(MonitoringHeader) + SessionBeginMonitoringContent::GetSerializedSize() +
                      SessionStateMonitoringContent::GetSerializedSize();
    m_BeginContent.Initialize();
    m_StateContent.Initialize();
}

// 0x00731C90 | fefates:bytes-fuzzy [tier B]
nn::Result nn::pia::common::SessionEndMonitoringData::Serialize(unsigned char* pBuffer, unsigned int* pSize, unsigned int bufferSize) const
{
    if (!IsValidPointer(pBuffer) || !IsValidPointer(pSize) ||
        bufferSize < sizeof(MonitoringHeader) + SessionBeginMonitoringContent::GetSerializedSize() +
                         SessionStateMonitoringContent::GetSerializedSize()) {
        return RESULT_INVALID_ARGUMENT;
    }
    unsigned int size = 0;
    nn::Result result = SerializeHeader(pBuffer, &size, bufferSize);
    if (result.IsFailure()) {
        return result;
    }
    unsigned char* pBegin = pBuffer + size;
    unsigned int restSize = bufferSize - size;
    result = m_BeginContent.Serialize(pBegin, &size, restSize);
    if (result.IsFailure()) {
        return result;
    }
    unsigned char* pState = pBegin + size;
    result = m_StateContent.Serialize(pState, &size, restSize - size);
    if (result.IsFailure()) {
        return result;
    }
    *pSize = pState + size - pBuffer;
    return nn::Result();
}

} // namespace common
} // namespace pia
} // namespace nn
