#include "nn/pia/transport/transport_StationProtocolReliable.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_StationAddress.h"
#include "nn/pia/common/common_Watermark.h"
#include "nn/pia/common/common_WatermarkManager.h"
#include "nn/pia/transport/transport_PacketHandler.h"
#include "nn/pia/transport/transport_ProtocolEvent.h"
#include "nn/pia/transport/transport_ProtocolId.h"
#include "nn/pia/transport/transport_ProtocolMessageReader.h"
#include "nn/pia/transport/transport_ReliableSlidingWindow.h"
#include "nn/pia/transport/transport_Station.h"
#include "nn/pia/transport/transport_StationManager.h"

namespace nn {
namespace pia {
namespace transport {
namespace {
// the watermarks of the windows
const int WATERMARK_SEND_BUFFER = 5;
const int WATERMARK_RECEIVE_BUFFER = 6;
} // namespace

// 0x0045CF6C | fefates:callseq [tier C]
nn::Result nn::pia::transport::StationProtocolReliable::receiveProc()
{
    if (!common::IsValidPointer(m_pPacketHandler)) {
        return common::RESULT_INVALID_STATE;
    }
    ProtocolId protocolId = m_ProtocolId;
    PacketHandler::Iterator* pIterator = m_pPacketHandler->GetIterator(protocolId);
    if (pIterator->m_pPacketHandler->IsEndIteration()) {
        return nn::Result();
    }
    do {
        const ProtocolMessageReader* pReader = pIterator->GetMessageReader();
        Station* pStation = StationManager::s_pInstance->GetStation(pReader->GetSourceStationIndex());
        ReliableSlidingWindow* pWindow;
        nn::Result result;
        if (pStation == nullptr || !common::IsValidPointer(pWindow = pStation->m_pReliableSlidingWindow)) {
            result = common::RESULT_NOT_FOUND;
        } else if (!pWindow->IsInCommunication()) {
            result = common::RESULT_INVALID_STATE;
        } else {
            result = pWindow->AnalyzeProtocolMessage(*pReader);
            if (result.IsSuccess() && common::WatermarkManager::s_pInstance != nullptr) {
                common::WatermarkManager::s_pInstance->GetWatermark(WATERMARK_SEND_BUFFER)->Update(pWindow->m_SendCount);
                if (common::WatermarkManager::s_pInstance != nullptr) {
                    common::WatermarkManager::s_pInstance->GetWatermark(WATERMARK_RECEIVE_BUFFER)->Update(pWindow->m_ReceiveCount);
                }
            }
        }
        if (result.IsFailure() && result == common::RESULT_BUFFER_IS_FULL) {
            break;
        }
    } while (!pIterator->m_pPacketHandler->IsEndIteration());
    return nn::Result();
}

// 0x0045D0E8 | fefates:bytes
nn::Result nn::pia::transport::StationProtocolReliable::UpdateProtocolEvent(const nn::pia::transport::ProtocolEvent& event)
{
    StationManager* pManager = StationManager::s_pInstance;
    switch (event.m_Type) {
    case ProtocolEvent::TYPE_JOIN: {
        StationIndex stationIndex = event.m_StationIndex;
        StationIndex localStationIndex = m_LocalStationIndex;
        Station* pStation = pManager->GetStation(stationIndex);
        if (!common::IsValidPointer(pStation)) {
            return common::RESULT_NOT_FOUND;
        }
        ReliableSlidingWindow* pWindow = pStation->m_pReliableSlidingWindow;
        if (!common::IsValidPointer(pWindow)) {
            return common::RESULT_INVALID_STATE;
        }
        return pWindow->Startup(m_pPacketHandler, m_ProtocolId.m_Id, localStationIndex, stationIndex);
    }
    case ProtocolEvent::TYPE_LEAVE: {
        Station* pStation = pManager->GetStation(event.m_StationIndex);
        if (common::IsValidPointer(pStation) && common::IsValidPointer(pStation->m_pReliableSlidingWindow)) {
            pStation->m_pReliableSlidingWindow->Cleanup();
        }
        return nn::Result();
    }
    default:
        return common::RESULT_INTERNAL_ERROR;
    }
}

// 0x0045D1A0
void nn::pia::transport::StationProtocolReliable::Cleanup()
{
    m_LocalStationIndex = STATION_INDEX_UNIDENTIFIED;
}

// 0x0045D1AC | fefates:bytes [tier B]
nn::Result nn::pia::transport::StationProtocolReliable::Receive(nn::pia::StationIndex stationIndex, unsigned int bufferSize, unsigned char* pBuffer, unsigned int* pSize, nn::pia::common::StationAddress* pAddress)
{
    if (!common::IsValidPointer(pBuffer) || !common::IsValidPointer(pSize) || !common::IsValidPointer(pAddress)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    Station* pStation = StationManager::s_pInstance->GetStation(stationIndex);
    ReliableSlidingWindow* pWindow;
    if (pStation == nullptr || (pWindow = pStation->m_pReliableSlidingWindow) == nullptr) {
        return common::RESULT_NOT_FOUND;
    }
    if (!pWindow->IsInCommunication()) {
        return common::RESULT_NO_DATA;
    }
    StationManager* pManager = StationManager::s_pInstance;
    if (pManager->GetStation(stationIndex) == pManager->m_pLocalStation) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    nn::Result result = pWindow->PopData(pBuffer, pSize, bufferSize);
    if (result.IsFailure()) {
        return result;
    }
    if (common::WatermarkManager::s_pInstance != nullptr) {
        common::WatermarkManager::s_pInstance->GetWatermark(WATERMARK_RECEIVE_BUFFER)->Update(pWindow->m_ReceiveCount);
    }
    // (the station of the local index is looked up, its result is not used)
    pManager->GetStation(pWindow->m_LocalStationIndex);
    *pAddress = StationManager::s_pInstance->GetStation(pWindow->m_PeerStationIndex)->m_StationAddress;
    return nn::Result();
}

// 0x0045D2DC | fefates:bytes
nn::Result nn::pia::transport::StationProtocolReliable::Startup(nn::pia::StationIndex localStationIndex)
{
    if (localStationIndex > STATION_INDEX_MAX) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    m_LocalStationIndex = localStationIndex;
    return nn::Result();
}

// 0x0045D2F4 | fefates:bytes
nn::Result nn::pia::transport::StationProtocolReliable::Dispatch()
{
    receiveProc();
    StationManager* pManager = StationManager::s_pInstance;
    if (!common::IsValidPointer(pManager)) {
        return common::RESULT_INVALID_STATE;
    }
    for (Station** it = pManager->m_ActiveStations.Begin(); it != pManager->m_ActiveStations.End(); it++) {
        if ((*it)->m_pReliableSlidingWindow->IsInCommunication()) {
            nn::Result result = (*it)->m_pReliableSlidingWindow->Dispatch(m_pPacketHandler);
            if (result.IsFailure()) {
                return result;
            }
        }
    }
    return nn::Result();
}

// 0x0045D388 | fefates:bytes [tier B]
nn::pia::transport::StationProtocolReliable::StationProtocolReliable() : m_LocalStationIndex(STATION_INDEX_UNIDENTIFIED)
{
    if (common::WatermarkManager::s_pInstance != nullptr) {
        common::WatermarkManager::s_pInstance->GetWatermark(WATERMARK_SEND_BUFFER)->SetName("StationProtocolReliable send buffer num");
        if (common::WatermarkManager::s_pInstance != nullptr) {
            common::WatermarkManager::s_pInstance->GetWatermark(WATERMARK_RECEIVE_BUFFER)->SetName("StationProtocolReliable receive buffer num");
        }
    }
}

// 0x0045D454
// 0x0045D444 (deleting dtor)
nn::pia::transport::StationProtocolReliable::~StationProtocolReliable()
{
    // empty (in the original too)
}

// 0x00736770
u16 nn::pia::transport::StationProtocolReliable::GetProtocolType() const
{
    return PROTOCOL_TYPE_STATION;
}

// 0x00736778
void nn::pia::transport::StationProtocolReliable::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace transport
} // namespace pia
} // namespace nn
