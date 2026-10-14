#include "nn/nex/nex_PRUDPMessageSelector.h"
#include "nn/nex/nex_ByteStream.h"
#include "nn/nex/nex_PRUDPMessageV0.h"
#include "nn/nex/nex_PRUDPMessageV1.h"
#include "nn/nex/nex_PacketIn.h"
#include "nn/nex/nex_PacketOut.h"
#include "nn/nex/nex_RelayMessage.h"

namespace nn {
namespace nex {
// 0x003976B4 (name is ours)
nn::nex::PRUDPMessageSelector::PRUDPMessageSelector() : m_Messages(2)
{
    m_Messages[0] = new PRUDPMessageV0();
    m_Messages[1] = new PRUDPMessageV1();
    m_pRelayMessage = new RelayMessage();
}

// 0x003977F4 slot 0x00 | fefates:bytes
// 0x003977C4 (deleting dtor)
nn::nex::PRUDPMessageSelector::~PRUDPMessageSelector()
{
    for (u32 i = 0; i < m_Messages.size(); i++) {
        if (m_Messages[i] != nullptr) {
            delete m_Messages[i];
        }
    }
    if (m_pRelayMessage != nullptr) {
        delete m_pRelayMessage;
    }
}

// 0x00397554 slot 0x08 | fefates:bytes
bool nn::nex::PRUDPMessageSelector::CalcSignature(nn::nex::PacketOut* pPacket, nn::nex::PRUDPMessageInterface::SignatureMethod method, const nn::nex::Key* pKey, nn::nex::Stream::Type type, nn::nex::TransportSignatureGenerator* pGenerator, const nn::nex::SignatureBytes* pReceived, nn::nex::SignatureBytes& signature)
{
    return GetMessage(pPacket)->CalcSignature(pPacket, method, pKey, type, pGenerator, pReceived, signature);
}

// 0x00397520 slot 0x0C | fefates:bytes
void nn::nex::PRUDPMessageSelector::CalcSignature(nn::nex::Packet* pPacket, nn::nex::PRUDPMessageInterface::SignatureMethod method, const nn::nex::Key* pKey, nn::nex::Stream::Type type, nn::nex::TransportSignatureGenerator* pGenerator, const nn::nex::SignatureBytes* pReceived, nn::nex::SignatureBytes& signature)
{
    GetMessage(pPacket)->CalcSignature(pPacket, method, pKey, type, pGenerator, pReceived, signature);
}

// 0x003975BC slot 0x10 | fefates:bytes
nn::nex::PRUDPMessageInterface::SignatureMethod nn::nex::PRUDPMessageSelector::DecideSignatureMethod(const nn::nex::Packet* pPacket, nn::nex::Stream::Type type, bool isOutgoing)
{
    return GetMessage(pPacket)->DecideSignatureMethod(pPacket, type, isOutgoing);
}

// 0x00397588 slot 0x14 | fefates:bytes
int nn::nex::PRUDPMessageSelector::CalcExpectedSignature(const nn::nex::Packet* pPacket, nn::nex::PRUDPMessageInterface::SignatureMethod method, const nn::nex::Key* pKey, nn::nex::TransportSignatureGenerator* pGenerator, const nn::nex::SignatureBytes* pReceived, nn::nex::SignatureBytes& signature)
{
    return GetMessage(pPacket)->CalcExpectedSignature(pPacket, method, pKey, pGenerator, pReceived, signature);
}

// 0x0039760C slot 0x18 | fefates:bytes
bool nn::nex::PRUDPMessageSelector::Unpack(nn::nex::ByteStream* pStream, nn::nex::PacketIn* pPacket, unsigned int* pSize)
{
    if (pSize != nullptr) {
        *pSize = 0;
    }
    Buffer* pBuffer = pStream->m_pBuffer;
    if (pBuffer->m_ContentSize < MIN_MESSAGE_SIZE) {
        return false;
    }
    const u8* pData = pBuffer->m_pData + pBuffer->m_HeaderSize;
    if (pData[0] == RelayMessage::MAGIC_0 && pData[1] == RelayMessage::MAGIC_1) {
        return m_pRelayMessage->Unpack(pStream, pPacket, pSize);
    }
    // (one byte of the magic is enough)
    int version = 0;
    if (pData[0] == PRUDPMessageV1::MAGIC_0 || pData[1] == PRUDPMessageV1::MAGIC_1) {
        version = pData[2];
        if (version == 0 || version > PRUDPMessageV1::VERSION) {
            return false;
        }
    }
    return m_Messages[version]->Unpack(pStream, pPacket, pSize);
}

// 0x00397478 slot 0x1C | fefates:bytes
bool nn::nex::PRUDPMessageSelector::FastUnpack(nn::nex::ByteStream* pStream, nn::nex::PacketIn* pPacket, unsigned int* pSize)
{
    if (pSize != nullptr) {
        *pSize = 0;
    }
    Buffer* pBuffer = pStream->m_pBuffer;
    if (pBuffer->m_ContentSize < MIN_MESSAGE_SIZE) {
        return false;
    }
    const u8* pData = pBuffer->m_pData + pBuffer->m_HeaderSize;
    if (pData[0] == RelayMessage::MAGIC_0 && pData[1] == RelayMessage::MAGIC_1) {
        return m_pRelayMessage->FastUnpack(pStream, pPacket, pSize);
    }
    // (one byte of the magic is enough)
    int version = 0;
    if (pData[0] == PRUDPMessageV1::MAGIC_0 || pData[1] == PRUDPMessageV1::MAGIC_1) {
        version = pData[2];
        if (version == 0 || version > PRUDPMessageV1::VERSION) {
            return false;
        }
    }
    return m_Messages[version]->FastUnpack(pStream, pPacket, pSize);
}

// 0x003975D4 slot 0x20 | fefates:bytes
bool nn::nex::PRUDPMessageSelector::Pack(nn::nex::PacketOut* pPacket, nn::nex::ByteStream* pStream)
{
    if (pPacket->GetStreamType() == STREAM_TYPE_RELAY) {
        return m_pRelayMessage->Pack(pPacket, pStream);
    }
    return GetMessage(pPacket)->Pack(pPacket, pStream);
}

} // namespace nex
} // namespace nn
