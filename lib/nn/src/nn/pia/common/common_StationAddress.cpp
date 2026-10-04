#include "nn/pia/common/common_StationAddress.h"
#include "nn/pia/common/common_Api.h"
#include "nn/pia/common/common_Result.h"

namespace nn {
namespace pia {
namespace common {
// 0x00426DB0 | fefates:bytes [tier B]
nn::Result nn::pia::common::StationAddress::Deserialize(const unsigned char* pBuffer)
{
    if (!IsValidPointer(pBuffer)) {
        return RESULT_INVALID_ARGUMENT;
    }
    nn::Result result = m_InetAddress.Deserialize(pBuffer);
    if (result.IsFailure()) {
        return result;
    }
    m_ExtensionId = deserializeU16(pBuffer + InetAddress::SERIALIZED_SIZE);
    return nn::Result();
}

// 0x00426E00 | fefates:callgraph [tier C]
nn::Result nn::pia::common::StationAddress::SetExtensionId(unsigned short extensionId)
{
    m_ExtensionId = extensionId;
    return nn::Result();
}

// 0x00426E0C | fefates:bytes [tier B]
nn::Result nn::pia::common::StationAddress::SetInetAddress(const nn::pia::common::InetAddress& address)
{
    m_InetAddress = address;
    return nn::Result();
}

// 0x00426E20 | fefates:bytes [tier B]
void nn::pia::common::StationAddress::Clear()
{
    m_InetAddress.m_Address = 0;
    m_InetAddress.m_Port = 0;
    m_ExtensionId = 0;
}

// 0x00426E3C | fefates:bytes [tier B]
int nn::pia::common::StationAddress::Compare(const nn::pia::common::StationAddress& lhs, const nn::pia::common::StationAddress& rhs)
{
    if (lhs.m_InetAddress.GetKey() < rhs.m_InetAddress.GetKey()) {
        return -1;
    }
    if (rhs.m_InetAddress.GetKey() < lhs.m_InetAddress.GetKey()) {
        return 1;
    }
    if (lhs.m_ExtensionId == rhs.m_ExtensionId) {
        return 0;
    }
    return lhs.m_ExtensionId < rhs.m_ExtensionId ? -1 : 1;
}

// 0x00426EC4 | fefates:bytes [tier B]
nn::pia::common::StationAddress::StationAddress(const nn::pia::common::StationAddress& rhs)
    : m_InetAddress(rhs.m_InetAddress), m_ExtensionId(rhs.m_ExtensionId)
{
}

// 0x00426EF0 | fefates:bytes [tier B]
nn::pia::common::StationAddress::StationAddress() : m_ExtensionId(0)
{
}

// 0x00426F40 | fefates:bytes [tier B]
StationAddress& nn::pia::common::StationAddress::operator=(const nn::pia::common::StationAddress& rhs)
{
    if (this != &rhs) {
        m_InetAddress = rhs.m_InetAddress;
        m_ExtensionId = rhs.m_ExtensionId;
    }
    return *this;
}

// 0x00731978 (name after StepSequenceJob::Trace)
void nn::pia::common::StationAddress::Trace(u64) const
{
    // empty (in the original too)
}

// 0x0073197C | fefates:bytes [tier B]
bool nn::pia::common::StationAddress::IsValid() const
{
    if (m_ExtensionId != 0) {
        return true;
    }
    return m_InetAddress.IsValid();
}

// 0x007319A4 | fefates:bytes [tier B]
nn::Result nn::pia::common::StationAddress::Serialize(unsigned char* pBuffer, unsigned int* pSize, unsigned int bufferSize) const
{
    if (!IsValidPointer(pBuffer) || !IsValidPointer(pSize)) {
        return RESULT_INVALID_ARGUMENT;
    }
    if (bufferSize < SERIALIZED_SIZE) {
        return RESULT_INVALID_ARGUMENT;
    }
    u32 size;
    nn::Result result = m_InetAddress.Serialize(pBuffer, &size, bufferSize - sizeof(u16));
    if (result.IsFailure()) {
        return result;
    }
    serializeU16(pBuffer + size, m_ExtensionId);
    *pSize = SERIALIZED_SIZE;
    return nn::Result();
}

// 0x00731A1C | fefates:bytes [tier B]
bool nn::pia::common::StationAddress::operator==(const nn::pia::common::StationAddress& rhs) const
{
    return m_InetAddress.GetKey() == rhs.m_InetAddress.GetKey() && m_ExtensionId == rhs.m_ExtensionId;
}

// 0x00731A70 | fefates:bytes [tier B]
bool nn::pia::common::StationAddress::operator<(const nn::pia::common::StationAddress& rhs) const
{
    if (m_InetAddress.GetKey() < rhs.m_InetAddress.GetKey()) {
        return true;
    }
    if (rhs.m_InetAddress.GetKey() < m_InetAddress.GetKey()) {
        return false;
    }
    return m_ExtensionId < rhs.m_ExtensionId;
}

} // namespace common
} // namespace pia
} // namespace nn
