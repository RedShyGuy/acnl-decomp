#include "nn/pia/common/common_MonitoringData.h"
#include "nn/pia/common/common_Api.h"
#include "nn/pia/common/common_Result.h"

namespace nn {
namespace pia {
namespace common {
namespace {
// (inline in both SerializeHeader functions)
DECOMP_ALWAYS_INLINE nn::Result SerializeMonitoringHeader(const MonitoringHeader& header, unsigned char* pBuffer,
                                                          unsigned int* pSize, unsigned int bufferSize)
{
    if (!IsValidPointer(pBuffer) || !IsValidPointer(pSize)) {
        return RESULT_INVALID_ARGUMENT;
    }
    if (bufferSize < sizeof(MonitoringHeader)) {
        return RESULT_INVALID_ARGUMENT;
    }
    unsigned char* p = pBuffer;
    serializeU8(p, header.m_Version);
    p += 1;
    serializeU8(p, header.m_Type);
    p += 1;
    serializeU8(p, header.m_Unknown0x2);
    p += 1;
    serializeU8(p, header.m_Unknown0x3);
    p += 1;
    serializeU16(p, header.m_Size);
    p += 2;
    for (int i = 0; i < 10; i++) {
        serializeU8(p, header.m_Reserved[i]);
        p += 1;
    }
    *pSize = p - pBuffer;
    return nn::Result();
}
} // namespace

// 0x00731B04 (name is ours)
nn::Result nn::pia::common::MonitoringData::SerializeHeader(unsigned char* pBuffer, unsigned int* pSize, unsigned int bufferSize) const
{
    return SerializeMonitoringHeader(m_Header, pBuffer, pSize, bufferSize);
}

// 0x00731BD0 (name is ours)
nn::Result nn::pia::common::MonitoringContent::SerializeHeader(unsigned char* pBuffer, unsigned int* pSize, unsigned int bufferSize) const
{
    return SerializeMonitoringHeader(m_Header, pBuffer, pSize, bufferSize);
}

} // namespace common
} // namespace pia
} // namespace nn
