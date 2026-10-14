#include "nn/nex/nex_PRUDPMessageV1.h"
#include "nn/nex/nex_ByteStream.h"
#include "nn/nex/nex_Key.h"
#include "nn/nex/nex_LiteByteStream.h"
#include "nn/nex/nex_PacketIn.h"
#include "nn/nex/nex_PacketOut.h"
#include "nn/nex/nex_Platform.h"
#include "nn/nex/nex_SignatureBytes.h"
#include <cstring>

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

// 0x00372710 | fefates:bytes
bool nn::nex::PRUDPMessageV1::FastUnpack(nn::nex::ByteStream* pStream, nn::nex::PacketIn* pPacket, unsigned int* pSize)
{
    u32 start = pStream->m_Position;
    if (static_cast<s32>(pStream->m_pBuffer->m_ContentSize) <= static_cast<s32>(start)) {
        return false;
    }
    u32 optionsSize = 0;
    u32 payloadSize = 0;
    if (!HeaderParse(pStream, pPacket, optionsSize, payloadSize)) {
        return false;
    }
    if (!Skip(pStream, SIGNATURE_SIZE) || !Skip(pStream, optionsSize)) {
        return false;
    }
    u32 end = pStream->m_Position + payloadSize;
    if (end > pStream->m_pBuffer->m_ContentSize) {
        return false;
    }
    pStream->m_Position = end;
    if (pSize != nullptr) {
        *pSize = end - start;
    }
    return true;
}

// 0x003727CC | fefates:bytes [tier B]
bool nn::nex::PRUDPMessageV1::HeaderParse(nn::nex::ByteStream* pStream, nn::nex::PacketIn* pPacket, unsigned int& optionsSize, unsigned int& payloadSize)
{
    // the magic
    if (!Skip(pStream, sizeof(MAGIC_0) + sizeof(MAGIC_1))) {
        return false;
    }
    if (pPacket->m_IsHeaderV1Valid) {
        pPacket->m_IsHeaderV1Valid = true;
        if (!Skip(pStream, sizeof(Packet::HeaderV1))) {
            return false;
        }
        optionsSize = pPacket->m_HeaderV1.m_OptionsSize;
        payloadSize = pPacket->m_HeaderV1.m_PayloadSize;
        return true;
    }
    if (!pStream->ExtractRaw(reinterpret_cast<u8*>(&pPacket->m_HeaderV1), sizeof(Packet::HeaderV1))) {
        return false;
    }
    pPacket->m_MessageVersion = VERSION;
    optionsSize = pPacket->m_HeaderV1.m_OptionsSize;
    payloadSize = pPacket->m_HeaderV1.m_PayloadSize;
    pPacket->m_SourcePort = pPacket->m_HeaderV1.m_SourcePort;
    pPacket->m_DestinationPort = pPacket->m_HeaderV1.m_DestinationPort;
    pPacket->m_TypeFlags = pPacket->m_HeaderV1.m_TypeFlags;
    pPacket->m_SessionId = pPacket->m_HeaderV1.m_SessionId;
    pPacket->m_SubstreamId = pPacket->m_HeaderV1.m_SubstreamId;
    pPacket->m_SequenceId.m_Value = pPacket->m_HeaderV1.m_SequenceId;
    return true;
}

// 0x003728AC | fefates:bytes [tier B]
bool nn::nex::PRUDPMessageV1::OptionUnpack(nn::nex::ByteStream* pStream, nn::nex::PacketIn* pPacket, unsigned int size)
{
    if (size >= MAX_OPTIONS_SIZE) {
        return false;
    }
    u32 start = pStream->m_Position;
    u32 end = start + size;
    if (pStream->m_pBuffer->m_ContentSize < end) {
        return false;
    }
    while (pStream->m_Position < end) {
        u8 id = 0;
        u8 length = 0;
        if (!pStream->ExtractPrimitive(&id)) {
            return false;
        }
        if (!pStream->ExtractPrimitive(&length)) {
            return false;
        }
        if (pStream->m_Position + length > end) {
            return false;
        }
        switch (id) {
        case OPTION_SUPPORTED_FUNCTIONS:
            if (length != sizeof(u32) || !pStream->ExtractPrimitiveInline(&pPacket->m_Unknown0x8C)) {
                return false;
            }
            break;
        case OPTION_CONNECTION_SIGNATURE:
            if (length != SIGNATURE_SIZE || !pStream->ExtractRaw(pPacket->m_Signature.m_Data, SIGNATURE_SIZE)) {
                return false;
            }
            pPacket->m_Signature.m_Size = SIGNATURE_SIZE;
            break;
        case OPTION_FRAGMENT_ID: {
            u8 value;
            if (length != sizeof(u8) || !pStream->ExtractPrimitive(&value)) {
                return false;
            }
            pPacket->m_Unknown0x64 = value;
            break;
        }
        case OPTION_INITIAL_SEQUENCE_ID: {
            u16 value;
            if (length != sizeof(u16) || !pStream->ExtractPrimitive(&value)) {
                return false;
            }
            pPacket->m_Unknown0x48.m_Value = value;
            break;
        }
        case OPTION_MAX_SUBSTREAM_ID: {
            u8 value;
            if (length != sizeof(u8) || !pStream->ExtractPrimitive(&value)) {
                return false;
            }
            pPacket->m_Unknown0x67 = value;
            break;
        }
        default:
            // (an unknown option is skipped)
            if (!Skip(pStream, length)) {
                return false;
            }
            break;
        }
    }
    // the options are kept as they are
    LiteByteStream<52>& options = pPacket->m_Options;
    options.m_Buffer.m_Size = 0;
    options.m_Position = 0;
    options.m_IsError = false;
    if (size != 0) {
        Buffer* pBuffer = pStream->m_pBuffer;
        return options.m_Buffer.AppendData(pBuffer->m_pData + pBuffer->m_HeaderSize + start, size, 0);
    }
    return true;
}

// 0x00372B34 slot 0x0C | fefates:bytes
void nn::nex::PRUDPMessageV1::CalcSignature(nn::nex::Packet* pPacket, nn::nex::PRUDPMessageInterface::SignatureMethod method, const nn::nex::Key* pKey, nn::nex::Stream::Type type, nn::nex::TransportSignatureGenerator*, const nn::nex::SignatureBytes* pReceived, nn::nex::SignatureBytes& signature)
{
    CalcSignatureHelper(pPacket, method, pKey, type, pReceived, signature);
}

// 0x00372B58 slot 0x08 | fefates:bytes
bool nn::nex::PRUDPMessageV1::CalcSignature(nn::nex::PacketOut* pPacket, nn::nex::PRUDPMessageInterface::SignatureMethod method, const nn::nex::Key* pKey, nn::nex::Stream::Type type, nn::nex::TransportSignatureGenerator*, const nn::nex::SignatureBytes* pReceived, nn::nex::SignatureBytes& signature)
{
    pPacket->m_TypeFlags |= 0x80;
    if (!pPacket->m_IsHeaderV1Valid) {
        pPacket->m_IsHeaderV1Valid = true;
        pPacket->m_HeaderV1.m_SourcePort = pPacket->m_SourcePort;
        pPacket->m_HeaderV1.m_DestinationPort = pPacket->m_DestinationPort;
        pPacket->m_HeaderV1.m_TypeFlags = pPacket->m_TypeFlags;
        pPacket->m_HeaderV1.m_SessionId = pPacket->m_SessionId;
        pPacket->m_HeaderV1.m_SubstreamId = pPacket->m_SubstreamId;
        pPacket->m_HeaderV1.m_SequenceId = pPacket->m_SequenceId.m_Value;
        if (!OptionBuilder(pPacket)) {
            return false;
        }
    }
    CalcSignatureHelper(pPacket, method, pKey, type, pReceived, signature);
    return true;
}

// 0x00372C08 | fefates:bytes [tier B]
bool nn::nex::PRUDPMessageV1::OptionBuilder(nn::nex::PacketOut* pPacket)
{
    LiteByteStream<52>& options = pPacket->m_Options;
    options.m_Buffer.m_Size = 0;
    options.m_Position = 0;
    options.m_IsError = false;
    u32 type = pPacket->GetType();
    if (type == Packet::TYPE_SYN || type == Packet::TYPE_CONNECT) {
        u8 byte = OPTION_SUPPORTED_FUNCTIONS;
        if (!options.Append(byte)) {
            return false;
        }
        byte = sizeof(u32);
        if (!options.Append(byte)) {
            return false;
        }
        u32 functions = pPacket->m_Unknown0x8C;
        if (!options.m_Buffer.AppendData(&functions, sizeof(functions), options.m_Buffer.m_Size)) {
            return false;
        }
        byte = OPTION_CONNECTION_SIGNATURE;
        if (!options.Append(byte)) {
            return false;
        }
        byte = SIGNATURE_SIZE;
        if (!options.Append(byte)) {
            return false;
        }
        if (pPacket->m_Signature.m_Size != 0) {
            if (!options.m_Buffer.AppendData(pPacket->m_Signature.m_Data, pPacket->m_Signature.m_Size, options.m_Buffer.m_Size)) {
                return false;
            }
        } else {
            for (u32 i = 0; i < SIGNATURE_SIZE; i++) {
                u8 zero = 0;
                if (!options.m_Buffer.AppendData(&zero, sizeof(zero), options.m_Buffer.m_Size)) {
                    return false;
                }
            }
        }
        if (pPacket->GetType() == Packet::TYPE_CONNECT && pPacket->m_Unknown0x48.m_Value != 0) {
            byte = OPTION_INITIAL_SEQUENCE_ID;
            if (!options.Append(byte)) {
                return false;
            }
            byte = sizeof(u16);
            if (!options.Append(byte)) {
                return false;
            }
            u16 sequenceId = pPacket->m_Unknown0x48.m_Value;
            if (!options.m_Buffer.AppendData(&sequenceId, sizeof(sequenceId), options.m_Buffer.m_Size)) {
                return false;
            }
        }
        byte = OPTION_MAX_SUBSTREAM_ID;
        if (!options.Append(byte)) {
            return false;
        }
        byte = sizeof(u8);
        if (!options.Append(byte)) {
            return false;
        }
        if (!options.Append(pPacket->m_Unknown0x67)) {
            return false;
        }
    }
    if (pPacket->GetType() == Packet::TYPE_DATA) {
        u8 byte = OPTION_FRAGMENT_ID;
        if (!options.Append(byte)) {
            return false;
        }
        byte = sizeof(u8);
        if (!options.Append(byte)) {
            return false;
        }
        if (!options.Append(pPacket->m_Unknown0x64)) {
            return false;
        }
    }
    return true;
}

// 0x00372F98 | fefates:callgraph [tier C]
void nn::nex::PRUDPMessageV1::CalcSignatureHelper(const nn::nex::Packet* pPacket, nn::nex::PRUDPMessageInterface::SignatureMethod method, const nn::nex::Key* pKey, nn::nex::Stream::Type type, const nn::nex::SignatureBytes* pReceived, nn::nex::SignatureBytes& signature)
{
    signature.m_Size = SIGNATURE_SIZE;
    switch (method) {
    case 0:
        memset(signature.m_Data, SIGNATURE_FILL_NONE, SIGNATURE_SIZE);
        break;
    case 1: {
        // the address and the port of the peer
        KeyedChecksumAlgorithm* pAlgorithm = Stream::GetSettings(type)->m_pChecksumAlgorithm;
        if (pAlgorithm == nullptr) {
            memset(signature.m_Data, SIGNATURE_FILL_NO_ALGORITHM, SIGNATURE_SIZE);
            return;
        }
        LiteByteStream<6> address;
        u32 ip = __builtin_bswap32(pPacket->m_Address.m_SockAddr.m_Address);
        address.m_Buffer.AppendData(&ip, sizeof(ip), address.m_Buffer.m_Size);
        u16 port = __builtin_bswap16(pPacket->m_Address.m_SockAddr.m_Port);
        address.m_Buffer.AppendData(&port, sizeof(port), address.m_Buffer.m_Size);
        const u8* data[1] = {address.m_Buffer.m_Data};
        u32 sizes[1] = {address.m_Buffer.m_Size};
        pAlgorithm->ComputeChecksum(data, sizes, 1, signature);
        signature.m_Size = SIGNATURE_SIZE;
        break;
    }
    case 4:
    case 5:
    case 6:
    case 7: {
        // the header, the key (6), the key sum and the received signature (not 7), the options and
        // the payload
        KeyedChecksumAlgorithm* pAlgorithm = Stream::GetSettings(type)->m_pChecksumAlgorithm;
        if (pAlgorithm == nullptr) {
            memset(signature.m_Data, SIGNATURE_FILL_NO_ALGORITHM, SIGNATURE_SIZE);
            return;
        }
        const u8* data[5] = {&pPacket->m_HeaderV1.m_SourcePort, nullptr};
        u32 sizes[5] = {8, 0};
        if (method == 6 && pKey != nullptr) {
            data[1] = pKey->GetContentPtr();
            sizes[1] = pKey->GetLength();
        }
        LiteByteStream<20> extra;
        u16 typeFlags = pPacket->m_TypeFlags;
        if ((typeFlags & Packet::TYPE_MASK) != Packet::TYPE_SYN || (typeFlags & 0x10) == 0 || (pPacket->m_Unknown0x8C & 0x100) == 0) {
            s32 keySum = Stream::GetSettings(type)->m_KeySum;
            extra.m_Buffer.AppendData(&keySum, sizeof(keySum), extra.m_Buffer.m_Size);
        }
        bool isReceivedSkipped = false;
        if (pReceived != nullptr && (typeFlags & Packet::TYPE_MASK) != Packet::TYPE_SYN) {
            if (method == 4 && (typeFlags & Packet::TYPE_MASK) == Packet::TYPE_CONNECT) {
                isReceivedSkipped = true;
            } else if ((typeFlags & 0x200) == 0 && (typeFlags & 0x400) == 0 && (typeFlags & Packet::TYPE_MASK) != 5 && pReceived->m_Size != 0) {
                extra.m_Buffer.AppendData(pReceived->m_Data, pReceived->m_Size, extra.m_Buffer.m_Size);
            }
        }
        if (method == 7 && !isReceivedSkipped) {
            data[2] = nullptr;
            sizes[2] = 0;
        } else {
            data[2] = extra.m_Buffer.m_Data;
            sizes[2] = extra.m_Buffer.m_Size;
        }
        if (pPacket->m_Options.m_Buffer.m_Size != 0) {
            data[3] = pPacket->m_Options.m_Buffer.m_Data;
            sizes[3] = pPacket->m_Options.m_Buffer.m_Size;
        } else {
            data[3] = nullptr;
            sizes[3] = 0;
        }
        Buffer* pPayload = pPacket->m_pPayload;
        if (pPayload != nullptr && pPayload->m_ContentSize != 0) {
            data[4] = pPayload->m_pData + pPayload->m_HeaderSize;
            sizes[4] = pPayload->m_ContentSize;
        } else {
            data[4] = nullptr;
            sizes[4] = 0;
        }
        pAlgorithm->ComputeChecksum(data, sizes, 5, signature);
        signature.m_Size = SIGNATURE_SIZE;
        break;
    }
    case 8: {
        // a random signature
        ScopedCS lock(m_Lock);
        for (u32 i = 0; i < SIGNATURE_SIZE; i++) {
            signature.m_Data[i] = m_Random.GetRandomNumber(255);
        }
        break;
    }
    default:
        break;
    }
}

// 0x003733D4 | fefates:bytes
int nn::nex::PRUDPMessageV1::CalcExpectedSignature(const nn::nex::Packet* pPacket, nn::nex::PRUDPMessageInterface::SignatureMethod method, const nn::nex::Key* pKey, nn::nex::TransportSignatureGenerator*, const nn::nex::SignatureBytes* pReceived, nn::nex::SignatureBytes& signature)
{
    // (no signature to check)
    if (method == SIGNATURE_METHOD_UNSIGNED) {
        return 3;
    }
    CalcSignatureHelper(pPacket, method, pKey, static_cast<Stream::Type>(pPacket->GetStreamType()), pReceived, signature);
    return 0;
}

// 0x00373414
nn::nex::PRUDPMessageInterface::SignatureMethod nn::nex::PRUDPMessageV1::DecideSignatureMethod(const nn::nex::Packet* pPacket, nn::nex::Stream::Type type, bool isOutgoing)
{
    if (Stream::GetSettings(type)->m_pChecksumAlgorithm == nullptr) {
        return static_cast<SignatureMethod>(0);
    }
    u32 packetType = pPacket->GetType();
    if (packetType != Packet::TYPE_SYN && packetType != Packet::TYPE_CONNECT && (type == 10 || type == 2)) {
        if (isOutgoing) {
            return static_cast<SignatureMethod>(6);
        }
        if (packetType == Packet::TYPE_DISCONNECT && (pPacket->m_TypeFlags & 0x10) != 0) {
            return SIGNATURE_METHOD_UNSIGNED;
        }
    }
    return static_cast<SignatureMethod>(5);
}

// 0x00373480 | fefates:callseq
bool nn::nex::PRUDPMessageV1::Pack(nn::nex::PacketOut* pPacket, nn::nex::ByteStream* pStream)
{
    Buffer* pData = nullptr;
    bool isOwned = false;
    u16 payloadSize = 0;
    Stream::Type streamType = static_cast<Stream::Type>(pPacket->GetStreamType());
    if (pPacket->GetType() == Packet::TYPE_RAW) {
        Buffer* pPayload = pPacket->m_pPayload;
        pStream->m_pBuffer->AppendData(pPayload->m_pData + pPayload->m_HeaderSize, pPayload->m_ContentSize, static_cast<unsigned>(-1));
        pPacket->m_Unknown0x134 = pStream->m_pBuffer->m_ContentSize;
        return true;
    }
    if (!pPacket->m_IsHeaderV1Valid) {
        pPacket->m_IsHeaderV1Valid = true;
        pPacket->m_HeaderV1.m_SourcePort = pPacket->m_SourcePort;
        pPacket->m_HeaderV1.m_DestinationPort = pPacket->m_DestinationPort;
        pPacket->m_HeaderV1.m_TypeFlags = pPacket->m_TypeFlags;
        pPacket->m_HeaderV1.m_SessionId = pPacket->m_SessionId;
        pPacket->m_HeaderV1.m_SubstreamId = pPacket->m_SubstreamId;
        pPacket->m_HeaderV1.m_SequenceId = pPacket->m_SequenceId.m_Value;
        if (!OptionBuilder(pPacket)) {
            return false;
        }
    }
    if (pPacket->m_pPayload != nullptr) {
        // the payload compressed and encrypted with the algorithms of the stream type
        pData = pPacket->m_pPayload;
        CompressionAlgorithm* pCompression = Stream::GetSettings(streamType)->m_pCompressionAlgorithm;
        if (pCompression != nullptr) {
            pData = new Buffer(pPacket->m_pPayload->m_ContentSize + 64);
            pCompression->Compress(*pPacket->m_pPayload, pData);
            isOwned = true;
        }
        EncryptionAlgorithm* pEncryption = Stream::GetSettings(streamType)->m_pEncryptionAlgorithm;
        if (pEncryption != nullptr) {
            Buffer* pEncrypted = new Buffer(pData->m_ContentSize);
            pEncryption->Encrypt(*pData, pEncrypted);
            if (isOwned && pData != nullptr) {
                delete pData;
            }
            pData = pEncrypted;
            isOwned = true;
        }
        payloadSize = pData->m_ContentSize;
    }
    u8 magic = MAGIC_0;
    pStream->m_pBuffer->AppendData(&magic, sizeof(magic), static_cast<unsigned>(-1));
    magic = MAGIC_1;
    pStream->m_pBuffer->AppendData(&magic, sizeof(magic), static_cast<unsigned>(-1));
    pPacket->m_HeaderV1.m_Version = VERSION;
    pPacket->m_HeaderV1.m_OptionsSize = pPacket->m_Options.m_Buffer.m_Size;
    pPacket->m_HeaderV1.m_PayloadSize = payloadSize;
    if (!pStream->m_pBuffer->AppendData(&pPacket->m_HeaderV1, sizeof(Packet::HeaderV1), static_cast<unsigned>(-1))) {
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
    if (pPacket->m_Options.m_Buffer.m_Size != 0) {
        if (!pStream->m_pBuffer->AppendData(pPacket->m_Options.m_Buffer.m_Data, pPacket->m_Options.m_Buffer.m_Size, static_cast<unsigned>(-1))) {
            return false;
        }
    }
    if (pPacket->m_pPayload != nullptr) {
        pStream->m_pBuffer->AppendData(pData->m_pData + pData->m_HeaderSize, pData->m_ContentSize, static_cast<unsigned>(-1));
    }
    pPacket->m_Unknown0x134 = pStream->m_pBuffer->m_ContentSize;
    if (isOwned && pData != nullptr) {
        delete pData;
    }
    return true;
}

// 0x003737B4 | fefates:callseq
bool nn::nex::PRUDPMessageV1::Unpack(nn::nex::ByteStream* pStream, nn::nex::PacketIn* pPacket, unsigned int* pSize)
{
    bool isUnpacked = false;
    if (pStream->m_pBuffer->m_ContentSize == 0) {
        return isUnpacked;
    }
    u32 optionsSize = 0;
    u32 payloadSize = 0;
    if (!HeaderParse(pStream, pPacket, optionsSize, payloadSize)) {
        return false;
    }
    if (!pStream->ExtractRaw(pPacket->m_PacketSignature.m_Data, SIGNATURE_SIZE)) {
        return false;
    }
    pPacket->m_PacketSignature.m_Size = SIGNATURE_SIZE;
    Buffer* pBuffer = pStream->m_pBuffer;
    if (optionsSize != 0 && !OptionUnpack(pStream, pPacket, optionsSize)) {
        return false;
    }
    Stream::Type streamType = static_cast<Stream::Type>(pPacket->GetStreamType());
    if (streamType > MAX_STREAM_TYPE) {
        return false;
    }
    Buffer* pPayload = pPacket->m_pPayload;
    pPayload->m_ContentSize = 0;
    pPayload->m_HeaderSize = 0;
    if (payloadSize == 0) {
        isUnpacked = true;
    } else {
        if (pStream->m_Position + payloadSize > pBuffer->m_ContentSize) {
            return false;
        }
        if (pPacket->m_pPayload->AppendData(pBuffer->m_pData + pBuffer->m_HeaderSize + pStream->m_Position, payloadSize, static_cast<unsigned>(-1))) {
            isUnpacked = true;
            u32 end = payloadSize + pStream->m_Position;
            if (pStream->m_pBuffer->m_ContentSize >= end) {
                pStream->m_Position = end;
            }
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
    if (pSize != nullptr) {
        *pSize = pStream->m_Position;
    }
    return isUnpacked;
}

// 0x003739B8 (name is ours)
nn::nex::PRUDPMessageV1::PRUDPMessageV1() : m_Lock(1)
{
    m_Random.SetSeed(Platform::GetRandomSeed());
}

// 0x00373AD0 slot 0x00
// 0x00373A94 (deleting dtor)
nn::nex::PRUDPMessageV1::~PRUDPMessageV1()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
