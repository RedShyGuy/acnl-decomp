#include "nn/pia/transport/transport_StationProtocolManager.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/transport/transport_Api.h"
#include "nn/pia/transport/transport_ProtocolManager.h"
#include "nn/pia/transport/transport_RttProtocol.h"
#include "nn/pia/transport/transport_StationProtocol.h"
#include "nn/pia/transport/transport_StationProtocolReliable.h"
#include "nn/pia/transport/transport_Transport.h"

namespace nn {
namespace pia {
namespace transport {
// 0x0097E484
nn::pia::transport::StationProtocolManager* nn::pia::transport::StationProtocolManager::s_pInstance;

// 0x0045BDEC | fefates:callseq [tier C]
nn::Result nn::pia::transport::StationProtocolManager::Initialize(unsigned int stationNum)
{
    Transport* pTransport = Transport::s_pInstance;
    if (!common::IsValidPointer(pTransport)) {
        return common::RESULT_INVALID_STATE;
    }
    m_StationProtocolId = pTransport->m_ProtocolManager.CreateProtocol<StationProtocol>(PROTOCOL_TYPE_STATION, 0);
    m_StationProtocolReliableId = pTransport->m_ProtocolManager.CreateProtocol<StationProtocolReliable>(PROTOCOL_TYPE_STATION, 1);
    m_RttProtocolId = pTransport->m_ProtocolManager.CreateProtocol<RttProtocol>(PROTOCOL_TYPE_RTT, 0);
    if (m_RttProtocolId.m_Id != 0) {
        pTransport->m_ProtocolManager.GetProtocol<RttProtocol>(m_RttProtocolId, PROTOCOL_TYPE_RTT)->Initialize(stationNum);
    }
    return nn::Result();
}

// 0x0045BF54 | fefates:callgraph [tier C]
nn::Result nn::pia::transport::StationProtocolManager::CreateInstance()
{
    if (!IsInitialized()) {
        return common::RESULT_NOT_INITIALIZED;
    }
    if (!IsInSetupMode()) {
        return common::RESULT_INVALID_STATE;
    }
    if (s_pInstance != nullptr) {
        return common::RESULT_ALREADY_EXISTS;
    }
    s_pInstance = new StationProtocolManager();
    return nn::Result();
}

// 0x0045BFC8 | fefates:bytes [tier B]
nn::pia::transport::RttProtocol* nn::pia::transport::StationProtocolManager::GetRttProtocol()
{
    return Transport::s_pInstance->m_ProtocolManager.GetProtocol<RttProtocol>(m_RttProtocolId, PROTOCOL_TYPE_RTT);
}

// 0x0045BFE4 | fefates:callgraph [tier C]
void nn::pia::transport::StationProtocolManager::DestroyInstance()
{
    if (s_pInstance != nullptr) {
        delete s_pInstance;
        s_pInstance = nullptr;
    }
}

// 0x0045C00C | fefates:bytes [tier B]
nn::pia::transport::StationProtocol* nn::pia::transport::StationProtocolManager::GetStationProtocol()
{
    return Transport::s_pInstance->m_ProtocolManager.GetProtocol<StationProtocol>(m_StationProtocolId, PROTOCOL_TYPE_STATION);
}

// 0x0045C028 | fefates:bytes [tier B]
nn::pia::transport::StationProtocolReliable* nn::pia::transport::StationProtocolManager::GetStationProtocolReliable()
{
    return Transport::s_pInstance->m_ProtocolManager.GetProtocol<StationProtocolReliable>(m_StationProtocolReliableId, PROTOCOL_TYPE_STATION);
}

// 0x0045C044 | fefates:bytes [tier B]
void nn::pia::transport::StationProtocolManager::Finalize()
{
    Transport* pTransport = Transport::s_pInstance;
    if (!common::IsValidPointer(pTransport)) {
        return;
    }
    if (m_RttProtocolId.m_Id != 0) {
        pTransport->m_ProtocolManager.GetProtocol<RttProtocol>(m_RttProtocolId, PROTOCOL_TYPE_RTT)->Finalize();
        pTransport->m_ProtocolManager.DestroyProtocol(m_RttProtocolId.m_Id);
        m_RttProtocolId.m_Id = 0;
    }
    if (m_StationProtocolReliableId.m_Id != 0) {
        pTransport->m_ProtocolManager.DestroyProtocol(m_StationProtocolReliableId.m_Id);
        m_StationProtocolReliableId.m_Id = 0;
    }
    if (m_StationProtocolId.m_Id != 0) {
        pTransport->m_ProtocolManager.DestroyProtocol(m_StationProtocolId.m_Id);
        m_StationProtocolId.m_Id = 0;
    }
}

} // namespace transport
} // namespace pia
} // namespace nn
