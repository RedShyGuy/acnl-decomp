#include "nn/pia/inet/inet_NatProbeData.h"
#include "nn/pia/common/common_Api.h"
#include "nn/pia/common/common_Result.h"

namespace nn {
namespace pia {
namespace inet {
// 0x003E44B8 | fefates:bytes [tier B]
nn::Result nn::pia::inet::NatProbeData::Deserialize(const unsigned char* pBuffer, unsigned int size)
{
    if (!common::IsValidPointer(pBuffer) || size < SERIALIZED_SIZE) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    m_StationKey = common::deserializeU32(pBuffer);
    m_Type = pBuffer[4];
    m_Unknown0x9 = pBuffer[5];
    m_Unknown0xA = pBuffer[6];
    m_Unknown0xB = pBuffer[7];
    m_SendTime = common::deserializeU64(pBuffer + 8);
    return nn::Result();
}

// 0x003E4528 | fefates:bytes [tier B]
nn::pia::inet::NatProbeData::NatProbeData() : m_StationKey(0), m_Type(0), m_SendTime(0)
{
}

// 0x003E4550
// 0x003E454C (deleting dtor)
nn::pia::inet::NatProbeData::~NatProbeData()
{
    // empty (in the original too)
}

// 0x0072EF60 | fefates:bytes [tier B]
nn::Result nn::pia::inet::NatProbeData::Serialize(unsigned char* pBuffer, unsigned int* pSize, unsigned int bufferSize) const
{
    if (!common::IsValidPointer(pBuffer) || !common::IsValidPointer(pSize) || bufferSize < SERIALIZED_SIZE) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    common::serializeU32(pBuffer, m_StationKey);
    pBuffer[4] = m_Type;
    pBuffer[5] = m_Unknown0x9;
    pBuffer[6] = m_Unknown0xA;
    pBuffer[7] = m_Unknown0xB;
    common::serializeU64(pBuffer + 8, m_SendTime);
    *pSize = SERIALIZED_SIZE;
    return nn::Result();
}

} // namespace inet
} // namespace pia
} // namespace nn
