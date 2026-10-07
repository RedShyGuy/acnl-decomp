#include "nn/pia/transport/transport_ReliableSlidingWindow.h"
#include "nn/pia/common/common_SessionStateMonitoringContent.h"
#include "nn/pia/common/common_TimeSpan.h"
#include "nn/pia/transport/transport_PacketHandler.h"
#include "nn/pia/transport/transport_ProtocolMessageReader.h"
#include "nn/pia/transport/transport_ProtocolMessageWriter.h"
#include "nn/pia/transport/transport_Station.h"
#include "nn/pia/transport/transport_StationManager.h"
#include "nn/pia/transport/transport_Transport.h"
#include "nn/nstd/nstd_String.h"
#include "pead/peadTickSpan.h"

namespace nn {
namespace pia {
namespace transport {
namespace {
// the first sequence id of both sides (name is ours)
const u32 FIRST_SEQUENCE_ID = static_cast<u32>(-2001);
// from this size on a transfer is monitored (name is ours)
const u32 MONITORED_SIZE_MIN = 100 * 1024;
} // namespace

// 0x0097E460
nn::pia::transport::ReliableSlidingWindow::Setting nn::pia::transport::ReliableSlidingWindow::s_Setting = {1.25f, 8, nullptr};

// 0x0045A540 | fefates:bytes [tier B]
nn::Result nn::pia::transport::ReliableSlidingWindow::Initialize(unsigned int sendNum, unsigned int receiveNum)
{
    nn::Result result = m_SendBuffer.Initialize(sendNum);
    if (result.IsFailure()) {
        return result;
    }
    result = m_ReceiveBuffer.Initialize(receiveNum);
    if (result.IsFailure()) {
        m_SendBuffer.Finalize();
        return result;
    }
    m_ResendInterval = pead::TickSpan::fromMilliSeconds(500).mSpan;
    return nn::Result();
}

// 0x0045A6D8 | fefates:bytes [tier B]
bool nn::pia::transport::ReliableSlidingWindow::CanPushData(unsigned int size)
{
    if (m_ProtocolId == ProtocolId::INVALID) {
        return false;
    }
    return m_SendBuffer.m_Num - m_SendCount >= GetMessageNum(size);
}

// 0x0045A738 | fefates:callseq [tier C]
nn::Result nn::pia::transport::ReliableSlidingWindow::AnalyzeProtocolMessage(const nn::pia::transport::ProtocolMessageReader& reader)
{
    if (m_ProtocolId == ProtocolId::INVALID) {
        return common::RESULT_INVALID_STATE;
    }
    u32 payloadSize = reader.m_PayloadSize;
    if (payloadSize < sizeof(MessageHeader)) {
        return common::RESULT_INVALID_FORMAT;
    }
    const MessageHeader* pHeader = reinterpret_cast<const MessageHeader*>(reader.GetPayload());

    // the acknowledgement of the sent messages
    if (m_LocalStationIndex != STATION_INDEX_UNIDENTIFIED) {
        s32 ackNum = __builtin_bswap32(pHeader->m_AckSequenceId) - m_SendSequenceId;
        if (ackNum >= 0) {
            if (m_SendCount < static_cast<u32>(ackNum)) {
                return common::RESULT_INVALID_FORMAT;
            }
            if (ackNum > 0) {
                m_SendHead += ackNum;
                if (m_SendHead >= m_SendBuffer.m_Num) {
                    m_SendHead -= m_SendBuffer.m_Num;
                }
                m_SendSequenceId += ackNum;
                m_SendCount -= ackNum;
            }
            u64 ackBitmap = (static_cast<u64>(__builtin_bswap32(pHeader->m_AckBitmap[0])) << 32) | __builtin_bswap32(pHeader->m_AckBitmap[1]);
            if (m_SendCount != 0) {
                for (u32 i = 1; i < m_SendCount; i++) {
                    if (ackBitmap & 1) {
                        GetSendData(i).m_ResendTime = 0xFFFFFFFFFFFFFFFFULL;
                    }
                    ackBitmap >>= 1;
                    if (ackBitmap == 0) {
                        break;
                    }
                }
            }
        }
    }

    // the end of a monitored transfer
    if (m_MonitoredSize != 0 && m_SendCount == 0) {
        if (m_PeerStationIndex < 4) {
            common::g_SessionStateMonitoringContent.m_ReliableTransferSize[m_PeerStationIndex] = m_MonitoredSize;
            common::TimeSpan span = Transport::s_pInstance->GetDispatchTime() - m_MonitoringStartTime;
            common::g_SessionStateMonitoringContent.m_ReliableTransferMSec[m_PeerStationIndex] = span.GetTick() / common::TimeSpan::GetTicksPerMSec().GetTick();
        }
        m_MonitoredSize = 0;
        m_MonitoringStartTime = common::Time();
    }

    // the data
    u16 flags = __builtin_bswap16(pHeader->m_Flags);
    if (!(flags & FLAG_DATA)) {
        return nn::Result();
    }
    u16 dataSize = __builtin_bswap16(pHeader->m_DataSize);
    if (payloadSize != dataSize + sizeof(MessageHeader) || dataSize > DATA_SIZE_MAX) {
        return common::RESULT_INVALID_FORMAT;
    }
    u32 sequenceId = __builtin_bswap32(pHeader->m_SequenceId);
    s32 index = sequenceId - m_ReceiveSequenceId;
    if (index < 0) {
        // received again: the acknowledgement was lost
        m_IsAckRequired = true;
        return nn::Result();
    }
    if (m_ReceiveBuffer.m_Num <= static_cast<u32>(index)) {
        return common::RESULT_BUFFER_IS_FULL;
    }
    while (m_ReceiveCount <= static_cast<u32>(index)) {
        GetReceiveData(m_ReceiveCount)->m_IsReceived = false;
        m_ReceiveCount++;
    }
    ReceiveData* pData = GetReceiveData(index);
    m_IsAckRequired = true;
    if (pData->m_IsReceived) {
        return nn::Result();
    }
    nnnstdMemCpy(pData->m_Data, pHeader + 1, dataSize);
    pData->m_IsReceived = true;
    pData->m_IsLast = (flags & FLAG_LAST) >> 1;
    pData->m_Size = dataSize;

    s32 ackIndex = sequenceId - m_AckSequenceId;
    if (ackIndex == 0) {
        // the messages that are complete now
        bool isNextReceived;
        do {
            isNextReceived = (m_AckBitmap & 1) != 0;
            m_AckSequenceId++;
            m_AckBitmap >>= 1;
            s32 lastIndex = m_AckSequenceId + 64 - m_ReceiveSequenceId;
            if (lastIndex >= 0 && static_cast<u32>(lastIndex) < m_ReceiveCount) {
                ReceiveData* pLast = GetReceiveData(lastIndex);
                if (pLast != nullptr && pLast->m_IsReceived) {
                    m_AckBitmap |= 1ULL << 63;
                }
            }
        } while (isNextReceived);
    } else if (static_cast<u32>(ackIndex - 1) < 64) {
        m_AckBitmap |= 1ULL << (ackIndex - 1);
    }
    return nn::Result();
}

// 0x0045AB34 | fefates:bytes [tier B]
void nn::pia::transport::ReliableSlidingWindow::Cleanup()
{
    if (m_ProtocolId != ProtocolId::INVALID) {
        m_ProtocolId = ProtocolId::INVALID;
        m_PeerStationIndex = STATION_INDEX_UNIDENTIFIED;
        m_LocalStationIndex = STATION_INDEX_UNIDENTIFIED;
        m_DataSizeMax = 0;
    }
}

// 0x0045AB68 | fefates:callseq [tier C]
nn::Result nn::pia::transport::ReliableSlidingWindow::PopData(void* pBuffer, unsigned int* pSize, unsigned int bufferSize)
{
    if (m_ProtocolId == ProtocolId::INVALID) {
        return common::RESULT_INVALID_STATE;
    }
    if (!common::IsValidPointer(pBuffer) || !common::IsValidPointer(pSize)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    // the size of the next complete data
    u32 size = 0;
    u32 messageNum;
    for (u32 i = 0;; i++) {
        if (i >= m_ReceiveCount) {
            if (i >= m_ReceiveBuffer.m_Num) {
                return common::RESULT_INVALID_FORMAT;
            }
            return common::RESULT_NO_DATA;
        }
        ReceiveData* pData = GetReceiveData(i);
        if (!pData->m_IsReceived) {
            return common::RESULT_NO_DATA;
        }
        size += pData->m_Size;
        if (pData->m_IsLast) {
            messageNum = i + 1;
            break;
        }
    }
    if (size > bufferSize) {
        return common::RESULT_INVALID_FORMAT;
    }
    u8* pDst = static_cast<u8*>(pBuffer);
    for (u32 i = 0; i < m_ReceiveCount; i++) {
        ReceiveData* pData = GetReceiveData(i);
        nnnstdMemCpy(pDst, pData->m_Data, pData->m_Size);
        pDst += pData->m_Size;
        if (pData->m_IsLast) {
            break;
        }
    }
    *pSize = size;
    m_ReceiveHead += messageNum;
    if (m_ReceiveHead >= m_ReceiveBuffer.m_Num) {
        m_ReceiveHead -= m_ReceiveBuffer.m_Num;
    }
    m_ReceiveSequenceId += messageNum;
    m_ReceiveCount -= messageNum;
    return nn::Result();
}

// 0x0045ACF4 | fefates:bytes [tier B]
nn::Result nn::pia::transport::ReliableSlidingWindow::Startup(nn::pia::transport::PacketHandler* pPacketHandler, unsigned int protocolId, nn::pia::StationIndex localStationIndex, nn::pia::StationIndex peerStationIndex)
{
    if (m_SendBuffer.m_Num == 0) {
        return common::RESULT_NOT_INITIALIZED;
    }
    if (m_ProtocolId != ProtocolId::INVALID) {
        return common::RESULT_INVALID_STATE;
    }
    if (protocolId == ProtocolId::INVALID.m_Id || localStationIndex > STATION_INDEX_MAX || peerStationIndex > STATION_INDEX_MAX ||
        localStationIndex == peerStationIndex) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    m_ProtocolId.m_Id = protocolId;
    m_LocalStationIndex = localStationIndex;
    m_PeerStationIndex = peerStationIndex;
    m_DataSizeMax = (pPacketHandler->GetPayloadSizeLimit() & ~7) - sizeof(MessageHeader);
    m_SendHead = 0;
    m_SendSequenceId = FIRST_SEQUENCE_ID;
    m_SendCount = 0;
    m_ReceiveHead = 0;
    m_ReceiveSequenceId = FIRST_SEQUENCE_ID;
    m_ReceiveCount = 0;
    m_Now = pead::TickTime().mTick;
    m_AckSequenceId = FIRST_SEQUENCE_ID;
    m_AckBitmap = 0;
    m_IsAckRequired = false;
    m_MonitoringStartTime = common::Time();
    m_MonitoredSize = 0;
    return nn::Result();
}

// 0x0045ADE8 | fefates:bytes-fuzzy [tier B]
nn::Result nn::pia::transport::ReliableSlidingWindow::Dispatch(nn::pia::transport::PacketHandler* pPacketHandler)
{
    if (m_ProtocolId == ProtocolId::INVALID) {
        return common::RESULT_INVALID_STATE;
    }
    m_Now = pead::TickTime().mTick;

    // the time of the next resend: the round trip time to the station (times the factor)
    s32 rtt = -1;
    u64 resendTime = m_ResendInterval;
    Station* pStation = StationManager::s_pInstance->GetStation(m_PeerStationIndex);
    if (common::IsValidPointer(pStation)) {
        rtt = pStation->GetRtt(s_Setting.m_RttSampleNum);
        u64 interval;
        if (rtt > 0) {
            interval = pead::TickSpan::fromMilliSeconds(static_cast<s32>(rtt * s_Setting.m_RttFactor)).mSpan;
        } else {
            interval = m_ResendInterval;
        }
        resendTime = m_Now + interval;
    }

    u32 ackSequenceId = __builtin_bswap32(m_AckSequenceId);
    u32 ackBitmapHigh = __builtin_bswap32(static_cast<u32>(m_AckBitmap >> 32));
    u32 ackBitmapLow = __builtin_bswap32(static_cast<u32>(m_AckBitmap));
    for (u32 i = 0; i < m_SendCount; i++) {
        SendData& data = GetSendData(i);
        if (m_Now < data.m_ResendTime) {
            continue;
        }
        ProtocolId protocolId = m_ProtocolId;
        ProtocolMessageWriter* pWriter = pPacketHandler->AssignByStationIndex(protocolId, m_PeerStationIndex, data.m_Size, false);
        if (!common::IsValidPointer(pWriter)) {
            return nn::Result();
        }
        data.m_Header.m_AckSequenceId = ackSequenceId;
        data.m_Header.m_AckBitmap[0] = ackBitmapHigh;
        data.m_Header.m_AckBitmap[1] = ackBitmapLow;
        pWriter->SetPayload(&data.m_Header);
        pPacketHandler->Commit();
        if (s_Setting.m_pResendIntervalCallback != nullptr) {
            s32 msec = s_Setting.m_pResendIntervalCallback(rtt, data.m_ResendCount);
            data.m_ResendTime = m_Now + pead::TickSpan::fromMilliSeconds(msec).mSpan;
        } else {
            data.m_ResendTime = resendTime;
        }
        m_IsAckRequired = false;
        u16& resendCountMax = common::g_SessionStateMonitoringContent.m_ReliableResendCountMax;
        if (resendCountMax == 0xFFFF || data.m_ResendCount > resendCountMax) {
            resendCountMax = data.m_ResendCount;
        }
        data.m_ResendCount++;
    }

    // an acknowledgement without data
    if (m_IsAckRequired) {
        ProtocolId protocolId = m_ProtocolId;
        ProtocolMessageWriter* pWriter = pPacketHandler->AssignByStationIndex(protocolId, m_PeerStationIndex, sizeof(MessageHeader), false);
        if (common::IsValidPointer(pWriter)) {
            MessageHeader header;
            header.m_Flags = 0;
            header.m_DataSize = 0;
            header.m_Reserved = 0;
            header.m_SequenceId = 0;
            header.m_AckSequenceId = ackSequenceId;
            header.m_AckBitmap[0] = ackBitmapHigh;
            header.m_AckBitmap[1] = ackBitmapLow;
            pWriter->SetPayload(&header);
            pPacketHandler->Commit();
            m_IsAckRequired = false;
        }
    }
    return nn::Result();
}

// 0x0045B14C | fefates:bytes [tier B]
void nn::pia::transport::ReliableSlidingWindow::Finalize()
{
    m_ReceiveBuffer.Finalize();
    m_SendBuffer.Finalize();
}

// 0x0045B1A8 | fefates:bytes [tier B]
nn::Result nn::pia::transport::ReliableSlidingWindow::PushData(const void* pData, unsigned int size)
{
    if (m_ProtocolId == ProtocolId::INVALID) {
        return common::RESULT_INVALID_STATE;
    }
    if (!common::IsValidPointer(pData) && size != 0) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    u32 messageNum = GetMessageNum(size);
    if (messageNum > m_SendBuffer.m_Num - m_SendCount) {
        return common::RESULT_BUFFER_IS_FULL;
    }
    if (m_MonitoredSize != 0 || size >= MONITORED_SIZE_MIN) {
        if (m_MonitoredSize == 0) {
            m_MonitoringStartTime = Transport::s_pInstance->GetDispatchTime();
        }
        m_MonitoredSize += size;
    }
    const u8* pSrc = static_cast<const u8*>(pData);
    for (u32 i = 0; i < messageNum - 1; i++) {
        SendData& data = GetSendData(m_SendCount);
        data.m_Header.m_Flags = __builtin_bswap16(FLAG_DATA);
        data.m_Header.m_SequenceId = __builtin_bswap32(m_SendCount + m_SendSequenceId);
        m_SendCount++;
        nnnstdMemCpy(data.m_Data, pSrc, m_DataSizeMax);
        data.m_Header.m_DataSize = __builtin_bswap16(m_DataSizeMax);
        data.m_Size = m_DataSizeMax + sizeof(MessageHeader);
        data.m_ResendTime = 0;
        data.m_ResendCount = 0;
        pSrc += m_DataSizeMax;
        size -= m_DataSizeMax;
    }
    SendData& data = GetSendData(m_SendCount);
    data.m_Header.m_Flags = __builtin_bswap16(FLAG_DATA);
    data.m_Header.m_SequenceId = __builtin_bswap32(m_SendSequenceId + m_SendCount);
    m_SendCount++;
    if (size != 0) {
        nnnstdMemCpy(data.m_Data, pSrc, size);
    }
    data.m_Header.m_Flags |= __builtin_bswap16(FLAG_LAST);
    data.m_Header.m_DataSize = __builtin_bswap16(size);
    data.m_ResendTime = 0;
    data.m_Size = size + sizeof(MessageHeader);
    data.m_ResendCount = 0;
    return nn::Result();
}

// 0x0045B3E0 | fefates:bytes [tier B]
nn::pia::transport::ReliableSlidingWindow::ReliableSlidingWindow()
    : m_ProtocolId(ProtocolId::INVALID), m_LocalStationIndex(STATION_INDEX_UNIDENTIFIED),
      m_PeerStationIndex(STATION_INDEX_UNIDENTIFIED), m_SendHead(0), m_SendSequenceId(0), m_SendCount(0), m_ReceiveHead(0),
      m_ReceiveSequenceId(0), m_ReceiveCount(0), m_DataSizeMax(0), m_MonitoredSize(0)
{
}

// 0x0045B45C | fefates:bytes
// 0x0045B44C (deleting dtor)
nn::pia::transport::ReliableSlidingWindow::~ReliableSlidingWindow()
{
    Finalize();
}

// 0x00736204 | fefates:bytes [tier B]
bool nn::pia::transport::ReliableSlidingWindow::IsInCommunication() const
{
    return m_ProtocolId != ProtocolId::INVALID;
}

// 0x00736224
void nn::pia::transport::ReliableSlidingWindow::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace transport
} // namespace pia
} // namespace nn
