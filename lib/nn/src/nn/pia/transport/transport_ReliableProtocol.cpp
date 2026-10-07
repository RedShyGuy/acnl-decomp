#include "nn/pia/transport/transport_ReliableProtocol.h"
#include "nn/pia/common/common_NewArray.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_SessionBeginMonitoringContent.h"
#include "nn/pia/common/common_SessionStateMonitoringContent.h"
#include "nn/pia/common/common_Watermark.h"
#include "nn/pia/common/common_WatermarkManager.h"
#include "nn/pia/transport/transport_Api.h"
#include "nn/pia/transport/transport_PacketHandler.h"
#include "nn/pia/transport/transport_ProtocolEvent.h"
#include "nn/pia/transport/transport_ProtocolId.h"
#include "nn/pia/transport/transport_ProtocolMessageReader.h"
#include "nn/pia/transport/transport_ReliableSlidingWindow.h"
#include "nn/pia/transport/transport_Transport.h"

namespace nn {
namespace pia {
namespace transport {
namespace {
// the watermarks of the windows
const int WATERMARK_SEND_BUFFER = 3;
const int WATERMARK_RECEIVE_BUFFER = 4;
// the station index that sends to all stations
const StationIndex STATION_INDEX_ALL = static_cast<StationIndex>(255);
} // namespace

inline nn::pia::transport::ReliableSlidingWindow* nn::pia::transport::ReliableProtocol::GetWindow(StationIndex stationIndex) const
{
    return &m_pWindows[m_LocalStationIndex < stationIndex ? stationIndex - 1 : stationIndex];
}

// 0x00452664 | fefates:bytes [tier B]
nn::Result nn::pia::transport::ReliableProtocol::Initialize(unsigned int sendNum, unsigned int receiveNum)
{
    if (m_pWindows != nullptr) {
        return common::RESULT_ALREADY_INITIALIZED;
    }
    m_LocalStationIndex = STATION_INDEX_UNIDENTIFIED;
    m_StationNum = Transport::s_pInstance->m_StationNum;
    u32 windowNum = m_StationNum - 1;
    m_pWindows = common::NewArray<ReliableSlidingWindow>(windowNum);
    for (u32 i = 0; i < windowNum; i++) {
        nn::Result result = m_pWindows[i].Initialize(sendNum, receiveNum);
        if (result.IsFailure()) {
            Finalize();
            return result;
        }
    }
    common::g_SessionBeginMonitoringContent.m_ReliableSendNum = sendNum;
    common::g_SessionBeginMonitoringContent.m_ReliableReceiveNum = receiveNum;
    return nn::Result();
}

// 0x004527C0 | fefates:bytes [tier B]
nn::Result nn::pia::transport::ReliableProtocol::ReceiveImpl(nn::pia::StationIndex* pStationIndex, void* pBuffer, unsigned int* pSize, unsigned int bufferSize, bool withStationId)
{
    if (!common::IsValidPointer(pStationIndex)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_LocalStationIndex == STATION_INDEX_UNIDENTIFIED) {
        return common::RESULT_NOT_IN_COMMUNICATION;
    }
    for (u32 i = 0; i < m_StationNum - 1; i++) {
        if (!m_pWindows[i].IsInCommunication()) {
            continue;
        }
        if (withStationId) {
            StationId stationId;
            if (Transport::s_pInstance->ConvertToStationId(&stationId, m_pWindows[i].m_PeerStationIndex).IsFailure()) {
                continue;
            }
        }
        nn::Result result = m_pWindows[i].PopData(pBuffer, pSize, bufferSize);
        if (result.IsSuccess()) {
            if (common::WatermarkManager::s_pInstance != nullptr) {
                common::WatermarkManager::s_pInstance->GetWatermark(WATERMARK_RECEIVE_BUFFER)->Update(m_pWindows[i].m_ReceiveCount);
            }
            *pStationIndex = m_pWindows[i].m_PeerStationIndex;
            return result;
        }
        if (result != common::RESULT_NO_DATA) {
            return result;
        }
    }
    return common::RESULT_NO_DATA;
}

// 0x00452924 | fefates:bytes
nn::Result nn::pia::transport::ReliableProtocol::UpdateProtocolEvent(const nn::pia::transport::ProtocolEvent& event)
{
    StationIndex localStationIndex = m_LocalStationIndex;
    if (localStationIndex == STATION_INDEX_UNIDENTIFIED) {
        return common::RESULT_INVALID_STATE;
    }
    StationIndex stationIndex = event.m_StationIndex;
    if (static_cast<u8>(m_StationNum) <= stationIndex || stationIndex == localStationIndex) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    ReliableSlidingWindow* pWindow = GetWindow(stationIndex);
    switch (event.m_Type) {
    case ProtocolEvent::TYPE_JOIN:
        return pWindow->Startup(m_pPacketHandler, m_ProtocolId.m_Id, localStationIndex, stationIndex);
    case ProtocolEvent::TYPE_LEAVE:
        pWindow->Cleanup();
        return nn::Result();
    default:
        return common::RESULT_INVALID_ARGUMENT;
    }
}

// 0x004529C0 | fefates:callseq [tier C]
nn::Result nn::pia::transport::ReliableProtocol::Send(nn::pia::StationIndex stationIndex, const void* pData, unsigned int size)
{
    if (m_pWindows == nullptr) {
        return common::RESULT_INVALID_STATE;
    }
    if (m_LocalStationIndex == STATION_INDEX_UNIDENTIFIED) {
        return common::RESULT_NOT_IN_COMMUNICATION;
    }
    if (m_LocalStationIndex == stationIndex) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (stationIndex == STATION_INDEX_ALL) {
        u32 windowNum = m_StationNum - 1;
        for (u32 i = 0; i < windowNum; i++) {
            if (m_pWindows[i].IsInCommunication() && !m_pWindows[i].CanPushData(size)) {
                common::SessionStateMonitoringContent& content = common::g_SessionStateMonitoringContent;
                if (content.m_ReliableBufferFullNum != 0xFFFFFFFF) {
                    content.m_ReliableBufferFullNum++;
                }
                return common::RESULT_BUFFER_IS_FULL;
            }
        }
        for (u32 i = 0; i < windowNum; i++) {
            ReliableSlidingWindow* pWindow = &m_pWindows[i];
            if (pWindow->IsInCommunication()) {
                nn::Result result = pWindow->PushData(pData, size);
                if (result.IsFailure()) {
                    return result;
                }
                if (common::WatermarkManager::s_pInstance != nullptr) {
                    common::WatermarkManager::s_pInstance->GetWatermark(WATERMARK_SEND_BUFFER)->Update(pWindow->m_SendCount);
                }
            }
        }
        return nn::Result();
    }
    if (static_cast<u8>(m_StationNum) <= stationIndex) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    ReliableSlidingWindow* pWindow = GetWindow(stationIndex);
    if (!pWindow->IsInCommunication()) {
        return common::RESULT_NOT_IN_COMMUNICATION;
    }
    nn::Result result = pWindow->PushData(pData, size);
    if (result.IsFailure()) {
        if (result == common::RESULT_BUFFER_IS_FULL) {
            common::SessionStateMonitoringContent& content = common::g_SessionStateMonitoringContent;
            if (content.m_ReliableBufferFullNum != 0xFFFFFFFF) {
                content.m_ReliableBufferFullNum++;
            }
        }
        return result;
    }
    if (common::WatermarkManager::s_pInstance != nullptr) {
        common::WatermarkManager::s_pInstance->GetWatermark(WATERMARK_SEND_BUFFER)->Update(pWindow->m_SendCount);
    }
    return result;
}

// 0x00452BCC | fefates:bytes [tier B]
nn::Result nn::pia::transport::ReliableProtocol::Send(nn::pia::StationId stationId, const void* pData, unsigned int size)
{
    StationIndex stationIndex;
    nn::Result result = Transport::s_pInstance->ConvertToStationIndex(&stationIndex, stationId);
    if (result.IsFailure()) {
        return result;
    }
    return Send(stationIndex, pData, size);
}

// 0x00452C18 | fefates:bytes
void nn::pia::transport::ReliableProtocol::Cleanup()
{
    if (m_LocalStationIndex == STATION_INDEX_UNIDENTIFIED) {
        return;
    }
    u32 windowNum = m_StationNum - 1;
    m_LocalStationIndex = STATION_INDEX_UNIDENTIFIED;
    for (u32 i = 0; i < windowNum; i++) {
        if (m_pWindows[i].IsInCommunication()) {
            m_pWindows[i].Cleanup();
        }
    }
}

// 0x00452C84 | fefates:bytes [tier B]
nn::Result nn::pia::transport::ReliableProtocol::Receive(nn::pia::StationId* pStationId, void* pBuffer, unsigned int* pSize, unsigned int bufferSize)
{
    if (!common::IsValidPointer(pStationId)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    StationIndex stationIndex;
    nn::Result result = ReceiveImpl(&stationIndex, pBuffer, pSize, bufferSize, true);
    if (result.IsSuccess()) {
        *pStationId = Conv2StationId(stationIndex);
    }
    return result;
}

// 0x00452CEC | fefates:bytes-fuzzy
nn::Result nn::pia::transport::ReliableProtocol::Startup(nn::pia::StationIndex localStationIndex)
{
    if (m_pWindows == nullptr) {
        return common::RESULT_NOT_INITIALIZED;
    }
    if (m_LocalStationIndex != STATION_INDEX_UNIDENTIFIED) {
        return common::RESULT_INVALID_STATE;
    }
    if (static_cast<u8>(m_StationNum) <= localStationIndex) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    m_LocalStationIndex = localStationIndex;
    m_DispatchIndex = 0;
    common::g_SessionStateMonitoringContent.m_ReliableBufferFullNum = 0;
    return nn::Result();
}

// 0x00452D4C | fefates:callseq
nn::Result nn::pia::transport::ReliableProtocol::Dispatch()
{
    if (m_LocalStationIndex == STATION_INDEX_UNIDENTIFIED) {
        return nn::Result();
    }
    ProtocolId protocolId = m_ProtocolId;
    PacketHandler::Iterator* pIterator = m_pPacketHandler->GetIterator(protocolId);
    pIterator->m_pPacketHandler->BeginIteration();
    while (!pIterator->m_pPacketHandler->IsEndIteration()) {
        const ProtocolMessageReader* pReader = pIterator->GetMessageReader();
        StationIndex sourceStationIndex = pReader->GetSourceStationIndex();
        if (static_cast<u8>(m_StationNum) > sourceStationIndex && m_LocalStationIndex != sourceStationIndex) {
            ReliableSlidingWindow* pWindow = GetWindow(sourceStationIndex);
            if (pWindow->IsInCommunication() && pWindow->AnalyzeProtocolMessage(*pReader).IsSuccess() &&
                common::WatermarkManager::s_pInstance != nullptr) {
                common::WatermarkManager::s_pInstance->GetWatermark(WATERMARK_SEND_BUFFER)->Update(pWindow->m_SendCount);
                if (common::WatermarkManager::s_pInstance != nullptr) {
                    common::WatermarkManager::s_pInstance->GetWatermark(WATERMARK_RECEIVE_BUFFER)->Update(pWindow->m_ReceiveCount);
                }
            }
        }
        pIterator->m_pPacketHandler->NextIteration();
    }
    // the windows send in turns, each dispatch beginning with the next one
    m_DispatchIndex++;
    u32 windowNum = m_StationNum - 1;
    if (windowNum <= m_DispatchIndex) {
        m_DispatchIndex = 0;
    }
    for (u32 i = m_DispatchIndex; i < windowNum; i++) {
        if (m_pWindows[i].IsInCommunication()) {
            m_pWindows[i].Dispatch(m_pPacketHandler);
        }
    }
    for (u32 i = 0; i < m_DispatchIndex; i++) {
        if (m_pWindows[i].IsInCommunication()) {
            m_pWindows[i].Dispatch(m_pPacketHandler);
        }
    }
    return nn::Result();
}

// 0x00452F48 | fefates:bytes [tier B]
void nn::pia::transport::ReliableProtocol::Finalize()
{
    m_LocalStationIndex = STATION_INDEX_UNIDENTIFIED;
    m_StationNum = 0;
    if (m_pWindows != nullptr) {
        common::DeleteArray(m_pWindows);
        m_pWindows = nullptr;
    }
}

// 0x00452FC4 | fefates:bytes [tier B]
nn::pia::transport::ReliableProtocol::ReliableProtocol()
    : m_LocalStationIndex(STATION_INDEX_UNIDENTIFIED), m_StationNum(0), m_pWindows(nullptr), m_DispatchIndex(0)
{
    if (common::WatermarkManager::s_pInstance != nullptr) {
        common::WatermarkManager::s_pInstance->GetWatermark(WATERMARK_SEND_BUFFER)->SetName("ReliableProtocol send buffer num");
        if (common::WatermarkManager::s_pInstance != nullptr) {
            common::WatermarkManager::s_pInstance->GetWatermark(WATERMARK_RECEIVE_BUFFER)->SetName("ReliableProtocol receive buffer num");
        }
    }
}

// 0x0045311C
// 0x00453084 (deleting dtor)
nn::pia::transport::ReliableProtocol::~ReliableProtocol()
{
    Finalize();
}

// 0x00735668
u16 nn::pia::transport::ReliableProtocol::GetProtocolType() const
{
    return PROTOCOL_TYPE_RELIABLE;
}

// 0x00735670 (name is ours)
bool nn::pia::transport::ReliableProtocol::IsInCommunication(nn::pia::StationId stationId) const
{
    StationIndex stationIndex = Conv2StationIndex(stationId);
    if (m_LocalStationIndex == STATION_INDEX_UNIDENTIFIED || static_cast<u8>(m_StationNum) <= stationIndex ||
        m_LocalStationIndex == stationIndex) {
        return false;
    }
    return GetWindow(stationIndex)->IsInCommunication();
}

// 0x007356D0
void nn::pia::transport::ReliableProtocol::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace transport
} // namespace pia
} // namespace nn
