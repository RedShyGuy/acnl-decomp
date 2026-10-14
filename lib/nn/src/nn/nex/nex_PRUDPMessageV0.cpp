#include "nn/nex/nex_PRUDPMessageV0.h"
#include "nn/nex/nex_ByteStream.h"
#include "nn/nex/nex_Key.h"
#include "nn/nex/nex_LiteBuffer.h"
#include "nn/nex/nex_PacketIn.h"
#include "nn/nex/nex_PacketOut.h"
#include "nn/nex/nex_SignatureBytes.h"
#include "nn/nex/nex_StreamBundling.h"
#include "nn/nex/nex_TransportSignatureGenerator.h"

namespace nn {
namespace nex {
namespace {
// (inline) the position moves by size; false behind the end
inline bool Skip(ByteStream* pStream, u32 size)
{
    u32 position = pStream->m_Position + size;
    if (position > pStream->m_pBuffer->m_ContentSize) {
        return false;
    }
    pStream->m_Position = position;
    return true;
}
} // namespace

// 0x0037190C | fefates:bytes
bool nn::nex::PRUDPMessageV0::FastUnpack(nn::nex::ByteStream* pStream, nn::nex::PacketIn* pPacket, unsigned int* pSize)
{
    u32 start = pStream->m_Position;
    if (static_cast<s32>(pStream->m_pBuffer->m_ContentSize) <= static_cast<s32>(start)) {
        return false;
    }
    if (!pStream->ExtractPrimitive(&pPacket->m_SourcePort)) {
        return false;
    }
    if (!pStream->ExtractPrimitive(&pPacket->m_DestinationPort)) {
        return false;
    }
    if (!pStream->ExtractPrimitive(&pPacket->m_TypeFlags)) {
        return false;
    }
    // the session id and the packet signature
    if (!Skip(pStream, sizeof(u8)) || !Skip(pStream, SIGNATURE_SIZE)) {
        return false;
    }
    u16 sequenceId = 0;
    if (!pStream->ExtractPrimitive(&sequenceId)) {
        return false;
    }
    pPacket->m_SequenceId.m_Value = sequenceId;
    u32 type = pPacket->GetType();
    if ((type == Packet::TYPE_SYN || type == Packet::TYPE_CONNECT) && !Skip(pStream, SIGNATURE_SIZE)) {
        return false;
    }
    if (pPacket->GetType() == Packet::TYPE_DATA && !Skip(pStream, sizeof(u8))) {
        return false;
    }
    u16 payloadSize = 0;
    u32 size;
    if ((pPacket->m_TypeFlags & FLAG_HAS_SIZE) != 0) {
        if (!pStream->ExtractPrimitive(&payloadSize)) {
            return false;
        }
        size = pStream->m_Position + payloadSize - start + 1;
    } else {
        size = pStream->m_pBuffer->m_ContentSize;
        payloadSize = size - pStream->m_Position - 1;
    }
    if (pPacket->GetStreamType() > MAX_STREAM_TYPE) {
        return false;
    }
    // the payload and the checksum
    if (!Skip(pStream, payloadSize + 1)) {
        return false;
    }
    if (pSize != nullptr) {
        *pSize = size;
    }
    return true;
}

// 0x00371AD0 slot 0x0C | fefates:bytes
void nn::nex::PRUDPMessageV0::CalcSignature(nn::nex::Packet* pPacket, nn::nex::PRUDPMessageInterface::SignatureMethod method, const nn::nex::Key* pKey, nn::nex::Stream::Type type, nn::nex::TransportSignatureGenerator* pGenerator, const nn::nex::SignatureBytes*, nn::nex::SignatureBytes& signature)
{
    CalcSignatureHelper(pPacket, method, pKey, type, pGenerator, signature);
}

// 0x00371AF8 slot 0x08 | fefates:bytes
bool nn::nex::PRUDPMessageV0::CalcSignature(nn::nex::PacketOut* pPacket, nn::nex::PRUDPMessageInterface::SignatureMethod method, const nn::nex::Key* pKey, nn::nex::Stream::Type type, nn::nex::TransportSignatureGenerator* pGenerator, const nn::nex::SignatureBytes*, nn::nex::SignatureBytes& signature)
{
    CalcSignatureHelper(pPacket, method, pKey, type, pGenerator, signature);
    return true;
}

// 0x00371B24 | fefates:bytes-fuzzy [tier B]
void nn::nex::PRUDPMessageV0::CalcSignatureHelper(const nn::nex::Packet* pPacket, nn::nex::PRUDPMessageInterface::SignatureMethod method, const nn::nex::Key* pKey, nn::nex::Stream::Type type, nn::nex::TransportSignatureGenerator* pGenerator, nn::nex::SignatureBytes& signature)
{
    signature.m_Size = SIGNATURE_SIZE;
    u32 value = 0;
    switch (method) {
    case SIGNATURE_METHOD_ADDRESS:
        if (pGenerator != nullptr) {
            // the address and the port in host order
            struct
            {
                u32 m_Address;
                u16 m_Port;
            } address;
            address.m_Address = __builtin_bswap32(pPacket->m_Address.m_SockAddr.m_Address);
            address.m_Port = __builtin_bswap16(pPacket->m_Address.m_SockAddr.m_Port);
            value = pGenerator->m_pChecksumAlgorithm->ComputeChecksumForTransport(reinterpret_cast<const u8*>(&address), 6);
        }
        break;
    case SIGNATURE_METHOD_PAYLOAD: {
        KeyedChecksumAlgorithm* pAlgorithm = Stream::GetSettings(type)->m_pChecksumAlgorithm;
        if (pAlgorithm == nullptr) {
            value = SIGNATURE_NO_ALGORITHM;
            break;
        }
        Buffer* pPayload = pPacket->m_pPayload;
        if (pPayload == nullptr || pPayload->m_ContentSize == 0) {
            value = SIGNATURE_NO_DATA;
            break;
        }
        const u8* data[2] = {nullptr, pPayload->m_pData + pPayload->m_HeaderSize};
        u32 sizes[2] = {0, pPayload->m_ContentSize};
        value = pAlgorithm->ComputeChecksumForTransportArray(data, sizes, 2);
        break;
    }
    case SIGNATURE_METHOD_KEY_SEQUENCE_PAYLOAD: {
        u16 sequenceId = pPacket->m_SequenceId.m_Value;
        u8 fragmentId = pPacket->m_Unknown0x64;
        Buffer buffer(pKey->GetLength() + 3);
        buffer.AppendData(pKey->GetContentPtr(), pKey->GetLength(), static_cast<unsigned>(-1));
        buffer.AppendData(&sequenceId, sizeof(sequenceId), static_cast<unsigned>(-1));
        buffer.AppendData(&fragmentId, sizeof(fragmentId), static_cast<unsigned>(-1));
        const u8* pData = buffer.m_pData + buffer.m_HeaderSize;
        u32 size = buffer.m_ContentSize;
        KeyedChecksumAlgorithm* pAlgorithm = Stream::GetSettings(type)->m_pChecksumAlgorithm;
        if (pAlgorithm == nullptr) {
            value = SIGNATURE_NO_ALGORITHM;
            break;
        }
        Buffer* pPayload = pPacket->m_pPayload;
        if (pPayload == nullptr || pPayload->m_ContentSize == 0) {
            if (size == 0) {
                value = SIGNATURE_NO_DATA;
                break;
            }
            if (pPayload == nullptr) {
                value = pAlgorithm->ComputeChecksumForTransport(pData, size);
                break;
            }
        }
        const u8* data[2] = {pData, pPayload->m_pData + pPayload->m_HeaderSize};
        u32 sizes[2] = {size, pPayload->m_ContentSize};
        value = pAlgorithm->ComputeChecksumForTransportArray(data, sizes, 2);
        break;
    }
    case SIGNATURE_METHOD_HEADER: {
        u16 payloadSize = 0;
        u16 sequenceId = pPacket->m_SequenceId.m_Value;
        u8 sessionId = pPacket->m_SessionId;
        u8 sourcePort = pPacket->m_SourcePort;
        u8 destinationPort = pPacket->m_DestinationPort;
        if (pPacket->m_pPayload != nullptr) {
            payloadSize = pPacket->m_pPayload->m_ContentSize;
        }
        LiteBuffer<8> header;
        header.AppendData(&sequenceId, sizeof(sequenceId), header.m_Size);
        header.AppendData(&sessionId, sizeof(sessionId), header.m_Size);
        header.AppendData(&sourcePort, sizeof(sourcePort), header.m_Size);
        header.AppendData(&destinationPort, sizeof(destinationPort), header.m_Size);
        header.AppendData(&payloadSize, sizeof(payloadSize), header.m_Size);
        KeyedChecksumAlgorithm* pAlgorithm = Stream::GetSettings(type)->m_pChecksumAlgorithm;
        if (pAlgorithm == nullptr) {
            value = SIGNATURE_NO_ALGORITHM;
            break;
        }
        value = pAlgorithm->ComputeChecksumForTransport(header.m_Data, header.m_Size);
        break;
    }
    default:
        break;
    }
    signature.m_Data[0] = value;
    signature.m_Data[1] = value >> 8;
    signature.m_Data[2] = value >> 16;
    signature.m_Data[3] = value >> 24;
}

// 0x00371E88 | fefates:bytes
int nn::nex::PRUDPMessageV0::CalcExpectedSignature(const nn::nex::Packet* pPacket, nn::nex::PRUDPMessageInterface::SignatureMethod method, const nn::nex::Key* pKey, nn::nex::TransportSignatureGenerator* pGenerator, const nn::nex::SignatureBytes*, nn::nex::SignatureBytes& signature)
{
    // (no signature to check)
    if (method == SIGNATURE_METHOD_UNSIGNED) {
        return 3;
    }
    CalcSignatureHelper(pPacket, method, pKey, static_cast<Stream::Type>(pPacket->GetStreamType()), pGenerator, signature);
    return 0;
}

// 0x00371EC8 slot 0x10
nn::nex::PRUDPMessageInterface::SignatureMethod nn::nex::PRUDPMessageV0::DecideSignatureMethod(const nn::nex::Packet* pPacket, nn::nex::Stream::Type type, bool isOutgoing)
{
    if (Stream::GetSettings(type)->m_pChecksumAlgorithm == nullptr || pPacket->m_Unknown0x88 != 0) {
        return SIGNATURE_METHOD_ADDRESS;
    }
    if (type == 10 || type == 2) {
        u32 packetType = pPacket->GetType();
        if (isOutgoing) {
            if (packetType == Packet::TYPE_DATA || packetType == Packet::TYPE_DISCONNECT) {
                return SIGNATURE_METHOD_KEY_SEQUENCE_PAYLOAD;
            }
        } else if (packetType == Packet::TYPE_DISCONNECT && (pPacket->m_TypeFlags & 0x10) != 0) {
            return SIGNATURE_METHOD_UNSIGNED;
        }
    } else {
        u32 packetType = pPacket->GetType();
        if (packetType == Packet::TYPE_DATA || packetType == 5) {
            return SIGNATURE_METHOD_PAYLOAD;
        }
    }
    return SIGNATURE_METHOD_ADDRESS;
}

// 0x00371F64 | fefates:callseq
bool nn::nex::PRUDPMessageV0::Pack(nn::nex::PacketOut* pPacket, nn::nex::ByteStream* pStream)
{
    u32 type = pPacket->GetType();
    if (type == Packet::TYPE_RAW) {
        Buffer* pPayload = pPacket->m_pPayload;
        pStream->m_pBuffer->AppendData(pPayload->m_pData + pPayload->m_HeaderSize, pPayload->m_ContentSize, static_cast<unsigned>(-1));
        pPacket->m_Unknown0x134 = pStream->m_pBuffer->m_ContentSize;
        return true;
    }
    // the payload size is written when the packet is bundled
    if (type != Packet::TYPE_DISCONNECT && Stream::GetSettings(static_cast<Stream::Type>(pPacket->GetStreamType()))->m_Bundling.m_IsEnabled != 0) {
        pPacket->m_TypeFlags |= FLAG_HAS_SIZE;
    } else if (g_Unknown0x96C92B && (pPacket->m_TypeFlags & Packet::TYPE_MASK) == Packet::TYPE_DATA && pPacket->GetStreamType() == 1 &&
               (g_Unknown0x96C92D || (pPacket->m_TypeFlags & 0x30) != 0)) {
        bool isBundled;
        if (g_Unknown0x96C92C && (pPacket->m_TypeFlags & 0x10) == 0) {
            isBundled = true;
        } else {
            isBundled = (pPacket->m_TypeFlags & 0x30) == 0;
        }
        if (isBundled && !pPacket->m_Unknown0x131) {
            pPacket->m_TypeFlags |= FLAG_HAS_SIZE;
        }
    }
    u8 byte = pPacket->m_SourcePort;
    pStream->m_pBuffer->AppendData(&byte, sizeof(byte), static_cast<unsigned>(-1));
    byte = pPacket->m_DestinationPort;
    pStream->m_pBuffer->AppendData(&byte, sizeof(byte), static_cast<unsigned>(-1));
    if (!pStream->m_pBuffer->AppendData(&pPacket->m_TypeFlags, sizeof(u16), static_cast<unsigned>(-1))) {
        return false;
    }
    if (!pStream->m_pBuffer->AppendData(&pPacket->m_SessionId, sizeof(u8), static_cast<unsigned>(-1))) {
        return false;
    }
    if (pPacket->m_PacketSignature.m_Size != 0) {
        if (!pStream->m_pBuffer->AppendData(pPacket->m_PacketSignature.m_Data, pPacket->m_PacketSignature.m_Size, static_cast<unsigned>(-1))) {
            return false;
        }
    } else {
        for (u32 i = 0; i < SIGNATURE_SIZE; i++) {
            u8 zero = 0;
            pStream->m_pBuffer->AppendData(&zero, sizeof(zero), static_cast<unsigned>(-1));
        }
    }
    u16 sequenceId = pPacket->m_SequenceId.m_Value;
    pStream->m_pBuffer->AppendData(&sequenceId, sizeof(sequenceId), static_cast<unsigned>(-1));
    type = pPacket->GetType();
    if (type == Packet::TYPE_SYN || type == Packet::TYPE_CONNECT) {
        if (pPacket->m_Signature.m_Size != 0) {
            if (!pStream->m_pBuffer->AppendData(pPacket->m_Signature.m_Data, pPacket->m_Signature.m_Size, static_cast<unsigned>(-1))) {
                return false;
            }
        } else {
            for (u32 i = 0; i < SIGNATURE_SIZE; i++) {
                u8 zero = 0;
                pStream->m_pBuffer->AppendData(&zero, sizeof(zero), static_cast<unsigned>(-1));
            }
        }
    }
    if (pPacket->GetType() == Packet::TYPE_DATA) {
        byte = pPacket->m_Unknown0x64;
        pStream->m_pBuffer->AppendData(&byte, sizeof(byte), static_cast<unsigned>(-1));
    }
    Stream::Type streamType = static_cast<Stream::Type>(pPacket->GetStreamType());
    Buffer* pMessage = pStream->m_pBuffer;
    if (pPacket->m_pPayload != nullptr) {
        // the payload compressed and encrypted with the algorithms of the stream type
        CompressionAlgorithm* pCompression = Stream::GetSettings(streamType)->m_pCompressionAlgorithm;
        Buffer compressed(pPacket->m_pPayload->m_ContentSize + 64);
        Buffer* pData;
        if (pCompression == nullptr) {
            pData = pPacket->m_pPayload;
        } else {
            pCompression->Compress(*pPacket->m_pPayload, &compressed);
            pData = &compressed;
        }
        EncryptionAlgorithm* pEncryption = Stream::GetSettings(streamType)->m_pEncryptionAlgorithm;
        Buffer encrypted(pData->m_ContentSize);
        if (pEncryption != nullptr) {
            pEncryption->Encrypt(*pData, &encrypted);
            pData = &encrypted;
        }
        if ((pPacket->m_TypeFlags & FLAG_HAS_SIZE) != 0) {
            u16 size = pData->m_ContentSize;
            pStream->m_pBuffer->AppendData(&size, sizeof(size), static_cast<unsigned>(-1));
        }
        pStream->m_pBuffer->AppendData(pData->m_pData + pData->m_HeaderSize, pData->m_ContentSize, static_cast<unsigned>(-1));
    } else if ((pPacket->m_TypeFlags & FLAG_HAS_SIZE) != 0) {
        u16 size = 0;
        pStream->m_pBuffer->AppendData(&size, sizeof(size), static_cast<unsigned>(-1));
    }
    u8 checksum = pMessage->ComputeCheckSum(pMessage->m_ContentSize, Stream::GetSettings(streamType)->m_KeyByteSum);
    pMessage->AppendData(&checksum, sizeof(checksum), static_cast<unsigned>(-1));
    pPacket->m_Unknown0x134 = pStream->m_pBuffer->m_ContentSize;
    return true;
}

// 0x003723B8 | fefates:callseq
bool nn::nex::PRUDPMessageV0::Unpack(nn::nex::ByteStream* pStream, nn::nex::PacketIn* pPacket, unsigned int* pSize)
{
    bool isUnpacked = false;
    if (pStream->m_pBuffer->m_ContentSize == 0) {
        return isUnpacked;
    }
    if (!pStream->ExtractPrimitive(&pPacket->m_SourcePort)) {
        return false;
    }
    if (!pStream->ExtractPrimitive(&pPacket->m_DestinationPort)) {
        return false;
    }
    if (!pStream->ExtractPrimitive(&pPacket->m_TypeFlags)) {
        return false;
    }
    if (!pStream->ExtractPrimitive(&pPacket->m_SessionId)) {
        return false;
    }
    if (!pStream->ExtractRaw(pPacket->m_PacketSignature.m_Data, SIGNATURE_SIZE)) {
        return false;
    }
    pPacket->m_PacketSignature.m_Size = SIGNATURE_SIZE;
    u16 sequenceId = 0;
    if (!pStream->ExtractPrimitive(&sequenceId)) {
        return false;
    }
    pPacket->m_SequenceId.m_Value = sequenceId;
    u32 type = pPacket->GetType();
    if (type == Packet::TYPE_SYN || type == Packet::TYPE_CONNECT) {
        if (!pStream->ExtractRaw(pPacket->m_Signature.m_Data, SIGNATURE_SIZE)) {
            return false;
        }
        pPacket->m_Signature.m_Size = SIGNATURE_SIZE;
    }
    if (pPacket->GetType() == Packet::TYPE_DATA && !pStream->ExtractPrimitive(&pPacket->m_Unknown0x64)) {
        return false;
    }
    // the content of the buffer is limited to the message (a bundled packet has its size)
    Buffer* pBuffer = pStream->m_pBuffer;
    u32 contentSize = pBuffer->m_ContentSize;
    u32 end = contentSize;
    if ((pPacket->m_TypeFlags & FLAG_HAS_SIZE) != 0) {
        u16 payloadSize = 0;
        if (!pStream->ExtractPrimitive(&payloadSize)) {
            return false;
        }
        end = pStream->m_Position + payloadSize + 1;
        if (end > contentSize) {
            return false;
        }
    }
    pBuffer->m_ContentSize = end;
    Stream::Type streamType = static_cast<Stream::Type>(pPacket->GetStreamType());
    if (streamType > MAX_STREAM_TYPE) {
        return false;
    }
    u8 key = Stream::GetSettings(streamType)->m_KeyByteSum;
    u32 size = pBuffer->m_ContentSize;
    if (size != 0) {
        u8 checksum;
        memcpy(&checksum, pBuffer->m_pData + pBuffer->m_HeaderSize + size - 1, sizeof(checksum));
        if (checksum == pBuffer->ComputeCheckSum(size - 1, key)) {
            pBuffer->m_ContentSize--;
            Buffer* pPayload = pPacket->m_pPayload;
            pPayload->m_ContentSize = 0;
            pPayload->m_HeaderSize = 0;
            if (pStream->m_Position > pBuffer->m_ContentSize) {
                return false;
            }
            if (pPacket->m_pPayload->AppendData(pBuffer->m_pData + pBuffer->m_HeaderSize + pStream->m_Position, pBuffer->m_ContentSize - pStream->m_Position, static_cast<unsigned>(-1))) {
                isUnpacked = true;
                if (pPacket->m_pPayload->m_ContentSize != 0) {
                    EncryptionAlgorithm* pEncryption = Stream::GetSettings(streamType)->m_pEncryptionAlgorithm;
                    if (pEncryption != nullptr) {
                        pEncryption->Decrypt(pPacket->m_pPayload);
                    }
                    CompressionAlgorithm* pCompression = Stream::GetSettings(streamType)->m_pCompressionAlgorithm;
                    if (pCompression != nullptr) {
                        Buffer* pData = pPacket->m_pPayload;
                        int result;
                        {
                            ScopedCS lock(pCompression->m_Lock);
                            Buffer decompressed(2000);
                            result = pCompression->DecompressImpl(*pData, &decompressed);
                            *pData = decompressed;
                        }
                        if (result != 0) {
                            return false;
                        }
                    }
                }
            }
        }
    }
    pPacket->m_MessageVersion = 0;
    pBuffer->m_ContentSize = contentSize;
    if (pSize != nullptr) {
        *pSize = end;
    }
    return isUnpacked;
}

// 0x0037270C slot 0x00
// 0x003726F4 (deleting dtor)
nn::nex::PRUDPMessageV0::~PRUDPMessageV0()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
