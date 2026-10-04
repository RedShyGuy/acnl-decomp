#include "nn/pia/common/common_PacketOld.h"
#include "nn/pia/common/common_Api.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_SignatureManager.h"
#include <string.h>

namespace nn {
namespace pia {
namespace common {
// 0x0097F9F0
u16 PacketOld::s_DefaultPayloadSize = DEFAULT_PAYLOAD_SIZE_MAX;
// 0x0097F9F4
unsigned int PacketOld::s_SignatureSize;
// 0x0097FA00
u32 PacketOld::s_SerializeCount;
// 0x0097FA04
u32 PacketOld::s_DeserializeCount;

// 0x004298D0 (name is ours)
nn::Result nn::pia::common::PacketOld::Deserialize(const unsigned char* pBuffer, unsigned int size)
{
    if (!IsValidPointer(pBuffer)) {
        return RESULT_INVALID_ARGUMENT;
    }
    if (deserializeU32(pBuffer) != MAGIC) {
        return RESULT_INVALID_ARGUMENT;
    }
    if (pBuffer[4] != VERSION) {
        return RESULT_INVALID_ARGUMENT;
    }
    if (size < HEADER_SIZE) {
        return RESULT_INVALID_ARGUMENT;
    }
    u16 packetSize = deserializeU16(pBuffer + 6);
    if (packetSize > DEFAULT_PAYLOAD_SIZE_MAX) {
        return RESULT_INVALID_FORMAT;
    }
    u16 payloadSize = deserializeU16(pBuffer + 12);
    if (packetSize < s_SignatureSize + payloadSize) {
        return RESULT_INVALID_FORMAT;
    }
    if (payloadSize + HEADER_SIZE > size) {
        return RESULT_INVALID_ARGUMENT;
    }
    SignatureManager* pSignatureManager = SignatureManager::GetInstance();
    if (pSignatureManager->m_State != SignatureManager::STATE_SETUP) {
        return RESULT_INVALID_FORMAT;
    }
    if (!pSignatureManager->m_DefaultContext.Check(pBuffer, size)) {
        return RESULT_INVALID_FORMAT;
    }
    const unsigned char* p = pBuffer + sizeof(u32);
    m_Version = *p++;
    m_Unknown0x1 = *p++;
    m_PacketSize = deserializeU16(p);
    p += 2;
    m_Unknown0x4 = deserializeU32(p);
    p += 4;
    m_PayloadSize = deserializeU16(p);
    p += 2;
    m_Unknown0xA = *p++;
    m_Unknown0xB = *p++;
    m_Unknown0xC = deserializeU16(p);
    p += 2;
    m_Unknown0xE = *p++;
    m_Unknown0xF = *p++;
    m_Unknown0x10 = deserializeU16(p);
    p += 2;
    m_Unknown0x12 = deserializeU16(p);
    p += 2;
    memcpy(m_Payload, p, m_PayloadSize);
    s_DeserializeCount++;
    return nn::Result();
}

// 0x00429A74 (name is ours)
bool nn::pia::common::PacketOld::IsPacketOld(const unsigned char* pBuffer)
{
    return deserializeU32(pBuffer) == MAGIC;
}

// 0x00429A94 | fefates:bytes [tier B]
nn::Result nn::pia::common::PacketOld::SetSignatureSize(unsigned int size)
{
    if (size >= s_DefaultPayloadSize) {
        return RESULT_INVALID_ARGUMENT;
    }
    s_SignatureSize = size;
    return nn::Result();
}

// 0x00429AB8 | fefates:bytes [tier B]
bool nn::pia::common::PacketOld::IsValidSignatureSize(unsigned int size)
{
    return size < s_DefaultPayloadSize;
}

// 0x00429AD4 | fefates:bytes [tier B]
nn::Result nn::pia::common::PacketOld::SetDefaultPayloadSize(unsigned int size)
{
    if (size > DEFAULT_PAYLOAD_SIZE_MAX) {
        return RESULT_INVALID_ARGUMENT;
    }
    s_DefaultPayloadSize = size;
    return nn::Result();
}

// 0x00429AFC (name is ours)
nn::Result nn::pia::common::PacketOld::SetSourceStationAddress(const StationAddress& address)
{
    m_SourceStationAddress = address;
    return nn::Result();
}

// 0x00429B14 (name is ours)
bool nn::pia::common::PacketOld::IsValidDefaultPayloadSize(unsigned int size)
{
    return size <= DEFAULT_PAYLOAD_SIZE_MAX;
}

// 0x00733420 (name is ours)
nn::Result nn::pia::common::PacketOld::Serialize(unsigned char* pBuffer, unsigned int* pSize, unsigned int bufferSize) const
{
    if (!IsValidPointer(pBuffer) || !IsValidPointer(pSize)) {
        return RESULT_INVALID_ARGUMENT;
    }
    if (bufferSize < m_PayloadSize + s_SignatureSize + HEADER_SIZE) {
        return RESULT_INVALID_ARGUMENT;
    }
    serializeU32(pBuffer, MAGIC);
    pBuffer[4] = m_Version;
    pBuffer[5] = m_Unknown0x1;
    unsigned char* p = pBuffer + 6;
    serializeU16(p, m_PacketSize);
    p += 2;
    serializeU32(p, m_Unknown0x4);
    p += 4;
    serializeU16(p, m_PayloadSize);
    p += 2;
    *p++ = m_Unknown0xA;
    *p++ = m_Unknown0xB;
    serializeU16(p, m_Unknown0xC);
    p += 2;
    *p++ = m_Unknown0xE;
    *p++ = m_Unknown0xF;
    serializeU16(p, m_Unknown0x10);
    p += 2;
    serializeU16(p, m_Unknown0x12);
    p += 2;
    memcpy(p, m_Payload, m_PayloadSize);
    SignatureManager::GetInstance()->m_DefaultContext.Append(pBuffer, bufferSize, p - pBuffer + m_PayloadSize);
    *pSize = m_PayloadSize + s_SignatureSize + HEADER_SIZE;
    s_SerializeCount++;
    return nn::Result();
}

} // namespace common
} // namespace pia
} // namespace nn
