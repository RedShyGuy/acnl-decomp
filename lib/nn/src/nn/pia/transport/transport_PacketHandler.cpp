#include "nn/pia/transport/transport_PacketHandler.h"
#include "nn/pia/common/common_Crypto.h"
#include "nn/pia/common/common_Packet.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/transport/transport_PacketAnalyzer.h"
#include "nn/pia/transport/transport_ProtocolMessageFilteringManager.h"
#include "nn/pia/transport/transport_Station.h"
#include "nn/pia/transport/transport_StationConnectionInfo.h"
#include "nn/pia/transport/transport_StationConnectionInfoTable.h"
#include "nn/pia/transport/transport_StationManager.h"
#include "pead/RuntimeTypeInfo/peadRoot.h"

namespace nn {
namespace pia {
namespace transport {
namespace {
// the packet at index of the stream (null outside of the ring) and the index after it
common::Packet* GetStreamPacket(const PacketStream::Accessor* pAccessor, s32 index)
{
    return pAccessor->m_pStream->GetPacket(index);
}

s32 GetNextIndex(const PacketStream::Accessor* pAccessor, s32 index)
{
    index++;
    if (index >= static_cast<s32>(pAccessor->m_pStream->m_PacketNum)) {
        index = 0;
    }
    return index;
}
} // namespace

// 0x0044DE94
u32 nn::pia::transport::PacketHandler::CheckPacket(const nn::pia::common::Packet&)
{
    return 0;
}

// 0x0044DED0 | fefates:bytes [tier B]
void nn::pia::transport::PacketHandler::CleanupCore()
{
    if (common::IsValidPointer(m_pReceiveAnalyzer)) {
        m_pReceiveAnalyzer->Cleanup();
    }
    if (common::IsValidPointer(m_pSendAnalyzer)) {
        m_pSendAnalyzer->Cleanup();
    }
    m_pWriter = nullptr;
    m_pReader = nullptr;
    m_PayloadSizeLimit = 0;
    m_PacketSizeLimit = 0;
    m_CryptoSetting = common::CryptoSetting();
}

// 0x0044DF50 | fefates:bytes [tier B]
nn::pia::transport::PacketHandler::Iterator* nn::pia::transport::PacketHandler::GetIterator(const nn::pia::transport::ProtocolId& protocolId)
{
    m_IterationProtocolId = __builtin_bswap32(protocolId.m_Id);
    m_IterationMask = 0xFFFFFFFF;
    BeginIteration();
    return &m_Iterator;
}

// 0x0044DF88 | fefates:bytes [tier B]
nn::pia::transport::PacketHandler::Iterator* nn::pia::transport::PacketHandler::GetIterator(unsigned short protocolType)
{
    ProtocolId protocolId;
    protocolId.SetType(protocolType);
    m_IterationProtocolId = __builtin_bswap32(protocolId.m_Id & 0xFFFF0000);
    m_IterationMask = __builtin_bswap32(0xFFFF0000);
    BeginIteration();
    return &m_Iterator;
}

// 0x0044DFD8 | fefates:bytes [tier B]
nn::Result nn::pia::transport::PacketHandler::StartupCore(nn::pia::transport::PacketStream::Writer* pWriter, nn::pia::transport::PacketStream::Reader* pReader, unsigned int packetSize, const nn::pia::common::CryptoSetting* pCryptoSetting)
{
    if (!common::IsValidPointer(pWriter) || !common::IsValidPointer(pReader)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (common::IsValidPointer(pCryptoSetting) && pCryptoSetting->m_Mode != common::Crypto::MODE_NONE &&
        pCryptoSetting->m_Mode != common::Crypto::MODE_AES128) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_pWriter != nullptr || m_pReader != nullptr) {
        return common::RESULT_INVALID_STATE;
    }
    m_pWriter = pWriter;
    m_pReader = pReader;
    m_MessageWriter.SetSource(STATION_INDEX_UNIDENTIFIED, 0);
    m_pReservedWriter = nullptr;
    if (packetSize - common::Packet::HEADER_SIZE >= 0x5AB) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (common::IsValidPointer(pCryptoSetting)) {
        m_CryptoSetting = *pCryptoSetting;
    } else {
        m_CryptoSetting = common::CryptoSetting();
    }
    switch (m_CryptoSetting.m_Mode) {
    case common::Crypto::MODE_NONE:
        m_PacketSizeLimit = packetSize;
        break;
    case common::Crypto::MODE_AES128: {
        // the encrypted part in whole blocks
        u32 blockSize = common::Crypto::GetBlockSize(common::Crypto::MODE_AES128);
        m_PacketSizeLimit = (packetSize - common::Packet::HEADER_SIZE) / blockSize * blockSize + common::Packet::HEADER_SIZE;
        break;
    }
    }
    m_PayloadSizeLimit = m_PacketSizeLimit - common::Packet::HEADER_SIZE;
    m_pSendAnalyzer->Startup();
    m_pSendAnalyzer->ClearPacketAnalysisData();
    m_pReceiveAnalyzer->Startup();
    m_pReceiveAnalyzer->ClearPacketAnalysisData();
    return nn::Result();
}

// 0x0044E16C | fefates:bytes
nn::pia::common::Packet* nn::pia::transport::PacketHandler::AssignPacket(nn::pia::StationIndex stationIndex, unsigned int stationBitmap, const nn::pia::common::StationAddress& address, bool isOwnPacket)
{
    common::Packet* pPacket = m_pWriter->Assign();
    if (pPacket == nullptr) {
        return nullptr;
    }
    pPacket->Reset();
    pPacket->m_DestinationStationIndex = stationIndex;
    pPacket->m_DestinationBitmap = stationBitmap;
    pPacket->m_DestinationStationAddress = address;
    pPacket->m_Unknown0x5D5 = isOwnPacket;
    return pPacket;
}

// 0x0044E1D8
bool nn::pia::transport::PacketHandler::CheckMessage(const nn::pia::transport::ProtocolMessageReader&)
{
    return false;
}

// 0x0044E1E0
bool nn::pia::transport::PacketHandler::CheckReceive(const nn::pia::transport::ProtocolMessageReader&)
{
    return false;
}

// 0x0044E21C | fefates:bytes [tier B]
void nn::pia::transport::PacketHandler::FinalizeCore()
{
    if (m_pReceiveAnalyzer != nullptr) {
        delete m_pReceiveAnalyzer;
        m_pReceiveAnalyzer = nullptr;
    }
    if (m_pSendAnalyzer != nullptr) {
        delete m_pSendAnalyzer;
        m_pSendAnalyzer = nullptr;
    }
    m_pWriter = nullptr;
    m_pReader = nullptr;
    m_PayloadSizeLimit = 0;
    m_PacketSizeLimit = 0;
    m_CryptoSetting = common::CryptoSetting();
}

// 0x0044E2AC
void nn::pia::transport::PacketHandler::RelayMessage(const nn::pia::transport::ProtocolMessageReader&)
{
    // empty (in the original too)
}

// 0x0044E2B0 | fefates:bytes-fuzzy
void nn::pia::transport::PacketHandler::NextIteration()
{
    if (IsEndIteration()) {
        return;
    }
    for (;;) {
        if (m_MessageReader.IsValid()) {
            // the next message of the packet
            common::Packet* pPacket = m_pReader->Get(m_IterationIndex);
            m_IterationOffset += m_MessageReader.GetMessageSize();
            if (m_IterationOffset >= pPacket->m_Size - common::Packet::HEADER_SIZE) {
                m_MessageReader.Clear();
            } else {
                m_MessageReader.Attach(*pPacket, m_IterationOffset);
            }
        } else {
            // the first message of the next packet
            m_IterationIndex++;
            m_IterationOffset = 0;
            if (m_IterationIndex >= m_pReader->m_Count) {
                return;
            }
            common::Packet* pPacket = m_pReader->Get(m_IterationIndex);
            if (!pPacket->m_HasMessages) {
                continue;
            }
            m_MessageReader.Attach(*pPacket, m_IterationOffset);
        }
        if (!m_MessageReader.IsValid()) {
            continue;
        }
        u32 protocolId = *reinterpret_cast<const u32*>(m_MessageReader.m_pHeader + 12);
        if ((m_IterationMask & protocolId) != m_IterationProtocolId) {
            continue;
        }

        // the messages of unknown stations only for the protocols without filtering
        bool isKnownStation = true;
        if (common::IsValidPointer(StationManager::s_pInstance)) {
            Station* pStation = StationManager::s_pInstance->GetStation(m_MessageReader.m_SourceAddress);
            if (pStation == nullptr) {
                m_MessageReader.m_SourceAddress.Trace(0x8000000);
                isKnownStation = false;
            } else if (common::IsValidPointer(StationConnectionInfoTable::s_pInstance)) {
                StationConnectionInfo info;
                if (StationConnectionInfoTable::s_pInstance->GetStationConnectionInfo(pStation, &info).IsFailure()) {
                    m_MessageReader.m_SourceAddress.Trace(0x8000000);
                    isKnownStation = false;
                }
            } else if (!pStation->m_Unknown0x69) {
                isKnownStation = false;
            }
        }
        if (!isKnownStation && !(m_MessageReader.m_pHeader[0] & ProtocolMessageReader::FLAG_RELAYED) &&
            ProtocolMessageFilteringManager::s_pInstance->IsFilteringEnabled(__builtin_bswap32(protocolId) >> 16)) {
            continue;
        }
        if (CheckReceive(m_MessageReader)) {
            return;
        }
    }
}

// 0x0044E4E4 | fefates:bytes
void nn::pia::transport::PacketHandler::BeginIteration()
{
    m_IterationIndex = -1;
    m_IterationOffset = 0;
    m_MessageReader.Clear();
    NextIteration();
}

// 0x0044E518 | fefates:bytes [tier B]
nn::Result nn::pia::transport::PacketHandler::InitializeCore(unsigned int destinationNumMax, bool isBroadcast, unsigned int headerSize)
{
    if (destinationNumMax == 0) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    m_DestinationNumMax = destinationNumMax;
    m_IsBroadcast = isBroadcast;
    m_pWriter = nullptr;
    m_pReader = nullptr;
    m_PayloadSizeLimit = 0;
    m_PacketSizeLimit = 0;
    m_pSendAnalyzer = new PacketAnalyzer("Pia Send", headerSize);
    if (!common::IsValidPointer(m_pSendAnalyzer)) {
        m_pSendAnalyzer = nullptr;
        return common::RESULT_OUT_OF_MEMORY;
    }
    m_pReceiveAnalyzer = new PacketAnalyzer("Pia Receive", headerSize);
    if (common::IsValidPointer(m_pReceiveAnalyzer)) {
        m_CryptoSetting.m_Mode = common::Crypto::MODE_NONE;
        return nn::Result();
    }
    if (m_pSendAnalyzer != nullptr) {
        delete m_pSendAnalyzer;
        m_pSendAnalyzer = nullptr;
    }
    m_pReceiveAnalyzer = nullptr;
    return common::RESULT_OUT_OF_MEMORY;
}

// 0x0044E6A0 | fefates:bytes [tier B]
void nn::pia::transport::PacketHandler::EndDispatchCore()
{
    if (m_pReservedWriter != nullptr) {
        return;
    }
    if (common::IsValidPointer(m_pSendAnalyzer)) {
        for (s32 i = m_pWriter->m_Head; i != m_pWriter->m_Position; i = GetNextIndex(m_pWriter, i)) {
            m_pSendAnalyzer->UpdatePacketAnalysisData(*GetStreamPacket(m_pWriter, i), true);
        }
    }
    if (m_CryptoSetting.m_Mode != common::Crypto::MODE_NONE) {
        common::Crypto::Setting setting;
        if (m_CryptoSetting.m_Mode == common::Crypto::MODE_AES128) {
            setting.m_Mode = common::Crypto::MODE_AES128;
            setting.m_pKey = m_CryptoSetting.m_Key;
            setting.m_KeySize = common::CryptoSetting::KEY_SIZE;
        }
        for (s32 i = m_pWriter->m_Head; i != m_pWriter->m_Position; i = GetNextIndex(m_pWriter, i)) {
            if (GetStreamPacket(m_pWriter, i)->m_DestinationBitmap != 0) {
                GetStreamPacket(m_pWriter, i)->Encrypt(setting);
            }
        }
    }
    m_pReader->Release();
    m_pWriter->Push();
}

// 0x0044E814 | fefates:bytes [tier B]
void nn::pia::transport::PacketHandler::BeginDispatchCore()
{
    m_pReader->PullAll();
    common::Crypto::Setting setting;
    if (m_CryptoSetting.m_Mode == common::Crypto::MODE_AES128) {
        setting.m_Mode = common::Crypto::MODE_AES128;
        setting.m_pKey = m_CryptoSetting.m_Key;
        setting.m_KeySize = common::CryptoSetting::KEY_SIZE;
    } else {
        setting.m_Mode = common::Crypto::MODE_NONE;
        setting.m_pKey = nullptr;
        setting.m_KeySize = 0;
    }
    PacketStream::Reader* pReader = m_pReader;
    for (s32 i = pReader->m_Head; i != pReader->m_Position; i = GetNextIndex(pReader, i)) {
        common::Packet* pPacket = GetStreamPacket(pReader, i);
        if (!pPacket->IsValid()) {
            pPacket->m_HasMessages = false;
            continue;
        }
        if (pPacket->m_State != common::Packet::STATE_PLAIN) {
            if (pPacket->Decrypt(setting).IsFailure() || pPacket->m_State != common::Packet::STATE_PLAIN) {
                pPacket->m_HasMessages = false;
                continue;
            }
        }
        u32 size = CheckPacket(*pPacket);
        if (size < common::Packet::HEADER_SIZE) {
            pPacket->m_HasMessages = false;
            continue;
        }
        pPacket->m_Size = size;

        // all messages valid, some to relay?
        bool isValid = true;
        bool hasRelayMessage = false;
        u32 offset = 0;
        while (offset < pPacket->m_Size - common::Packet::HEADER_SIZE) {
            m_MessageReader.Attach(*pPacket, offset);
            if (!m_MessageReader.IsValid()) {
                if (!m_MessageReader.m_IsTerminated) {
                    isValid = false;
                }
                break;
            }
            offset += m_MessageReader.GetMessageSize();
            isValid = isValid & CheckMessage(m_MessageReader);
            if (!isValid) {
                break;
            }
            hasRelayMessage = hasRelayMessage | ((m_MessageReader.m_pHeader[0] & ProtocolMessageReader::FLAG_RELAY_REQUEST) >> 1);
        }
        pPacket->m_HasMessages = isValid;
        if (isValid && hasRelayMessage) {
            for (offset = 0; offset < pPacket->m_Size - common::Packet::HEADER_SIZE; offset += m_MessageReader.GetMessageSize()) {
                m_MessageReader.Attach(*pPacket, offset);
                if (!m_MessageReader.IsValid()) {
                    break;
                }
                if (m_MessageReader.m_pHeader[0] & ProtocolMessageReader::FLAG_RELAY_REQUEST) {
                    RelayMessage(m_MessageReader);
                }
            }
        }
    }
    if (common::IsValidPointer(m_pReceiveAnalyzer)) {
        for (s32 i = m_pReader->m_Head; i != m_pReader->m_Position; i = GetNextIndex(m_pReader, i)) {
            m_pReceiveAnalyzer->UpdatePacketAnalysisData(*GetStreamPacket(m_pReader, i), false);
        }
    }
}

// 0x0044EABC (name is ours)
nn::pia::transport::ProtocolMessageWriter* nn::pia::transport::PacketHandler::AssignByStationKey(const nn::pia::transport::ProtocolId&, unsigned int, unsigned int, bool)
{
    return nullptr;
}

// 0x0044EAC4 | fefates:bytes [tier B]
u8* nn::pia::transport::PacketHandler::AssignPacketPayload(nn::pia::common::Packet* pPacket, unsigned int size)
{
    u8* pPayload = pPacket->AssignPayload(size);
    // the padding of the message
    *reinterpret_cast<u32*>(pPayload + (size & ~3) - 4) = 0;
    return pPayload;
}

// 0x0044EAEC
nn::pia::transport::ProtocolMessageWriter* nn::pia::transport::PacketHandler::AssignByStationIndex(const nn::pia::transport::ProtocolId&, nn::pia::StationIndex, unsigned int, bool)
{
    return nullptr;
}

// 0x0044EAF4 | fefates:bytes [tier B]
nn::pia::transport::ProtocolMessageWriter* nn::pia::transport::PacketHandler::ReserveMessageWriter()
{
    if (m_MessageWriter.m_BufferNum != 0) {
        m_pReservedWriter = &m_MessageWriter;
    }
    return m_pReservedWriter;
}

// 0x0044EB0C
nn::pia::transport::ProtocolMessageWriter* nn::pia::transport::PacketHandler::AssignByStationBitmap(const nn::pia::transport::ProtocolId&, unsigned int, unsigned int, bool)
{
    return nullptr;
}

// 0x0044EB14 | fefates:bytes
nn::pia::transport::ProtocolMessageWriter* nn::pia::transport::PacketHandler::AssignByStationAddress(const nn::pia::transport::ProtocolId& protocolId, const nn::pia::common::StationAddress& address, unsigned int size, bool isOwnPacket)
{
    ProtocolMessageWriter* pWriter = nullptr;
    PacketStream::Writer* pStreamWriter;
    if (!address.IsValid() || (pStreamWriter = m_pWriter) == nullptr) {
        return pWriter;
    }
    if (m_pReservedWriter != nullptr) {
        return pWriter;
    }
    u32 messageSize = ((size + ProtocolMessageWriter::HEADER_SIZE - 1) & ~3) + 4;
    if (messageSize > m_PayloadSizeLimit) {
        return pWriter;
    }
    u32 sizeLimit = m_PacketSizeLimit - messageSize;

    // a packet to the address that still has room, else a new one
    common::Packet* pPacket = nullptr;
    if (!isOwnPacket) {
        for (s32 i = pStreamWriter->m_Head; i != pStreamWriter->m_Position; i = GetNextIndex(pStreamWriter, i)) {
            common::Packet* p = GetStreamPacket(pStreamWriter, i);
            if (!p->m_Unknown0x5D5 && p->m_DestinationStationAddress == address && p->m_Size <= sizeLimit) {
                pPacket = p;
                break;
            }
        }
    }
    if (pPacket == nullptr) {
        pPacket = AssignPacket(STATION_INDEX_UNIDENTIFIED, 0, address, isOwnPacket);
        if (pPacket == nullptr) {
            return pWriter;
        }
    }
    u8* pPayload = pPacket->AssignPayload(messageSize);
    *reinterpret_cast<u32*>(pPayload + (messageSize & ~3) - 4) = 0;
    m_MessageWriter.Reset(protocolId, size, false, isOwnPacket);
    m_MessageWriter.AddMessageBuffer(pPacket, pPayload, 0, false, false);
    m_pReservedWriter = &m_MessageWriter;
    pWriter = &m_MessageWriter;
    return pWriter;
}

// 0x0044ECAC | fefates:bytes [tier B]
void nn::pia::transport::PacketHandler::ClearPacketAnalysisData()
{
    if (common::IsValidPointer(m_pSendAnalyzer)) {
        m_pSendAnalyzer->ClearPacketAnalysisData();
    }
    if (common::IsValidPointer(m_pReceiveAnalyzer)) {
        m_pReceiveAnalyzer->ClearPacketAnalysisData();
    }
}

// 0x0044ECE8 | fefates:bytes [tier B]
nn::Result nn::pia::transport::PacketHandler::Commit()
{
    if (m_pReservedWriter == nullptr) {
        return common::RESULT_INVALID_STATE;
    }
    m_pReservedWriter->Commit();
    m_pReservedWriter = nullptr;
    return nn::Result();
}

// 0x0044ED18 | fefates:bytes [tier B]
nn::pia::transport::PacketHandler::PacketHandler()
    : m_pWriter(nullptr), m_pReader(nullptr), m_pReservedWriter(nullptr), m_Iterator(this), m_pSendAnalyzer(nullptr),
      m_pReceiveAnalyzer(nullptr)
{
}

// 0x0044ED8C | fefates:bytes
// 0x0044ED64 (deleting dtor)
nn::pia::transport::PacketHandler::~PacketHandler()
{
    // empty (in the original too: only the destructors of the reader and the writer)
}

// 0x00734BD0 | fefates:bytes
bool nn::pia::transport::PacketHandler::IsEndIteration() const
{
    return m_IterationIndex >= m_pReader->m_Count;
}

// 0x00734BEC
const pead::RuntimeTypeInfo::Interface* nn::pia::transport::PacketHandler::getRuntimeTypeInfo() const
{
    return getRuntimeTypeInfoStatic();
}

// 0x00734C38 | fefates:bytes [tier B]
u32 nn::pia::transport::PacketHandler::GetPayloadSizeLimit() const
{
    return m_PayloadSizeLimit - ProtocolMessageReader::HEADER_SIZE;
}

// 0x00734C44 | fefates:bytes [tier B]
nn::Result nn::pia::transport::PacketHandler::GetPacketAnalysisData(nn::pia::transport::PacketAnalysisData* pSendData, nn::pia::transport::PacketAnalysisData* pReceiveData) const
{
    if (!common::IsValidPointer(pSendData) && !common::IsValidPointer(pReceiveData)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (!common::IsValidPointer(m_pSendAnalyzer) || !common::IsValidPointer(m_pReceiveAnalyzer)) {
        return common::RESULT_INVALID_STATE;
    }
    if (common::IsValidPointer(pSendData)) {
        nn::Result result = m_pSendAnalyzer->GetPacketAnalysisData(pSendData);
        if (result.IsFailure()) {
            return result;
        }
    }
    if (common::IsValidPointer(pReceiveData)) {
        nn::Result result = m_pReceiveAnalyzer->GetPacketAnalysisData(pReceiveData);
        if (result.IsFailure()) {
            return result;
        }
    }
    return nn::Result();
}

// 0x00734CE0
bool nn::pia::transport::PacketHandler::checkDerivedRuntimeTypeInfo(const pead::RuntimeTypeInfo::Interface* typeInfo) const
{
    return typeInfo == getRuntimeTypeInfoStatic();
}

} // namespace transport
} // namespace pia
} // namespace nn
