#include "nn/nex/nex_RelayMessage.h"
#include "nn/nex/nex_ByteStream.h"
#include "nn/nex/nex_MD5.h"
#include "nn/nex/nex_PacketIn.h"
#include "nn/nex/nex_PacketOut.h"

#include <algorithm>
#include <cstring>

namespace nn {
namespace nex {
namespace {
// appends a value little endian (byte by byte) at the end of the buffer
template <typename T>
inline bool AppendLittleEndian(Buffer* pBuffer, T value)
{
    u8 bytes[sizeof(T)];
    for (u32 i = 0; i < sizeof(T); i++) {
        bytes[i] = static_cast<u8>(value >> (i * 8));
    }
    return pBuffer->AppendData(bytes, sizeof(T), static_cast<unsigned>(-1));
}

// skips size bytes of the stream (when there are that many)
inline bool Skip(ByteStream* pStream, u32 size)
{
    u32 position = pStream->m_Position + size;
    if (position > pStream->m_pBuffer->m_ContentSize) {
        return false;
    }
    pStream->m_Position = position;
    return true;
}

// a is a == b or after b (the time stamps wrap around)
inline bool IsNotBefore(u32 a, u32 b)
{
    return a == b || static_cast<s32>(a - b) > 0;
}

// at most this many bytes of the payload of a data message are signed
// 0x00899868 (name is ours)
const u32 MAX_SIGNED_PAYLOAD_SIZE = 50;
} // namespace

// 0x0035FA94 slot 0x00
// 0x0035FA70 (deleting dtor)
nn::nex::RelayMessage::~RelayMessage()
{
    // nothing to do: the key buffer has no resources
}

// 0x0035F090 slot 0x08 | virtual slot, introduced by nn::nex::RelayMessage
bool nn::nex::RelayMessage::CalcSignature(nn::nex::PacketOut* pPacket, nn::nex::PRUDPMessageInterface::SignatureMethod method, const nn::nex::Key* pKey, nn::nex::Stream::Type type, nn::nex::TransportSignatureGenerator* pGenerator, const nn::nex::SignatureBytes* pReceived, nn::nex::SignatureBytes& signature)
{
    // the relay signs in Pack
    return false;
}

// 0x0035F08C slot 0x0C | virtual slot, introduced by nn::nex::RelayMessage
void nn::nex::RelayMessage::CalcSignature(nn::nex::Packet* pPacket, nn::nex::PRUDPMessageInterface::SignatureMethod method, const nn::nex::Key* pKey, nn::nex::Stream::Type type, nn::nex::TransportSignatureGenerator* pGenerator, const nn::nex::SignatureBytes* pReceived, nn::nex::SignatureBytes& signature)
{
    // nothing to do: the relay checks the signature in Unpack
}

// 0x0035F138 slot 0x10 | slot vf_0x10 of nn::nex::RelayMessage
nn::nex::PRUDPMessageInterface::SignatureMethod nn::nex::RelayMessage::DecideSignatureMethod(const nn::nex::Packet* pPacket, nn::nex::Stream::Type type, bool isOutgoing)
{
    return static_cast<SignatureMethod>(0);
}

// 0x0035F130 slot 0x14 | slot vf_0x14 of nn::nex::RelayMessage
int nn::nex::RelayMessage::CalcExpectedSignature(const nn::nex::Packet* pPacket, nn::nex::PRUDPMessageInterface::SignatureMethod method, const nn::nex::Key* pKey, nn::nex::TransportSignatureGenerator* pGenerator, const nn::nex::SignatureBytes* pReceived, nn::nex::SignatureBytes& signature)
{
    return 3;
}

// 0x0035F718 slot 0x18 | fefates:bytes
bool nn::nex::RelayMessage::Unpack(nn::nex::ByteStream* pStream, nn::nex::PacketIn* pPacket, unsigned int* pSize)
{
    if (!UnpackWithoutValidation(pStream, pPacket, pSize, true)) {
        return false;
    }
    SignatureBytes signature;
    signature.m_Size = 0;
    Buffer* pBuffer = pStream->m_pBuffer;
    const u8* pData = pBuffer->m_pData + pBuffer->m_HeaderSize;
    Packet::RelayHeader& header = pPacket->GetRelayHeader();
    CalcSignature(&signature, static_cast<RelayType>(header.m_TypeFlags & Packet::TYPE_MASK), m_Key.m_Data, m_Key.m_Size, pData,
                  pData + HEADER_SIZE, pBuffer->m_ContentSize - HEADER_SIZE);
    if (pPacket->m_PacketSignature.m_Size != signature.m_Size ||
        memcmp(pPacket->m_PacketSignature.m_Data, signature.m_Data, signature.m_Size) != 0) {
        return false;
    }
    if (!CheckTimeStamp(pPacket)) {
        return false;
    }
    if ((header.m_TypeFlags & Packet::TYPE_MASK) == TYPE_PING) {
        return UnpackPing(pPacket);
    }
    return true;
}

// 0x0035EE38 slot 0x1C | slot vf_0x1C of nn::nex::RelayMessage
bool nn::nex::RelayMessage::FastUnpack(nn::nex::ByteStream* pStream, nn::nex::PacketIn* pPacket, unsigned int* pSize)
{
    return false;
}

// 0x0035F3B4 slot 0x20 | fefates:bytes
bool nn::nex::RelayMessage::Pack(nn::nex::PacketOut* pPacket, nn::nex::ByteStream* pStream)
{
    Packet::RelayHeader& header = pPacket->GetRelayHeader();
    if ((header.m_TypeFlags & Packet::TYPE_MASK) == TYPE_PING && !PackPing(pPacket)) {
        return false;
    }
    header.m_TimeStamp = GetTimeStamp();
    return Pack(pPacket, pStream, m_Key.m_Data, m_Key.m_Size);
}

// 0x0035EE40 | fefates:bytes [tier B]
bool nn::nex::RelayMessage::UnpackPing(nn::nex::PacketIn* pPacket)
{
    Buffer* pPayload = pPacket->m_pPayload;
    if (pPayload->m_ContentSize < PING_SIZE) {
        return false;
    }
    ByteStream stream(pPayload);
    Packet::RelayHeader& header = pPacket->GetRelayHeader();
    u8 value0x1C;
    if (!stream.ExtractPrimitive(&value0x1C)) {
        return false;
    }
    header.m_Unknown0x1C = value0x1C;
    u8 value0x1D;
    if (!stream.ExtractPrimitive(&value0x1D)) {
        return false;
    }
    header.m_Unknown0x1D = value0x1D;
    if (!Skip(&stream, 2)) {
        return false;
    }
    u32 value0x20;
    if (!stream.ExtractPrimitive(&value0x20)) {
        return false;
    }
    header.m_Unknown0x20 = value0x20;
    u32 value0x24;
    if (!stream.ExtractPrimitive(&value0x24)) {
        return false;
    }
    header.m_Unknown0x24 = value0x24;
    pPacket->m_pPayload->SetHeaderSize(PING_SIZE);
    return true;
}

// 0x0035EF80 | fefates:bytes [tier B]
void nn::nex::RelayMessage::CalcSignature(nn::nex::SignatureBytes* pSignature, nn::nex::RelayType type, const unsigned char* pKey, unsigned int keySize, const unsigned char* pHeader, const unsigned char* pPayload, unsigned int payloadSize)
{
    MD5 md5;
    md5.Update(pKey, keySize);
    md5.Update(pHeader, SIGNED_HEADER_SIZE);
    if (pPayload != nullptr && payloadSize != 0) {
        if (type != TYPE_DATA) {
            md5.Update(pPayload, payloadSize);
        } else {
            md5.Update(pPayload, std::min(payloadSize, MAX_SIGNED_PAYLOAD_SIZE));
        }
    }
    memcpy(pSignature->m_Data, md5.GetDigest(), SIGNATURE_SIZE);
    pSignature->m_Size = SIGNATURE_SIZE;
}

// 0x0035F098
bool nn::nex::RelayMessage::CheckTimeStamp(nn::nex::PacketIn* pPacket)
{
    Packet::RelayHeader& header = pPacket->GetRelayHeader();
    if (header.m_Unknown0x10 == static_cast<u32>(-2) || header.m_Unknown0x10 == static_cast<u32>(-3)) {
        return true;
    }
    u32 now = GetTimeStamp();
    u32 timeStamp = header.m_TimeStamp;
    bool isValid = IsNotBefore(timeStamp, now - TIME_STAMP_TOLERANCE) && IsNotBefore(now + TIME_STAMP_TOLERANCE, timeStamp);
    if (!isValid) {
        return false;
    }
    return true;
}

// 0x0035F140 | fefates:bytes [tier B]
bool nn::nex::RelayMessage::UnpackWithoutValidation(nn::nex::ByteStream* pStream, nn::nex::PacketIn* pPacket, unsigned int* pSize, bool copyPayload)
{
    if (pStream->m_pBuffer->m_ContentSize == 0) {
        return false;
    }
    u8 magic = 0;
    u8 version = 0;
    u32 value0x08 = 0;
    u16 value0x0C = 0;
    u16 value0x0E = 0;
    u32 value0x14 = 0;
    u16 typeFlags = 0;
    u32 value0x04 = 0;
    u32 value0x10 = 0;
    u32 timeStamp = 0;
    Packet::RelayHeader& header = pPacket->GetRelayHeader();
    if (!pStream->ExtractPrimitive(&magic)) {
        return false;
    }
    if (magic != MAGIC_0) {
        return false;
    }
    if (!pStream->ExtractPrimitive(&version)) {
        return false;
    }
    pPacket->m_MessageVersion = version;
    if (!pStream->ExtractPrimitive(&typeFlags)) {
        return false;
    }
    header.m_TypeFlags = typeFlags;
    if (!pStream->ExtractPrimitive(&value0x04)) {
        return false;
    }
    header.m_Unknown0x04 = value0x04;
    if (!pStream->ExtractPrimitive(&value0x08)) {
        return false;
    }
    header.m_Unknown0x08 = value0x08;
    if (!pStream->ExtractPrimitive(&value0x0C)) {
        return false;
    }
    header.m_Unknown0x0C = value0x0C;
    if (!pStream->ExtractPrimitive(&value0x0E)) {
        return false;
    }
    header.m_Unknown0x0E = value0x0E;
    if (!pStream->ExtractPrimitive(&value0x10)) {
        return false;
    }
    header.m_Unknown0x10 = value0x10 ^ MASK_0x10;
    if (!pStream->ExtractPrimitive(&value0x14)) {
        return false;
    }
    header.m_Unknown0x14 = value0x14 ^ MASK_0x14;
    if (!pStream->ExtractPrimitive(&timeStamp)) {
        return false;
    }
    header.m_TimeStamp = timeStamp;
    if (!pStream->ExtractRaw(pPacket->m_PacketSignature.m_Data, SIGNATURE_SIZE)) {
        return false;
    }
    pPacket->m_PacketSignature.m_Size = SIGNATURE_SIZE;
    if (pStream->m_Position != HEADER_SIZE) {
        return false;
    }
    pPacket->m_SourcePort = PORT;
    pPacket->m_DestinationPort = PORT;
    Buffer* pBuffer = pStream->m_pBuffer;
    pPacket->m_pPayload->m_ContentSize = 0;
    pPacket->m_pPayload->m_HeaderSize = 0;
    u16 payloadSize = pBuffer->m_ContentSize - HEADER_SIZE;
    if (payloadSize != 0 && copyPayload) {
        // the payload is the rest of the message
        pPacket->m_pPayload->m_ContentSize = 0;
        pPacket->m_pPayload->m_HeaderSize = 0;
        u16 size = pStream->m_pBuffer->m_ContentSize - HEADER_SIZE;
        if (size != 0 &&
            !pPacket->m_pPayload->AppendData(pStream->m_pBuffer->m_pData + pStream->m_pBuffer->m_HeaderSize + pStream->m_Position,
                                             size, static_cast<unsigned>(-1))) {
            return false;
        }
        Skip(pStream, payloadSize);
    }
    if (pSize != nullptr) {
        *pSize = pBuffer->m_ContentSize;
    }
    return true;
}

// 0x0035F434 | fefates:bytes [tier B]
bool nn::nex::RelayMessage::Pack(nn::nex::PacketOut* pPacket, nn::nex::ByteStream* pStream, const unsigned char* pKey, unsigned int keySize)
{
    pStream->m_pBuffer->m_ContentSize = 0;
    pStream->m_pBuffer->m_HeaderSize = 0;
    pStream->m_IsError = false;
    pStream->m_Position = 0;
    u8 byte = MAGIC_0;
    pStream->m_pBuffer->AppendData(&byte, sizeof(byte), static_cast<unsigned>(-1));
    if (pPacket->m_MessageVersion != MAGIC_1) {
        return false;
    }
    byte = pPacket->m_MessageVersion;
    pStream->m_pBuffer->AppendData(&byte, sizeof(byte), static_cast<unsigned>(-1));
    Packet::RelayHeader& header = pPacket->GetRelayHeader();
    AppendLittleEndian(pStream->m_pBuffer, header.m_TypeFlags);
    AppendLittleEndian(pStream->m_pBuffer, header.m_Unknown0x04);
    AppendLittleEndian(pStream->m_pBuffer, header.m_Unknown0x08);
    AppendLittleEndian(pStream->m_pBuffer, header.m_Unknown0x0C);
    AppendLittleEndian(pStream->m_pBuffer, header.m_Unknown0x0E);
    AppendLittleEndian(pStream->m_pBuffer, header.m_Unknown0x10 ^ MASK_0x10);
    AppendLittleEndian(pStream->m_pBuffer, header.m_Unknown0x14 ^ MASK_0x14);
    AppendLittleEndian(pStream->m_pBuffer, header.m_TimeStamp);
    SignatureBytes signature;
    signature.m_Size = 0;
    Buffer* pPayload = pPacket->m_pPayload;
    Buffer* pBuffer = pStream->m_pBuffer;
    if (pPayload != nullptr) {
        CalcSignature(&signature, static_cast<RelayType>(header.m_TypeFlags & Packet::TYPE_MASK), pKey, keySize,
                      pBuffer->m_pData + pBuffer->m_HeaderSize, pPayload->m_pData + pPayload->m_HeaderSize, pPayload->m_ContentSize);
    } else {
        CalcSignature(&signature, static_cast<RelayType>(header.m_TypeFlags & Packet::TYPE_MASK), pKey, keySize,
                      pBuffer->m_pData + pBuffer->m_HeaderSize, nullptr, 0);
    }
    if (!pStream->m_pBuffer->AppendData(signature.m_Data, signature.m_Size, static_cast<unsigned>(-1))) {
        return false;
    }
    pPayload = pPacket->m_pPayload;
    if (pPayload != nullptr) {
        pStream->m_pBuffer->AppendData(pPayload->m_pData + pPayload->m_HeaderSize, pPayload->m_ContentSize, static_cast<unsigned>(-1));
    }
    return true;
}

// 0x0035F7F8 | fefates:bytes [tier B]
bool nn::nex::RelayMessage::PackPing(nn::nex::PacketOut* pPacket)
{
    // makes room for the ping data before the payload
    Buffer* pPayload = pPacket->m_pPayload;
    if (pPayload != nullptr && pPayload->m_ContentSize != 0) {
        if (pPayload->m_HeaderSize == PING_SIZE) {
            pPayload->SetHeaderSize(0);
        } else {
            Buffer* pBuffer = new Buffer(pPacket->m_pPayload->m_ContentSize + PING_SIZE);
            pBuffer->m_ContentSize = PING_SIZE;
            pBuffer->AppendData(pPacket->m_pPayload->m_pData + pPacket->m_pPayload->m_HeaderSize, pPacket->m_pPayload->m_ContentSize,
                                static_cast<unsigned>(-1));
            if (pPacket->m_pPayload != nullptr) {
                pPacket->m_pPayload->ReleaseRef();
            }
            pPacket->m_pPayload = pBuffer;
        }
    } else {
        Buffer* pBuffer = new Buffer(PING_SIZE);
        pBuffer->m_ContentSize = PING_SIZE;
        if (pPacket->m_pPayload != nullptr) {
            pPacket->m_pPayload->ReleaseRef();
        }
        pPacket->m_pPayload = pBuffer;
    }
    // writes the ping data over the first bytes
    u32 size = pPacket->m_pPayload->m_ContentSize;
    pPacket->m_pPayload->m_ContentSize = 0;
    {
        ByteStream stream(pPacket->m_pPayload);
        Packet::RelayHeader& header = pPacket->GetRelayHeader();
        u8 byte = header.m_Unknown0x1C;
        stream.m_pBuffer->AppendData(&byte, sizeof(byte), static_cast<unsigned>(-1));
        byte = header.m_Unknown0x1D;
        stream.m_pBuffer->AppendData(&byte, sizeof(byte), static_cast<unsigned>(-1));
        for (int i = 0; i < 2; i++) {
            u8 zero = 0;
            stream.m_pBuffer->AppendData(&zero, sizeof(zero), static_cast<unsigned>(-1));
        }
        AppendLittleEndian(stream.m_pBuffer, header.m_Unknown0x20);
        AppendLittleEndian(stream.m_pBuffer, header.m_Unknown0x24);
        pPacket->m_pPayload->m_ContentSize = size;
    }
    return true;
}

} // namespace nex
} // namespace nn
