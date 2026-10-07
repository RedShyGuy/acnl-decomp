#include "nn/pia/inet/inet_NexSessionInfo.h"
#include "nn/pia/session/session_SessionInfoList.h"
#include "nn/nstd/nstd_String.h"
#include "nn/pia/common/common_Result.h"
#include <string.h>

namespace nn {
namespace pia {
namespace inet {
// 0x003E6FC4
nn::pia::inet::NexSessionInfo::NexSessionInfo()
    : m_Unknown0x4(0)
    , m_Unknown0x8(0)
    , m_Unknown0xC(0)
    , m_Unknown0x10(0)
    , m_Unknown0x14(0)
    , m_Unknown0x18(false)
    , m_StringLength(0)
    , m_DataSize(0)
    , m_Unknown0x440(false)
    , m_Unknown0x441(false)
    , m_Unknown0x442(0)
    , m_Unknown0x444(0)
    , m_Unknown0x448(0)
    , m_Unknown0x44C(255)
{
    for (u32 i = 0; i < ATTRIBUTE_NUM; i++) {
        m_Attributes[i] = 0;
    }
    memset(m_String, 0, sizeof(m_String));
    memset(m_Data, 0, sizeof(m_Data));
    m_DateTime.Clear();
}

// 0x003E70AC
// 0x003E709C (deleting dtor)
nn::pia::inet::NexSessionInfo::~NexSessionInfo()
{
    // empty (in the original too)
}

// 0x0072EFF4
u32 nn::pia::inet::NexSessionInfo::vf_0x08() const
{
    return m_Unknown0x4;
}

// 0x0072F038
u32 nn::pia::inet::NexSessionInfo::vf_0x0C() const
{
    return m_Unknown0x8;
}

// 0x0072F0D0
u32 nn::pia::inet::NexSessionInfo::vf_0x10() const
{
    return m_Unknown0xC;
}

// 0x0072F0B0
u32 nn::pia::inet::NexSessionInfo::vf_0x14() const
{
    return m_Unknown0x10;
}

// 0x0072F0A8
u32 nn::pia::inet::NexSessionInfo::vf_0x18() const
{
    return m_Unknown0x14;
}

// 0x0072F0F0
bool nn::pia::inet::NexSessionInfo::vf_0x1C() const
{
    return m_Unknown0x18;
}

// 0x003E6F18 (name is ours)
void nn::pia::inet::NexSessionInfo::Clear()
{
    m_Unknown0x4 = 0;
    m_Unknown0x8 = 0;
    m_Unknown0xC = 0;
    m_Unknown0x10 = 0;
    m_Unknown0x14 = 0;
    m_Unknown0x18 = false;
    m_StringLength = 0;
    m_DataSize = 0;
    for (u32 i = 0; i < ATTRIBUTE_NUM; i++) {
        m_Attributes[i] = 0;
    }
    memset(m_String, 0, sizeof(m_String));
    memset(m_Data, 0, sizeof(m_Data));
    m_Unknown0x440 = false;
    m_Unknown0x441 = false;
    m_Unknown0x442 = 0;
    m_Unknown0x444 = 0;
    m_Unknown0x448 = 0;
    m_Unknown0x44C = 255;
    m_DateTime.Clear();
}

// 0x003E6FB8
void nn::pia::inet::NexSessionInfo::Trace(u64) const
{
    // empty (in the original too)
}

// 0x003E6DC4
nn::Result nn::pia::inet::NexSessionInfo::vf_0x28(void* pBuffer, unsigned int size) const
{
    if (!common::IsValidPointer(pBuffer) || size < m_DataSize) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    memcpy(pBuffer, m_Data, size);
    return nn::Result();
}

// 0x0072F0C8
u32 nn::pia::inet::NexSessionInfo::vf_0x2C() const
{
    return m_DataSize;
}

// 0x0072EFFC
nn::Result nn::pia::inet::NexSessionInfo::vf_0x30(u32* pValue, unsigned int index) const
{
    if (!common::IsValidPointer(pValue) || index >= ATTRIBUTE_NUM) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    *pValue = m_Attributes[index];
    return nn::Result();
}

// 0x0072F040
nn::Result nn::pia::inet::NexSessionInfo::vf_0x34(u16* pBuffer, unsigned int size) const
{
    if (!common::IsValidPointer(pBuffer) || size < m_StringLength) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    memcpy(pBuffer, m_String, size);
    return nn::Result();
}

// 0x0072F098
u32 nn::pia::inet::NexSessionInfo::vf_0x38() const
{
    return m_StringLength;
}

// 0x0072F0D8
bool nn::pia::inet::NexSessionInfo::vf_0x3C() const
{
    return m_Unknown0x440;
}

// 0x0072F0E4
bool nn::pia::inet::NexSessionInfo::vf_0x40() const
{
    return m_Unknown0x441;
}

// 0x0072F088
u8 nn::pia::inet::NexSessionInfo::vf_0x44() const
{
    return m_Unknown0x442;
}

// 0x0072F0A0
u32 nn::pia::inet::NexSessionInfo::vf_0x48() const
{
    return m_Unknown0x444;
}

// 0x0072F0B8
u32 nn::pia::inet::NexSessionInfo::vf_0x4C() const
{
    return m_Unknown0x448;
}

// 0x0072F0C0
u8 nn::pia::inet::NexSessionInfo::vf_0x50() const
{
    return m_Unknown0x44C;
}

// 0x0072F090
const nn::pia::common::DateTime* nn::pia::inet::NexSessionInfo::vf_0x54() const
{
    return &m_DateTime;
}

// 0x003E6E24 (name is ours)
void nn::pia::inet::NexSessionInfo::Copy(const nn::pia::inet::NexSessionInfo& rhs)
{
    m_Unknown0x4 = rhs.m_Unknown0x4;
    m_Unknown0x8 = rhs.m_Unknown0x8;
    m_Unknown0xC = rhs.m_Unknown0xC;
    m_Unknown0x10 = rhs.m_Unknown0x10;
    m_Unknown0x14 = rhs.m_Unknown0x14;
    m_Unknown0x18 = rhs.m_Unknown0x18;
    m_StringLength = rhs.m_StringLength;
    m_DataSize = rhs.m_DataSize;
    nnnstdMemCpy(m_Attributes, rhs.m_Attributes, sizeof(m_Attributes));
    nnnstdMemCpy(m_String, rhs.m_String, sizeof(m_String));
    nnnstdMemCpy(m_Data, rhs.m_Data, sizeof(m_Data));
    m_Unknown0x440 = rhs.m_Unknown0x440;
    m_Unknown0x441 = rhs.m_Unknown0x441;
    m_Unknown0x442 = rhs.m_Unknown0x442;
    m_Unknown0x444 = rhs.m_Unknown0x444;
    m_Unknown0x448 = rhs.m_Unknown0x448;
    m_Unknown0x44C = rhs.m_Unknown0x44C;
    m_DateTime.Set(rhs.m_DateTime);
}

// 0x003E6CAC
void nn::pia::inet::NexSessionInfo::vf_0x5C(u32 value)
{
    m_Unknown0x4 = value;
}

// 0x003E6CC4
void nn::pia::inet::NexSessionInfo::vf_0x60(u32 value)
{
    m_Unknown0x8 = value;
}

// 0x003E6E0C
void nn::pia::inet::NexSessionInfo::vf_0x64(u32 value)
{
    m_Unknown0xC = value;
}

// 0x003E6DAC
void nn::pia::inet::NexSessionInfo::vf_0x68(u32 value)
{
    m_Unknown0x10 = value;
}

// 0x003E6DA4
void nn::pia::inet::NexSessionInfo::vf_0x6C(u32 value)
{
    m_Unknown0x14 = value;
}

// 0x003E6FBC
void nn::pia::inet::NexSessionInfo::vf_0x70(bool value)
{
    m_Unknown0x18 = value;
}

// 0x003E6CB4
void nn::pia::inet::NexSessionInfo::vf_0x74(u32 value, unsigned int index)
{
    if (index < ATTRIBUTE_NUM) {
        m_Attributes[index] = value;
    }
}

// 0x003E6CCC
void nn::pia::inet::NexSessionInfo::vf_0x78(const u16* pString, unsigned int length)
{
    if (length > STRING_LENGTH_MAX) {
        length = STRING_LENGTH_MAX;
    }
    memset(m_String, 0, sizeof(m_String));
    nnnstdMemCpy(m_String, pString, length * sizeof(u16));
    m_String[length] = 0;
    m_StringLength = length;
}

// 0x003E6D60
void nn::pia::inet::NexSessionInfo::vf_0x7C(const void* pData, unsigned int size)
{
    if (size > DATA_SIZE_MAX) {
        size = DATA_SIZE_MAX;
    }
    memset(m_Data, 0, sizeof(m_Data));
    nnnstdMemCpy(m_Data, pData, size);
    m_DataSize = size;
}

// 0x003E6E14
void nn::pia::inet::NexSessionInfo::vf_0x80(bool value)
{
    m_Unknown0x440 = value;
}

// 0x003E6E1C
void nn::pia::inet::NexSessionInfo::vf_0x84(bool value)
{
    m_Unknown0x441 = value;
}

// 0x003E6D18
void nn::pia::inet::NexSessionInfo::vf_0x88(u8 value)
{
    m_Unknown0x442 = value;
}

// 0x003E6D9C
void nn::pia::inet::NexSessionInfo::vf_0x8C(u32 value)
{
    m_Unknown0x444 = value;
}

// 0x003E6DB4
void nn::pia::inet::NexSessionInfo::vf_0x90(u32 value)
{
    m_Unknown0x448 = value;
}

// 0x003E6DBC
void nn::pia::inet::NexSessionInfo::vf_0x94(u8 value)
{
    m_Unknown0x44C = value;
}

// 0x003E6D20
void nn::pia::inet::NexSessionInfo::vf_0x98(const nn::pia::common::DateTime& dateTime)
{
    m_DateTime.Set(dateTime);
}

} // namespace inet
} // namespace pia
} // namespace nn

// the list of NexNetworkFactory (out of line in the original)

// 0x007E5208
// 0x007E518C (deleting dtor)
template nn::pia::session::SessionInfoList<nn::pia::inet::NexSessionInfo>::~SessionInfoList();
// 0x00828000
template nn::pia::session::ISessionInfo** nn::pia::session::SessionInfoList<nn::pia::inet::NexSessionInfo>::Begin() const;
// 0x007E5124
template nn::pia::session::ISessionInfo** nn::pia::session::SessionInfoList<nn::pia::inet::NexSessionInfo>::Begin();
// 0x00827FF0
template nn::pia::session::ISessionInfo** nn::pia::session::SessionInfoList<nn::pia::inet::NexSessionInfo>::End() const;
// 0x007E5114
template nn::pia::session::ISessionInfo** nn::pia::session::SessionInfoList<nn::pia::inet::NexSessionInfo>::End();
// 0x00828008
template u32 nn::pia::session::SessionInfoList<nn::pia::inet::NexSessionInfo>::GetSize() const;
// 0x00827FE8
template u32 nn::pia::session::SessionInfoList<nn::pia::inet::NexSessionInfo>::GetCapacity() const;
// 0x007E512C
template void nn::pia::session::SessionInfoList<nn::pia::inet::NexSessionInfo>::Clear();
