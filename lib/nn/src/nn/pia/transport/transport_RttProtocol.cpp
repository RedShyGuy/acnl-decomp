#include "nn/pia/transport/transport_RttProtocol.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_TimeSpan.h"
#include "nn/pia/transport/transport_PacketHandler.h"
#include "nn/pia/transport/transport_PacketHandler_Iterator.h"
#include "nn/pia/transport/transport_ProtocolEvent.h"
#include "nn/pia/transport/transport_ProtocolMessageReader.h"
#include "nn/pia/transport/transport_ProtocolMessageWriter.h"
#include "nn/pia/transport/transport_RttCalculator.h"
#include "nn/pia/transport/transport_Transport.h"

namespace nn {
namespace pia {
namespace transport {
// (inline in the original)
inline void nn::pia::transport::RttProtocol::SendRequest(StationIndex stationIndex)
{
    if ((stationIndex < static_cast<StationIndex>(m_StationNum) || stationIndex == 254) && m_LocalStationIndex != STATION_INDEX_UNIDENTIFIED) {
        Data request = {DATA_TYPE_REQUEST, 0, 0};
        common::Time now;
        now.SetNow();
        request.m_Time = now.m_Tick;
        send(stationIndex, request);
    }
}

// 0x0044D3FC (name is ours)
nn::Result nn::pia::transport::RttProtocol::Initialize(unsigned int stationNum)
{
    if (common::IsValidPointer(m_pCalculators)) {
        return common::RESULT_ALREADY_INITIALIZED;
    }
    if (stationNum == 0 || stationNum > STATION_INDEX_MAX + 1) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    m_pCalculators = new RttCalculator[stationNum];
    if (!common::IsValidPointer(m_pCalculators)) {
        return common::RESULT_OUT_OF_MEMORY;
    }
    m_StationNum = stationNum;
    return nn::Result();
}

// 0x0044D4A4
nn::Result nn::pia::transport::RttProtocol::UpdateProtocolEvent(const nn::pia::transport::ProtocolEvent& event)
{
    if (m_LocalStationIndex == STATION_INDEX_UNIDENTIFIED) {
        return common::RESULT_INVALID_STATE;
    }
    if (event.m_StationIndex >= static_cast<StationIndex>(m_StationNum) || event.m_StationIndex == m_LocalStationIndex) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    switch (event.m_Type) {
    case ProtocolEvent::TYPE_JOIN:
        m_pCalculators[event.m_StationIndex].Startup();
        SendRequest(event.m_StationIndex);
        m_pCalculators[event.m_StationIndex].SetSendTime(Transport::s_pInstance->GetDispatchTime());
        return nn::Result();
    case ProtocolEvent::TYPE_LEAVE:
        m_pCalculators[event.m_StationIndex].Cleanup();
        return nn::Result();
    default:
        return common::RESULT_INVALID_ARGUMENT;
    }
}

// 0x0044D5C0 | fefates:bytes [tier B]
nn::Result nn::pia::transport::RttProtocol::send(nn::pia::StationIndex stationIndex, const nn::pia::transport::RttProtocol::Data& data)
{
    PacketHandler* pPacketHandler = m_pPacketHandler;
    if (!common::IsValidPointer(pPacketHandler)) {
        return common::RESULT_INVALID_STATE;
    }
    if (m_LocalStationIndex == stationIndex) {
        return common::RESULT_INVALID_STATE;
    }
    Data payload;
    payload.m_Type = __builtin_bswap32(data.m_Type);
    payload.m_Reserved = 0;
    payload.m_Time = __builtin_bswap64(data.m_Time);
    ProtocolId protocolId = m_ProtocolId;
    ProtocolMessageWriter* pWriter = pPacketHandler->AssignByStationIndex(protocolId, stationIndex, sizeof(Data), false);
    if (pWriter == nullptr) {
        return nn::Result();
    }
    pWriter->SetPayload(&payload);
    nn::Result result = pPacketHandler->Commit();
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result();
}

// 0x0044D6D0 | fefates:bytes
void nn::pia::transport::RttProtocol::Cleanup()
{
    m_LocalStationIndex = STATION_INDEX_UNIDENTIFIED;
    if (common::IsValidPointer(m_pCalculators)) {
        for (u32 i = 0; i < m_StationNum; i++) {
            m_pCalculators[i].Cleanup();
        }
    }
}

// 0x0044D72C
nn::Result nn::pia::transport::RttProtocol::Startup(nn::pia::StationIndex stationIndex)
{
    if (common::IsValidPointer(m_pCalculators)) {
        if (stationIndex > STATION_INDEX_MAX) {
            return common::RESULT_INVALID_ARGUMENT;
        }
        if (m_LocalStationIndex == STATION_INDEX_UNIDENTIFIED) {
            m_LocalStationIndex = stationIndex;
            return nn::Result();
        }
    }
    return common::RESULT_INVALID_STATE;
}

// 0x0044D778 | fefates:callseq
nn::Result nn::pia::transport::RttProtocol::Dispatch()
{
    if (m_LocalStationIndex == STATION_INDEX_UNIDENTIFIED) {
        return nn::Result();
    }
    PacketHandler* pPacketHandler = m_pPacketHandler;
    if (!common::IsValidPointer(pPacketHandler)) {
        return common::RESULT_INVALID_STATE;
    }

    // the requests to the stations whose answer is overdue
    for (u32 i = 0; i < m_StationNum; i++) {
        RttCalculator& calculator = m_pCalculators[i];
        if (calculator.m_IsActive && calculator.IsTimeOut()) {
            SendRequest(static_cast<StationIndex>(i));
            calculator.SetSendTime(Transport::s_pInstance->GetDispatchTime());
        }
    }

    // the received requests and answers
    ProtocolId protocolId = m_ProtocolId;
    PacketHandler::Iterator* pIterator = pPacketHandler->GetIterator(protocolId);
    while (!pIterator->m_pPacketHandler->IsEndIteration()) {
        const ProtocolMessageReader* pReader = pIterator->GetMessageReader();
        StationIndex sourceIndex = pReader->GetSourceStationIndex();
        if (sourceIndex != m_LocalStationIndex) {
            const u8* pPayload = pReader->GetPayload();
            u32 type = __builtin_bswap32(*reinterpret_cast<const u32*>(pPayload));
            u64 time = __builtin_bswap64(*reinterpret_cast<const u64*>(pPayload + 8));
            if (type == DATA_TYPE_REQUEST) {
                Data response = {DATA_TYPE_RESPONSE, 0, 0};
                response.m_Time = time;
                send(pReader->GetSourceStationIndex(), response);
            } else if (type == DATA_TYPE_RESPONSE) {
                common::Time now;
                now.SetNow();
                s32 rtt = (now.m_Tick - static_cast<s64>(time)) / common::TimeSpan::GetTicksPerMSec().GetTick();
                StationIndex stationIndex = pReader->GetSourceStationIndex();
                if (stationIndex < static_cast<StationIndex>(m_StationNum)) {
                    m_pCalculators[stationIndex].Update(rtt);
                }
            }
        }
        pIterator->m_pPacketHandler->NextIteration();
    }
    return nn::Result();
}

// 0x0044DA2C | fefates:bytes [tier B]
void nn::pia::transport::RttProtocol::Finalize()
{
    if (common::IsValidPointer(m_pCalculators)) {
        m_StationNum = 0;
        delete[] m_pCalculators;
        m_pCalculators = nullptr;
    }
}

// 0x0044DA6C | fefates:bytes [tier B]
nn::pia::transport::RttProtocol::RttProtocol() : m_LocalStationIndex(STATION_INDEX_UNIDENTIFIED), m_pCalculators(nullptr), m_StationNum(0)
{
}

// 0x0044DAA8
// 0x0044DA98 (deleting dtor)
nn::pia::transport::RttProtocol::~RttProtocol()
{
    // empty (in the original too)
}

// 0x00734B70
u16 nn::pia::transport::RttProtocol::GetProtocolType() const
{
    return PROTOCOL_TYPE_RTT;
}

// 0x00734B78
void nn::pia::transport::RttProtocol::Trace(u64) const
{
    // empty (in the original too)
}

// 0x00734B7C | fefates:bytes [tier B]
s32 nn::pia::transport::RttProtocol::GetRtt(nn::pia::StationIndex stationIndex) const
{
    if (static_cast<StationIndex>(m_StationNum) <= stationIndex) {
        return -1;
    }
    return m_pCalculators[stationIndex].GetRtt();
}

// 0x00734BA4 | fefates:bytes [tier B]
s32 nn::pia::transport::RttProtocol::GetRtt(nn::pia::StationIndex stationIndex, unsigned int num) const
{
    if (static_cast<StationIndex>(m_StationNum) <= stationIndex) {
        return -1;
    }
    return m_pCalculators[stationIndex].GetRtt(num);
}

} // namespace transport
} // namespace pia
} // namespace nn
