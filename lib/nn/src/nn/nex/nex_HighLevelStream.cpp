#include "nn/nex/nex_HighLevelStream.h"
#include "nn/nex/nex_Buffer.h"
#include "nn/nex/nex_InetAddress.h"
#include "nn/nex/nex_Key.h"
#include "nn/nex/nex_LiteBuffer.h"
#include "nn/nex/nex_Network.h"
#include "nn/nex/nex_PRUDPEndPoint.h"
#include "nn/nex/nex_PRUDPMessageInterface.h"
#include "nn/nex/nex_PacketBufferManager.h"
#include "nn/nex/nex_PacketBufferPacketOut.h"
#include "nn/nex/nex_PacketEncDec.h"
#include "nn/nex/nex_PacketOut.h"
#include "nn/nex/nex_Platform.h"
#include "nn/nex/nex_RootTransport.h"
#include "nn/nex/nex_StreamSettings.h"
#include "nn/nex/nex_Time.h"
#include "nn/nex/nex_TransportStreamManager.h"

#include <string.h>

namespace nn {
namespace nex {
namespace {
// the packet could not be made
const int RESULT_PACKET_FAILED = static_cast<int>(0x8005001F);
// the stream type with the encryption of the settings
const u8 ENCRYPTED_STREAM_TYPE = 9;
} // namespace

// 0x00377D84 slot 0x14 (name is ours)
void nn::nex::HighLevelStream::ProcessPayload(const nn::nex::InetAddress* pAddress, unsigned char sourceType, unsigned char sourcePort, nn::nex::Buffer* pPayload,
                                              const nn::nex::Time& time, unsigned short port)
{
    // nothing to do
}

// 0x00377D88 slot 0x08
bool nn::nex::HighLevelStream::ReceiveIncomingPacket(unsigned short port, unsigned char session, nn::nex::Packet* pPacket)
{
    if (m_IsSigned) {
        SignatureBytes signature;
        signature.m_Size = 0;
        PRUDPMessageInterface::SignatureMethod method = PRUDPMessageInterface::SIGNATURE_METHOD_HEADER;
        if (!m_IsHeaderSigned) {
            method = m_pMessageInterface->DecideSignatureMethod(pPacket, static_cast<Stream::Type>(m_Type), false);
            if (method == PRUDPMessageInterface::SIGNATURE_METHOD_UNSIGNED || method == PRUDPMessageInterface::SIGNATURE_METHOD_ADDRESS) {
                return false;
            }
        }
        m_pMessageInterface->CalcSignature(pPacket, method, nullptr, static_cast<Stream::Type>(m_Type), nullptr, nullptr, signature);
        if (signature.m_Size != pPacket->m_PacketSignature.m_Size || memcmp(signature.m_Data, pPacket->m_PacketSignature.m_Data, signature.m_Size) != 0) {
            return false;
        }
    }
    if (m_pEncDec != nullptr) {
        m_pEncDec->Decode(pPacket);
    }
    Time time = pPacket->m_Unknown0x100;
    u8 sourcePort = pPacket->m_SourcePort;
    ProcessPayload(&pPacket->m_Address, sourcePort >> 4, sourcePort & 0xF, pPacket->m_pPayload, time, port);
    pPacket->ReleaseRef();
    return true;
}

// 0x00377EEC slot 0x18 (name is ours)
nn::nex::qResult nn::nex::HighLevelStream::Send(const nn::nex::InetAddress* pAddress, unsigned char destinationType, unsigned char destinationPort, nn::nex::Buffer* pPayload,
                                                unsigned char unknown, unsigned short port)
{
    if (port == 0) {
        port = m_pTransport->m_PortNumber;
    }
    PacketOut* pPacket;
    if (pPayload != nullptr && (pPayload->m_Flags & Buffer::FLAG_USE_POOL)) {
        void* pMemory = PacketBufferManager::AllocateObject(sizeof(PacketBufferPacketOut), false);
        if (pMemory == nullptr) {
            return qResult(RESULT_PACKET_FAILED);
        }
        pPacket = ::new (pMemory) PacketBufferPacketOut(nullptr, static_cast<PacketType>(Packet::TYPE_HIGH_LEVEL), 0, pPayload, 0);
        if ((pPacket->m_TypeFlags & PRUDPEndPoint::FLAG_NEED_ACK) && pPacket->m_pTimeout == nullptr) {
            delete pPacket;
            return qResult(RESULT_PACKET_FAILED);
        }
    } else {
        pPacket = new PacketOut(nullptr, static_cast<PacketType>(Packet::TYPE_HIGH_LEVEL), 0, pPayload, 0);
    }
    pPacket->m_Address.m_SockAddr = pAddress->m_SockAddr;
    pPacket->m_SourcePort = (m_Type << 4) | m_Port;
    pPacket->m_DestinationPort = (destinationType << 4) | destinationPort;
    pPacket->m_MessageVersion = m_MessageVersion;
    pPacket->m_Unknown0x66 = unknown;
    pPacket->m_SessionId = m_SessionId;
    pPacket->m_SequenceId.m_Value = m_SequenceId.m_Value;
    m_SequenceId.m_Value++;
    if (m_pEncDec != nullptr) {
        m_pEncDec->Encode(pPacket, static_cast<Stream::Type>(m_Type), false);
    }
    if (m_IsSigned) {
        PRUDPMessageInterface::SignatureMethod method = PRUDPMessageInterface::SIGNATURE_METHOD_HEADER;
        if (!m_IsHeaderSigned) {
            method = m_pMessageInterface->DecideSignatureMethod(pPacket, static_cast<Stream::Type>(m_Type), false);
        }
        if (method != PRUDPMessageInterface::SIGNATURE_METHOD_ADDRESS) {
            SignatureBytes signature;
            signature.m_Size = 0;
            if (m_pMessageInterface->CalcSignature(pPacket, method, nullptr, static_cast<Stream::Type>(m_Type), nullptr, nullptr, signature)) {
                pPacket->m_PacketSignature = signature;
            }
        }
    }
    bool isSent = m_pTransport->SendPacket(port, m_Type, m_Port, destinationPort, pPacket, false);
    pPacket->ReleaseRef();
    return isSent ? qResult(qResult::SUCCESS) : qResult(qResult::FAILURE);
}

// 0x003781D0 slot 0x0C
void nn::nex::HighLevelStream::DoWork()
{
    // nothing to do
}

// 0x003781D4 (name is ours)
bool nn::nex::HighLevelStream::Listen(unsigned char port)
{
    if (m_Port != NO_PORT) {
        return false;
    }
    if (m_Type == ENCRYPTED_STREAM_TYPE && GetThisSettings()->m_Unknown0xA0) {
        LiteBuffer<32> keyData(GetSettings(static_cast<Stream::Type>(m_Type))->m_Unknown0xA4);
        Key key(keyData.m_Data, keyData.m_Size);
        if (key.GetLength() == 0) {
            return false;
        }
        m_pEncDec = new PacketEncDec();
        m_pEncDec->m_ReliableKey = key;
        m_pEncDec->GenerateKey(m_pEncDec->m_ReliableKey, s_UnreliableKeySalts, s_UnreliableKeySalts + 8, &m_pEncDec->m_UnreliableKey);
        m_pEncDec->GenerateKey(m_pEncDec->m_ReliableKey, s_UnreliableKeySalts + 16, s_UnreliableKeySalts + 24, &m_pEncDec->m_UnreliableKeyHigh);
    }
    if (!m_pTransport->m_StreamManager.RegisterStream(this, port, nullptr)) {
        return false;
    }
    m_Port = port;
    return true;
}

// 0x00378308
nn::nex::HighLevelStream::HighLevelStream(nn::nex::Stream::Type type, nn::nex::RootTransport* pTransport)
    : Stream(type), m_MessageVersion(0), m_IsSigned(false), m_IsHeaderSigned(false), m_pTransport(pTransport), m_Port(NO_PORT), m_pEncDec(nullptr)
{
    m_SequenceId.m_Value = 0;
    if (pTransport == nullptr) {
        Network* pNetwork = Network::GetInstance();
        m_pTransport = pNetwork != nullptr ? pNetwork->m_pRootTransport : nullptr;
    }
    m_pMessageInterface = &Network::GetInstance()->m_MessageSelector;
    m_SessionId = static_cast<u8>(SystemClock::GetTime().m_Value + Platform::GetRandomNumber(0xFF));
    m_SequenceId.m_Value = static_cast<u16>(SystemClock::GetTime().m_Value + Platform::GetRandomNumber(0xFFFF));
}

// 0x0037846C slot 0x00
// 0x003783E8 (deleting dtor)
nn::nex::HighLevelStream::~HighLevelStream()
{
    m_pTransport->m_StreamManager.UnregisterStream(static_cast<Stream::Type>(m_Type), m_Port);
    if (m_pEncDec != nullptr) {
        delete m_pEncDec;
        m_pEncDec = nullptr;
    }
}

} // namespace nex
} // namespace nn
