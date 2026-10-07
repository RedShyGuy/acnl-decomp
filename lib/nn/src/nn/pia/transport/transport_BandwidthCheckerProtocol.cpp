#include "nn/pia/transport/transport_BandwidthCheckerProtocol.h"
#include "nn/pia/common/common_NewArray.h"
#include "nn/pia/common/common_PayloadSizeManager.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_SessionBeginMonitoringContent.h"
#include "nn/pia/transport/transport_PacketHandler.h"
#include "nn/pia/transport/transport_ProtocolId.h"
#include "nn/pia/transport/transport_ProtocolMessageReader.h"
#include "nn/pia/transport/transport_ProtocolMessageWriter.h"
#include "nn/pia/transport/transport_StationManager.h"

namespace nn {
namespace pia {
namespace transport {
namespace {
// the flag of the Trace calls
const u64 TRACE_FLAG = 0x80000;
// the size of the DATA packet buffer
const u32 BUFFER_SIZE = 0x596;
// the bytes of a packet besides the payload of the message (and the signature)
const u32 PACKET_OVERHEAD = 60;
// the intervals of REQUEST, ACCEPT and RESULT (ms)
const s32 REQUEST_INTERVAL_MSEC = 150;
const s32 ACCEPT_INTERVAL_MSEC = 100;
const s32 RESULT_INTERVAL_MSEC = 100;
// the DATA packets per dispatch at most
const int SEND_NUM_PER_DISPATCH = 4;
} // namespace

// 0x0045D458 (name is ours)
void nn::pia::transport::BandwidthCheckerProtocol::Cancel()
{
    if (m_SendState == SEND_STATE_REQUESTING || m_SendState == SEND_STATE_SENDING || m_SendState == SEND_STATE_WAITING_RESULT) {
        m_IsCancelRequested = true;
    }
}

// 0x0045D474 (name is ours)
nn::Result nn::pia::transport::BandwidthCheckerProtocol::Initialize()
{
    if (m_IsInitialized) {
        return common::RESULT_ALREADY_INITIALIZED;
    }
    m_pBuffer = common::NewArray<u8>(BUFFER_SIZE);
    if (m_pBuffer == nullptr) {
        return common::RESULT_OUT_OF_MEMORY;
    }
    m_SendState = SEND_STATE_NONE;
    m_RequestTime = common::Time();
    m_IsAccepted = false;
    m_SendStartTime = common::Time();
    m_Result = -1;
    m_TargetStationIndex = STATION_INDEX_UNIDENTIFIED;
    m_SentNum = 0;
    m_PacketLoss = -1;
    m_IsCancelRequested = false;
    ResetReceive();
    m_Bandwidth = -1;
    m_PacketSize = 0;
    m_TimeoutMSec = 0;
    m_DurationMSec = 0;
    m_IsSetup = false;
    m_IsInitialized = true;
    return nn::Result();
}

// 0x0045D5A0 (name is ours)
void nn::pia::transport::BandwidthCheckerProtocol::DispatchSend()
{
    switch (m_SendState) {
    case SEND_STATE_REQUESTING: {
        common::Time now;
        now.SetNow();
        if ((now - m_RequestTime).GetTick() / common::TimeSpan::GetTicksPerMSec().GetTick() > REQUEST_INTERVAL_MSEC) {
            nn::Result result = SendMessage(m_TargetStationIndex, MESSAGE_TYPE_REQUEST, -1, -1, -1);
            if (result.IsFailure() && result == common::RESULT_NOT_FOUND) {
                m_IsCancelRequested = true;
            }
            m_RequestTime.SetNow();
        }
        break;
    }
    case SEND_STATE_SENDING: {
        common::Time now;
        now.SetNow();
        s32 elapsed = (now - m_SendStartTime).GetTick() / common::TimeSpan::GetTicksPerMSec().GetTick();
        s32 sentBits = m_PacketSize * 8 * m_SentNum;
        for (int i = 0; i < SEND_NUM_PER_DISPATCH; i++) {
            // 130 % of the bandwidth
            s32 bits = static_cast<s32>(static_cast<f32>(m_Bandwidth) * 130.0f * 0.01f * static_cast<f32>(elapsed) * 0.001f);
            if (bits <= sentBits) {
                continue;
            }
            Message* pMessage = reinterpret_cast<Message*>(m_pBuffer);
            pMessage->m_Type = __builtin_bswap32(MESSAGE_TYPE_DATA);
            pMessage->m_Value1 = __builtin_bswap32(m_SentNum);
            pMessage->m_Value0 = __builtin_bswap32(-1);
            pMessage->m_Value2 = __builtin_bswap32(-1);
            u32 size = m_PacketSize - common::PayloadSizeManager::s_pInstance->m_SignatureSize - PACKET_OVERHEAD;
            if (m_pPacketHandler->GetPayloadSizeLimit() < size) {
                Trace(TRACE_FLAG);
                size = m_pPacketHandler->GetPayloadSizeLimit();
            }
            nn::Result result = Send(m_TargetStationIndex, m_pBuffer, size);
            if (result.IsSuccess()) {
                m_SentNum++;
                result = nn::Result();
            }
            if (result.IsFailure() && result == common::RESULT_NOT_FOUND) {
                m_IsCancelRequested = true;
            }
        }
        break;
    }
    default:
        break;
    }

    switch (m_ReceiveState) {
    case RECEIVE_STATE_ACCEPTING: {
        common::Time now;
        now.SetNow();
        if ((now - m_AcceptMessageTime).GetTick() / common::TimeSpan::GetTicksPerMSec().GetTick() > ACCEPT_INTERVAL_MSEC) {
            SendMessage(m_SenderStationIndex, MESSAGE_TYPE_ACCEPT, -1, -1, -1);
            m_AcceptMessageTime.SetNow();
        }
        break;
    }
    case RECEIVE_STATE_SENDING_RESULT: {
        common::Time now;
        now.SetNow();
        if ((now - m_ResultTime).GetTick() / common::TimeSpan::GetTicksPerMSec().GetTick() > RESULT_INTERVAL_MSEC) {
            // bits per second
            s32 result = static_cast<s32>(static_cast<f32>(m_ReceivedSize) * 8.0f * 1000.0f / static_cast<f32>(m_DurationMSec));
            SendMessage(m_SenderStationIndex, MESSAGE_TYPE_RESULT, result, -1, m_ReceivedNum);
            m_ResultTime.SetNow();
        }
        break;
    }
    default:
        break;
    }
}

// 0x0045D9F8 (name is ours)
void nn::pia::transport::BandwidthCheckerProtocol::DispatchReceive()
{
    ProtocolId protocolId = m_ProtocolId;
    PacketHandler::Iterator* pIterator = m_pPacketHandler->GetIterator(protocolId);
    if (pIterator->m_pPacketHandler->IsEndIteration()) {
        UpdateSendState();
        UpdateReceiveState();
        return;
    }
    do {
        const ProtocolMessageReader* pReader = pIterator->GetMessageReader();
        const Message* pMessage = reinterpret_cast<const Message*>(pReader->GetPayload());
        s32 type = __builtin_bswap32(pMessage->m_Type);
        s32 value0 = __builtin_bswap32(pMessage->m_Value0);
        s32 value1 = __builtin_bswap32(pMessage->m_Value1);
        s32 value2 = __builtin_bswap32(pMessage->m_Value2);
        switch (type) {
        case MESSAGE_TYPE_REQUEST:
            if (m_ReceiveState == RECEIVE_STATE_NONE) {
                m_SenderStationIndex = pReader->GetSourceStationIndex();
                m_SenderAddress = pReader->m_SourceAddress;
                m_IsRequested = true;
                Trace(TRACE_FLAG);
            }
            break;
        case MESSAGE_TYPE_ACCEPT:
            if (m_SendState == SEND_STATE_REQUESTING) {
                if (m_TargetStationIndex == pReader->GetSourceStationIndex()) {
                    m_IsAccepted = true;
                }
                Trace(TRACE_FLAG);
            }
            break;
        case MESSAGE_TYPE_DATA:
            switch (m_ReceiveState) {
            case RECEIVE_STATE_ACCEPTING:
                if (m_SenderStationIndex == pReader->GetSourceStationIndex() && m_SenderAddress == pReader->m_SourceAddress) {
                    m_FirstSequenceNum = value1;
                    m_LastSequenceNum = value1;
                    Trace(TRACE_FLAG);
                }
                break;
            case RECEIVE_STATE_RECEIVING:
                if (m_SenderStationIndex == pReader->GetSourceStationIndex() && m_SenderAddress == pReader->m_SourceAddress) {
                    m_ReceivedSize += pReader->m_PayloadSize + common::PayloadSizeManager::s_pInstance->m_SignatureSize + PACKET_OVERHEAD;
                    m_ReceivedNum++;
                } else {
                    pReader->m_SourceAddress.Trace(TRACE_FLAG);
                }
                break;
            default:
                break;
            }
            break;
        case MESSAGE_TYPE_RESULT:
            if (m_SendState == SEND_STATE_SENDING || m_SendState == SEND_STATE_WAITING_RESULT) {
                m_Result = value0;
                common::g_SessionBeginMonitoringContent.m_BandwidthCheckResult = value0;
                // the packets that should have arrived
                s32 packetNum = static_cast<s32>(1.3f * (static_cast<f32>(m_Bandwidth) * 0.125f / static_cast<f32>(m_PacketSize)) *
                                                 (static_cast<f32>(m_DurationMSec) * 0.001f));
                s32 packetLoss = 10000 - value2 * 10000 / (packetNum + 1);
                m_PacketLoss = packetLoss >= 0 ? packetLoss : 0;
                common::g_SessionBeginMonitoringContent.m_BandwidthCheckPacketLoss = m_PacketLoss;
            }
            break;
        default:
            break;
        }
        UpdateSendState();
        UpdateReceiveState();
        pIterator->m_pPacketHandler->NextIteration();
    } while (!pIterator->m_pPacketHandler->IsEndIteration());
}

// (inline in DispatchSend and SendMessage; name is ours)
inline nn::Result nn::pia::transport::BandwidthCheckerProtocol::Send(StationIndex stationIndex, const void* pData, u32 size)
{
    StationManager* pManager = StationManager::s_pInstance;
    PacketHandler* pPacketHandler = m_pPacketHandler;
    // the pointer check of the original evaluates its argument twice (a macro): two calls
    if (!(reinterpret_cast<uptr>(pManager->GetStation(stationIndex)) >= 0x00100000 &&
          reinterpret_cast<uptr>(pManager->GetStation(stationIndex)) < 0x40000000)) {
        return common::RESULT_NOT_FOUND;
    }
    nn::Result result;
    ProtocolMessageWriter* pWriter = pPacketHandler->AssignByStationIndex(m_ProtocolId, stationIndex, size, false);
    if (pWriter != nullptr) {
        pWriter->SetPayload(pData);
        result = pPacketHandler->Commit();
    }
    return result;
}

// 0x0045DCE4 (name is ours)
nn::Result nn::pia::transport::BandwidthCheckerProtocol::SendMessage(nn::pia::StationIndex stationIndex, int type, int value0, int value1, int value2)
{
    Message message;
    message.m_Type = __builtin_bswap32(type);
    message.m_Value2 = __builtin_bswap32(value2);
    message.m_Value1 = __builtin_bswap32(value1);
    message.m_Value0 = __builtin_bswap32(value0);
    return Send(stationIndex, &message, sizeof(message));
}

// 0x0045DDC8 (name is ours)
void nn::pia::transport::BandwidthCheckerProtocol::UpdateSendState()
{
    switch (m_SendState) {
    case SEND_STATE_NONE:
        if (m_TargetStationIndex != STATION_INDEX_UNIDENTIFIED) {
            m_SendState = SEND_STATE_REQUESTING;
        }
        break;
    case SEND_STATE_REQUESTING:
        if (m_IsCancelRequested) {
            m_SendState = SEND_STATE_FAILED;
            m_IsCancelRequested = false;
        } else if (m_IsAccepted) {
            m_SendState = SEND_STATE_SENDING;
            m_SendStartTime.SetNow();
        }
        break;
    case SEND_STATE_SENDING:
        if (m_IsCancelRequested) {
            m_SendState = SEND_STATE_FAILED;
            m_IsCancelRequested = false;
        } else if (m_Result != -1) {
            m_SendState = SEND_STATE_SUCCEEDED;
        } else {
            common::Time now;
            now.SetNow();
            if ((now - m_SendStartTime).GetTick() / common::TimeSpan::GetTicksPerMSec().GetTick() > m_TimeoutMSec) {
                m_SendState = SEND_STATE_WAITING_RESULT;
            }
        }
        break;
    case SEND_STATE_WAITING_RESULT:
        if (m_IsCancelRequested) {
            m_SendState = SEND_STATE_FAILED;
            m_IsCancelRequested = false;
        } else if (m_Result != -1) {
            m_SendState = SEND_STATE_SUCCEEDED;
        } else {
            common::Time now;
            now.SetNow();
            if ((now - m_SendStartTime).GetTick() / common::TimeSpan::GetTicksPerMSec().GetTick() > m_TimeoutMSec + 2000) {
                m_SendState = SEND_STATE_FAILED;
            }
        }
        break;
    default:
        break;
    }
}

// 0x0045DF54 (name is ours)
void nn::pia::transport::BandwidthCheckerProtocol::UpdateReceiveState()
{
    switch (m_ReceiveState) {
    case RECEIVE_STATE_NONE:
        if (m_IsRequested) {
            m_ReceiveState = RECEIVE_STATE_ACCEPTING;
            m_AcceptTime.SetNow();
        }
        break;
    case RECEIVE_STATE_ACCEPTING:
        if (m_SenderStationIndex != STATION_INDEX_UNIDENTIFIED) {
            m_ReceiveState = RECEIVE_STATE_RECEIVING;
            m_ReceiveStartTime.SetNow();
        } else {
            common::Time now;
            now.SetNow();
            if ((now - m_AcceptTime).GetTick() / common::TimeSpan::GetTicksPerMSec().GetTick() > 2000) {
                ResetReceive();
            }
        }
        break;
    case RECEIVE_STATE_RECEIVING: {
        common::Time now;
        now.SetNow();
        if ((now - m_ReceiveStartTime).GetTick() / common::TimeSpan::GetTicksPerMSec().GetTick() > m_DurationMSec) {
            m_ReceiveState = RECEIVE_STATE_SENDING_RESULT;
        }
        break;
    }
    case RECEIVE_STATE_SENDING_RESULT: {
        common::Time now;
        now.SetNow();
        if ((now - m_ReceiveStartTime).GetTick() / common::TimeSpan::GetTicksPerMSec().GetTick() > m_DurationMSec + 1000) {
            // now the other direction
            if (m_Result == -1 && m_TargetStationIndex == STATION_INDEX_UNIDENTIFIED && !m_IsOneWay) {
                StartCheck(m_SenderStationIndex);
            }
            ResetReceive();
        }
        break;
    }
    default:
        break;
    }
}

// 0x0045E178 (name is ours)
nn::Result nn::pia::transport::BandwidthCheckerProtocol::Setup(int bandwidth, unsigned int packetSize, bool isOneWay, int durationMSec)
{
    common::PayloadSizeManager* pManager = common::PayloadSizeManager::s_pInstance;
    if (bandwidth <= 0 || bandwidth > 1000000 || (packetSize & 3) != 0 || packetSize > pManager->m_MtuSize + 28 ||
        packetSize < pManager->m_SignatureSize + 76 || durationMSec <= 0) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (!m_IsInitialized) {
        return common::RESULT_NOT_INITIALIZED;
    }
    m_Bandwidth = bandwidth;
    common::g_SessionBeginMonitoringContent.m_BandwidthCheckBandwidth = bandwidth;
    m_PacketSize = packetSize;
    common::g_SessionBeginMonitoringContent.m_BandwidthCheckPacketSize = packetSize;
    m_DurationMSec = durationMSec;
    m_TimeoutMSec = durationMSec + 250;
    m_IsOneWay = isOneWay;
    m_IsSetup = true;
    return nn::Result();
}

// 0x0045E23C
nn::Result nn::pia::transport::BandwidthCheckerProtocol::Dispatch()
{
    if (!m_IsInitialized) {
        return common::RESULT_NOT_INITIALIZED;
    }
    if (!m_IsSetup) {
        return common::RESULT_INVALID_STATE;
    }
    DispatchSend();
    DispatchReceive();
    return nn::Result();
}

// 0x0045E284 (name is ours)
void nn::pia::transport::BandwidthCheckerProtocol::Finalize()
{
    if (!m_IsInitialized) {
        return;
    }
    m_IsInitialized = false;
    m_IsSetup = false;
    if (m_pBuffer != nullptr) {
        common::DeleteArray(m_pBuffer);
    }
    m_pBuffer = nullptr;
}

// 0x0045E2C8 (name is ours)
nn::Result nn::pia::transport::BandwidthCheckerProtocol::StartCheck(nn::pia::StationIndex stationIndex)
{
    if (stationIndex > STATION_INDEX_MAX) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (!m_IsInitialized) {
        return common::RESULT_NOT_INITIALIZED;
    }
    if (!m_IsSetup || (m_SendState != SEND_STATE_NONE && m_SendState != SEND_STATE_SUCCEEDED && m_SendState != SEND_STATE_FAILED)) {
        return common::RESULT_INVALID_STATE;
    }
    ResetSend();
    m_TargetStationIndex = stationIndex;
    return nn::Result();
}

// 0x0045E360
nn::pia::transport::BandwidthCheckerProtocol::BandwidthCheckerProtocol()
    : m_IsInitialized(false), m_IsSetup(false), m_IsOneWay(false), m_pBuffer(nullptr), m_TimeoutMSec(0), m_DurationMSec(0)
{
}

// 0x0045E418
// 0x0045E3E8 (deleting dtor)
nn::pia::transport::BandwidthCheckerProtocol::~BandwidthCheckerProtocol()
{
    // empty (in the original too)
}

// 0x007367A4
u16 nn::pia::transport::BandwidthCheckerProtocol::GetProtocolType() const
{
    return PROTOCOL_TYPE_BANDWIDTH_CHECKER;
}

// 0x007367AC
bool nn::pia::transport::BandwidthCheckerProtocol::IsEnableProtocolFiltering() const
{
    return false;
}

// 0x007367B4
void nn::pia::transport::BandwidthCheckerProtocol::Trace(u64) const
{
    // empty (in the original too)
}

// 0x0073677C (name is ours)
bool nn::pia::transport::BandwidthCheckerProtocol::IsCheckDone() const
{
    return m_IsInitialized && (m_SendState == SEND_STATE_SUCCEEDED || m_SendState == SEND_STATE_FAILED);
}

// 0x007367B8 (name is ours)
bool nn::pia::transport::BandwidthCheckerProtocol::IsCheckStartable() const
{
    return m_IsInitialized && m_IsSetup &&
           (m_SendState == SEND_STATE_NONE || m_SendState == SEND_STATE_SUCCEEDED || m_SendState == SEND_STATE_FAILED);
}

} // namespace transport
} // namespace pia
} // namespace nn
