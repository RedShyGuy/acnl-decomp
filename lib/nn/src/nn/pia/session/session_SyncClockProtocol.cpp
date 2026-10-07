#include "nn/pia/session/session_SyncClockProtocol.h"
#include "nn/pia/common/common_ByteOrder.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_TimeSpan.h"
#include "nn/pia/session/session_Mesh.h"
#include "nn/pia/transport/transport_PacketHandler.h"
#include "nn/pia/transport/transport_ProtocolEvent.h"
#include "nn/pia/transport/transport_ProtocolId.h"
#include "nn/pia/transport/transport_ProtocolMessageReader.h"
#include "nn/pia/transport/transport_ProtocolMessageWriter.h"
#include "nn/pia/transport/transport_Transport.h"

namespace nn {
namespace pia {
namespace session {
namespace {
// the message: the time of the request (ticks of the client) and the clock of the host (ms)
const u32 MESSAGE_SIZE = 16;
// the round trip times the median is taken of
const u32 MEDIAN_NUM = 10;
} // namespace

// 0x00439028 | fefates:bytes [tier B]
void nn::pia::session::SyncClockProtocol::processByClient(const nn::pia::transport::ProtocolMessageReader* pReader)
{
    const u64* pMessage = reinterpret_cast<const u64*>(pReader->GetPayload());
    s64 requestTime = common::ByteOrder::Swap64(pMessage[0]);
    s64 hostTime = common::ByteOrder::Swap64(pMessage[1]);
    s64 hostTicks = common::TimeSpan::GetTicksPerMSec().GetTick() * hostTime;
    common::Time now;
    now.SetNow();
    s32 rtt = (now.m_Tick - requestTime) / common::TimeSpan::GetTicksPerMSec().GetTick();
    m_Rtts.Add(rtt);
    // the host's clock was read half a round trip ago
    s32 latency = m_Rtts.GetMedian(MEDIAN_NUM) / 2;
    common::Time baseTime(now.m_Tick - hostTicks - common::TimeSpan::GetTicksPerMSec().GetTick() * latency);
    m_SyncClock.SetBaseTime(baseTime);
}

// 0x004393CC slot 0x1C
nn::Result nn::pia::session::SyncClockProtocol::UpdateProtocolEvent(const nn::pia::transport::ProtocolEvent& event)
{
    if (m_LocalStationIndex == STATION_INDEX_UNIDENTIFIED) {
        return common::RESULT_INVALID_STATE;
    }
    StationIndex stationIndex = event.m_StationIndex;
    if (stationIndex > STATION_INDEX_MAX || stationIndex == m_LocalStationIndex) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    Mesh* pMesh = Mesh::s_pInstance;
    if (!common::IsValidPointer(pMesh)) {
        return common::RESULT_INVALID_STATE;
    }
    // the host keeps its clock
    if (pMesh->m_LocalStationIndex <= STATION_INDEX_MAX && pMesh->m_LocalStationIndex == pMesh->m_HostStationIndex) {
        return nn::Result();
    }
    switch (event.m_Type) {
    case transport::ProtocolEvent::TYPE_JOIN:
        if (stationIndex == pMesh->m_HostStationIndex) {
            m_IsRequesting = true;
            m_IsRequestSent = false;
        }
        break;
    case transport::ProtocolEvent::TYPE_LEAVE:
        if (stationIndex == pMesh->m_HostStationIndex) {
            m_IsRequesting = false;
        }
        break;
    default:
        return common::RESULT_INVALID_ARGUMENT;
    }
    return nn::Result();
}

// 0x0043949C slot 0x14 | fefates:bytes
void nn::pia::session::SyncClockProtocol::Cleanup()
{
    m_LocalStationIndex = STATION_INDEX_UNIDENTIFIED;
    m_Rtts.Clear();
}

// 0x004394B4 slot 0x10
nn::Result nn::pia::session::SyncClockProtocol::Startup(nn::pia::StationIndex localStationIndex)
{
    if (localStationIndex > STATION_INDEX_MAX) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_LocalStationIndex != STATION_INDEX_UNIDENTIFIED) {
        return common::RESULT_INVALID_STATE;
    }
    m_LocalStationIndex = localStationIndex;
    return nn::Result();
}

// 0x004394E0 (name is ours)
nn::Result nn::pia::session::SyncClockProtocol::SendRequest()
{
    Mesh* pMesh = Mesh::s_pInstance;
    if (!common::IsValidPointer(pMesh)) {
        return common::RESULT_INVALID_STATE;
    }
    if (pMesh->m_LocalStationIndex <= STATION_INDEX_MAX && pMesh->m_LocalStationIndex == pMesh->m_HostStationIndex) {
        return common::RESULT_INVALID_STATE;
    }
    transport::PacketHandler* pPacketHandler = m_pPacketHandler;
    if (!common::IsValidPointer(pPacketHandler)) {
        return common::RESULT_INVALID_STATE;
    }
    common::Time now;
    now.SetNow();
    u64 message[2];
    message[0] = common::ByteOrder::Swap64(now.m_Tick);
    message[1] = common::ByteOrder::Swap64(0);
    StationIndex hostStationIndex = pMesh->m_HostStationIndex;
    if (hostStationIndex == STATION_INDEX_UNIDENTIFIED) {
        return nn::Result();
    }
    transport::ProtocolMessageWriter* pWriter = pPacketHandler->AssignByStationIndex(m_ProtocolId, hostStationIndex, MESSAGE_SIZE, false);
    if (pWriter == nullptr) {
        return nn::Result();
    }
    pWriter->SetPayload(message);
    nn::Result result = pPacketHandler->Commit();
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result();
}

// 0x004395DC slot 0x18 | fefates:callseq
nn::Result nn::pia::session::SyncClockProtocol::Dispatch()
{
    Mesh* pMesh = Mesh::s_pInstance;
    if (!common::IsValidPointer(pMesh)) {
        return common::RESULT_INVALID_STATE;
    }
    if (m_IsRequesting && m_RequestIntervalMSec > 0) {
        transport::Transport* pTransport = transport::Transport::s_pInstance;
        if (!m_IsRequestSent) {
            SendRequest();
            m_LastRequestTime = pTransport->m_DispatchTime;
            m_IsRequestSent = true;
        } else if ((pTransport->m_DispatchTime - m_LastRequestTime).GetTick() / common::TimeSpan::GetTicksPerMSec().GetTick() > m_RequestIntervalMSec) {
            SendRequest();
            m_LastRequestTime = pTransport->m_DispatchTime;
        }
    }
    if (!common::IsValidPointer(m_pPacketHandler)) {
        return common::RESULT_INVALID_STATE;
    }
    transport::ProtocolId protocolId = m_ProtocolId;
    transport::PacketHandler::Iterator* pIterator = m_pPacketHandler->GetIterator(protocolId);
    while (!pIterator->m_pPacketHandler->IsEndIteration()) {
        const transport::ProtocolMessageReader* pReader = pIterator->GetMessageReader();
        if (pMesh->m_LocalStationIndex <= STATION_INDEX_MAX && pMesh->m_LocalStationIndex == pMesh->m_HostStationIndex) {
            response(pReader->GetSourceStationIndex(), common::ByteOrder::Swap64(*reinterpret_cast<const u64*>(pReader->GetPayload())));
        } else {
            processByClient(pReader);
        }
        pIterator->m_pPacketHandler->NextIteration();
    }
    return nn::Result();
}

// 0x0043976C | fefates:callgraph
nn::Result nn::pia::session::SyncClockProtocol::response(nn::pia::StationIndex stationIndex, unsigned long long requestTime)
{
    if (stationIndex > STATION_INDEX_MAX) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    transport::PacketHandler* pPacketHandler = m_pPacketHandler;
    if (!common::IsValidPointer(pPacketHandler) || m_SyncClock.GetTime() == -1) {
        return common::RESULT_INVALID_STATE;
    }
    u64 message[2];
    message[0] = common::ByteOrder::Swap64(requestTime);
    message[1] = common::ByteOrder::Swap64(m_SyncClock.GetTime());
    transport::ProtocolMessageWriter* pWriter = pPacketHandler->AssignByStationIndex(m_ProtocolId, stationIndex, MESSAGE_SIZE, false);
    if (pWriter == nullptr) {
        return nn::Result();
    }
    pWriter->SetPayload(message);
    return pPacketHandler->Commit();
}

// 0x00439854 | fefates:bytes [tier B]
nn::pia::session::SyncClockProtocol::SyncClockProtocol()
    : m_LocalStationIndex(STATION_INDEX_UNIDENTIFIED), m_RequestIntervalMSec(2000), m_LastRequestTime(), m_IsRequesting(false), m_IsRequestSent(false)
{
    m_Rtts.Clear();
}

// 0x004398C8
// 0x004398A4 (deleting dtor)
nn::pia::session::SyncClockProtocol::~SyncClockProtocol()
{
    // empty (in the original too)
}

// 0x00733900 slot 0x0C
u16 nn::pia::session::SyncClockProtocol::GetProtocolType() const
{
    return transport::PROTOCOL_TYPE_SYNC_CLOCK;
}

// 0x00733908 slot 0x08
void nn::pia::session::SyncClockProtocol::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace session
} // namespace pia
} // namespace nn
