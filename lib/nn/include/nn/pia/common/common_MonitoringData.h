#pragma once

// The monitoring data pia collects about a session (sent to a server by MonitoringDataSender).
// Each block starts with the same 16 byte header; the data and content objects serialize their
// header with their own (identical) functions. The file, the base classes and their members are
// ours; SessionBegin/EndMonitoringData and the content classes are from the fefates symbols.

#include "decomp.h"
#include "nn/Result.h"

namespace nn {
namespace pia {
namespace common {

struct MonitoringHeader
{
    // m_Version of all blocks
    static const u8 VERSION = 12;

    u8 m_Version;       // 0x0
    u8 m_Type;          // 0x1, 0 = session begin, 1 = session state / end
    u8 m_Unknown0x2;    // 0x2
    u8 m_Unknown0x3;    // 0x3
    u16 m_Size;         // 0x4, serialized size of the block
    u8 m_Reserved[10];  // 0x6
};
ASSERT_SIZE(MonitoringHeader, 0x10);

// base of SessionBegin/EndMonitoringData
class MonitoringData
{
public:
    nn::Result SerializeHeader(unsigned char* pBuffer, unsigned int* pSize, unsigned int bufferSize) const; // 0x00731B04

    MonitoringHeader m_Header; // 0x0
};

// base of the content blocks
class MonitoringContent
{
public:
    nn::Result SerializeHeader(unsigned char* pBuffer, unsigned int* pSize, unsigned int bufferSize) const; // 0x00731BD0

    MonitoringHeader m_Header; // 0x0
};

} // namespace common
} // namespace pia
} // namespace nn
