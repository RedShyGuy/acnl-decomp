#include "nn/pia/common/common_InetAddress.h"
#include "nn/pia/common/common_Api.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_String.h"

namespace nn {
namespace pia {
namespace common {
// 0x00426BD4 | fefates:bytes [tier B]
nn::Result nn::pia::common::InetAddress::Deserialize(const unsigned char* pBuffer)
{
    if (!IsValidPointer(pBuffer)) {
        return RESULT_INVALID_ARGUMENT;
    }
    m_Address = deserializeU32(pBuffer);
    m_Port = deserializeU16(pBuffer + sizeof(u32));
    return nn::Result();
}

// 0x00426C20 | fefates:callgraph [tier C]
nn::pia::common::InetAddress::InetAddress(const nn::pia::common::InetAddress& rhs)
    : m_Address(rhs.m_Address), m_Port(rhs.m_Port)
{
}

// 0x00426C34 | fefates:callgraph [tier C]
nn::pia::common::InetAddress::InetAddress(unsigned int address, unsigned short port) : m_Address(address), m_Port(port)
{
}

// 0x00426C40 | fefates:callgraph [tier C]
nn::pia::common::InetAddress::InetAddress() : m_Address(0), m_Port(0)
{
}

// 0x00426C54 | fefates:callgraph [tier C]
InetAddress& nn::pia::common::InetAddress::operator=(const nn::pia::common::InetAddress& rhs)
{
    m_Address = rhs.m_Address;
    m_Port = rhs.m_Port;
    return *this;
}

// 0x0073181C | fefates:callgraph [tier C]
bool nn::pia::common::InetAddress::IsValidAddress() const
{
    return m_Address != 0;
}

// 0x0073182C | fefates:bytes [tier B]
void nn::pia::common::InetAddress::GetAddressString(nn::pia::common::String* pString) const
{
    pString->Format("%d.%d.%d.%d:%d", m_Address >> 24, (m_Address & 0xFF0000) >> 16, (m_Address & 0xFF00) >> 8,
                    m_Address & 0xFF, m_Port);
}

// 0x00731888 | fefates:bytes [tier B]
s64 nn::pia::common::InetAddress::GetKey() const
{
    return static_cast<s64>((static_cast<u64>(m_Address) << 32) | m_Port);
}

// 0x007318A4 | fefates:bytes [tier B]
bool nn::pia::common::InetAddress::IsValid() const
{
    return m_Address != 0 && m_Port != 0;
}

// 0x007318C4 | fefates:bytes [tier B]
bool nn::pia::common::InetAddress::IsPrivate() const
{
    u32 a = m_Address >> 24;
    u32 b = (m_Address & 0xFF0000) >> 16;
    if (a == 10) {
        return true;
    }
    if (a == 172) {
        return 16 <= b && b < 32;
    }
    if (a == 192 && b == 168) {
        return true;
    }
    return false;
}

// 0x00731914 | fefates:bytes [tier B]
nn::Result nn::pia::common::InetAddress::Serialize(unsigned char* pBuffer, unsigned int* pSize, unsigned int bufferSize) const
{
    if (!IsValidPointer(pBuffer) || !IsValidPointer(pSize)) {
        return RESULT_INVALID_ARGUMENT;
    }
    if (bufferSize < SERIALIZED_SIZE) {
        return RESULT_INVALID_ARGUMENT;
    }
    serializeU32(pBuffer, m_Address);
    serializeU16(pBuffer + sizeof(u32), m_Port);
    *pSize = SERIALIZED_SIZE;
    return nn::Result();
}

} // namespace common
} // namespace pia
} // namespace nn
