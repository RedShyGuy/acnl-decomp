#include "nn/pia/common/common_Packet.h"
#include "nn/nstd/nstd_String.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/pia_Types.h"
#include "pead/peadBitUtil.h"
#include <string.h>

namespace nn {
namespace pia {
namespace common {
// 0x00429000 | fefates:bytes [tier B]
u8* nn::pia::common::Packet::AssignPayload(unsigned int size)
{
    u32 newSize = m_Size + size;
    if (newSize > BUFFER_SIZE_MAX) {
        return nullptr;
    }
    u8* p = reinterpret_cast<u8*>(this) + m_Size;
    m_Size = newSize;
    return p;
}

// 0x0042902C | fefates:bytes [tier B]
void nn::pia::common::Packet::Reset()
{
    m_Magic = MAGIC;
    m_State = STATE_PLAIN;
    m_Unknown0x5 = 0;
    m_SequenceId = 0;
    m_RttTimeStamp = 0;
    m_RttEcho = 0;
    m_Size = HEADER_SIZE;
    m_DestinationStationIndex = STATION_INDEX_UNIDENTIFIED;
    m_DestinationBitmap = 0;
    m_DestinationStationAddress.Clear();
    m_Ttl = 0;
    m_Unknown0x5D5 = false;
    m_SourceStationAddress.Clear();
    m_HasMessages = 0;
}

// 0x00429098 | fefates:bytes [tier B]
nn::Result nn::pia::common::Packet::Decrypt(const nn::pia::common::Crypto::Setting& setting)
{
    if (m_Magic != MAGIC || (m_State != STATE_PLAIN && m_State != STATE_ENCRYPTED)) {
        return RESULT_INVALID_FORMAT;
    }
    if (m_Size - HEADER_SIZE > PAYLOAD_SIZE_MAX) {
        return RESULT_INVALID_FORMAT;
    }
    if (m_State == STATE_PLAIN) {
        return nn::Result();
    }
    if (setting.m_Mode != Crypto::MODE_AES128) {
        return RESULT_INTERNAL_ERROR;
    }
    if (m_State != STATE_ENCRYPTED) {
        return RESULT_INVALID_FORMAT;
    }
    unsigned int payloadSize = m_Size - HEADER_SIZE;
    if (payloadSize == 0) {
        m_State = STATE_PLAIN;
        return nn::Result();
    }
    if (payloadSize % Crypto::GetBlockSize(setting.m_Mode) != 0 || payloadSize > PAYLOAD_SIZE_MAX) {
        return RESULT_INVALID_FORMAT;
    }
    u8 buffer[0x5B0];
    nn::Result result = Crypto::Decrypt(buffer, GetPayload(), payloadSize, setting);
    if (result.IsFailure()) {
        return result;
    }
    nnnstdMemCpy(GetPayload(), buffer, payloadSize);
    m_State = STATE_PLAIN;
    return nn::Result();
}

// 0x0042919C | fefates:bytes [tier B]
nn::Result nn::pia::common::Packet::Encrypt(const nn::pia::common::Crypto::Setting& setting)
{
    if (m_Magic != MAGIC || (m_State != STATE_PLAIN && m_State != STATE_ENCRYPTED)) {
        return RESULT_INVALID_STATE;
    }
    unsigned int payloadSize = m_Size - HEADER_SIZE;
    if (payloadSize > PAYLOAD_SIZE_MAX || m_State != STATE_PLAIN) {
        return RESULT_INVALID_STATE;
    }
    switch (setting.m_Mode) {
    case Crypto::MODE_NONE:
        return nn::Result();
    case Crypto::MODE_AES128:
        break;
    default:
        return RESULT_INTERNAL_ERROR;
    }
    if (payloadSize == 0) {
        return nn::Result();
    }
    size_t blockSize = Crypto::GetBlockSize(setting.m_Mode);
    unsigned int encryptedSize = (payloadSize + blockSize - 1) / blockSize * blockSize;
    if (encryptedSize > PAYLOAD_SIZE_MAX) {
        return RESULT_INVALID_STATE;
    }
    u8 buffer[0x5B0];
    nnnstdMemCpy(buffer, GetPayload(), payloadSize);
    unsigned int paddingSize = encryptedSize - payloadSize;
    if (paddingSize != 0) {
        memset(buffer + payloadSize, 0xFF, paddingSize);
    }
    nn::Result result = Crypto::Encrypt(GetPayload(), buffer, encryptedSize, setting);
    if (result.IsFailure()) {
        return result;
    }
    m_Size += paddingSize;
    m_State = STATE_ENCRYPTED;
    return nn::Result();
}

// 0x004292C4 (name after C++)
Packet& nn::pia::common::Packet::operator=(const Packet& rhs)
{
    return *static_cast<Packet*>(nnnstdMemCpy(this, &rhs, sizeof(Packet)));
}

// 0x004292D0 | fefates:bytes [tier B]
nn::pia::common::Packet::Packet()
{
    Reset();
}

// 0x00429358 | fefates:bytes [tier B]
nn::pia::common::Packet::~Packet()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x00733374 | fefates:bytes [tier B]
int nn::pia::common::Packet::GetPacketNumInNetwork() const
{
    int num = pead::CountOnes(m_DestinationBitmap);
    if (num <= 1) {
        num = 1;
    }
    return num;
}

// 0x0073338C | fefates:bytes [tier B]
bool nn::pia::common::Packet::IsValid() const
{
    if (m_Magic != MAGIC || (m_State != STATE_PLAIN && m_State != STATE_ENCRYPTED)) {
        return false;
    }
    return m_Size - HEADER_SIZE <= PAYLOAD_SIZE_MAX;
}

} // namespace common
} // namespace pia
} // namespace nn
