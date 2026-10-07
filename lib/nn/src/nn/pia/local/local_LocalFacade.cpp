#include "nn/pia/local/local_LocalFacade.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_Scheduler.h"
#include "nn/pia/common/common_StationAddress.h"
#include "nn/pia/local/local_Api.h"
#include "nn/pia/local/local_LocalNetwork.h"
#include "nn/pia/local/local_LocalNetworkManager.h"
#include "nn/pia/session/session_Mesh.h"
#include "nn/pia/transport/transport_StationConnectionInfo.h"
#include "nn/pia/transport/transport_StationConnectionInfoTable.h"
#include "nn/pia/transport/transport_StationLocation.h"
#include "nn/pia/transport/transport_Transport.h"

namespace nn {
namespace pia {
namespace local {
namespace {
const u8 TRANSPORT_ID_INVALID = 255;
} // namespace

// 0x00975A70
nn::pia::local::LocalFacade* nn::pia::local::LocalFacade::s_pInstance;

// 0x00414678 | fefates:bytes [tier B]
nn::Result nn::pia::local::LocalFacade::CreateInstance()
{
    if (!IsInitialized()) {
        return common::RESULT_NOT_INITIALIZED;
    }
    if (!IsDuringSetup()) {
        return common::RESULT_INVALID_STATE;
    }
    if (s_pInstance != nullptr) {
        return common::RESULT_ALREADY_EXISTS;
    }
    if (!common::IsValidPointer(LocalNetwork::s_pInstance)) {
        return common::RESULT_INVALID_STATE;
    }
    s_pInstance = new LocalFacade();
    return nn::Result();
}

// 0x00414710 | fefates:bytes [tier B]
void nn::pia::local::LocalFacade::DestroyInstance()
{
    if (s_pInstance != nullptr) {
        delete s_pInstance;
        s_pInstance = nullptr;
    }
}

// 0x00414740 | fefates:bytes [tier B]
void nn::pia::local::LocalFacade::LocalFacadeUpdateEventCallback(nn::pia::local::LocalUpdateEvent event, u8 nodeId, void*)
{
    if (event == LOCAL_UPDATE_EVENT_DISCONNECTED) {
        if (session::Mesh::s_pInstance != nullptr) {
            common::StationAddress address;
            address.SetExtensionId(nodeId);
            session::Mesh::s_pInstance->NotifyLeaveStationAddress(address);
        }
    } else if (event == LOCAL_UPDATE_EVENT_MIGRATION_STARTED) {
        if (session::Mesh::s_pInstance != nullptr) {
            session::Mesh::s_pInstance->SetHostMigrationStartFlag(true);
        }
    }
}

// 0x004147C8 | fefates:bytes
void nn::pia::local::LocalFacade::Cleanup()
{
    if (m_IsStarted) {
        LocalNetwork::s_pInstance->UnregisterUpdateEventCallback();
        m_IsStarted = false;
    }
}

// 0x004147F8 | fefates:bytes-fuzzy
nn::Result nn::pia::local::LocalFacade::Startup()
{
    if (m_IsStarted) {
        return common::RESULT_INVALID_STATE;
    }
    LocalNetwork* pNetwork = LocalNetwork::s_pInstance;
    if (pNetwork == nullptr) {
        return common::RESULT_INVALID_STATE;
    }
    u8 localTransportId = pNetwork->m_pNetworkManager->m_LocalTransportId;
    if (localTransportId == TRANSPORT_ID_INVALID) {
        return common::RESULT_INVALID_STATE;
    }

    transport::StationConnectionInfo info;
    ConvertTransportIdToStationConnectionInfo(localTransportId, &info);
    if (transport::Transport::s_pInstance == nullptr) {
        return common::RESULT_INVALID_STATE;
    }
    transport::StationConnectionInfoTable::s_pInstance->m_LocalInfo = info;
    if (GetHostStationConnectionInfo(&info).IsFailure()) {
        return common::RESULT_INVALID_STATE;
    }
    transport::StationConnectionInfoTable::s_pInstance->m_HostInfo = info;
    transport::StationConnectionInfoTable::s_pInstance->m_HostAddress = info.m_PublicLocation.m_StationAddress;
    if (session::Mesh::s_pInstance != nullptr) {
        session::Mesh::s_pInstance->m_HostMigrationStartFlag = false;
    }
    LocalNetwork::s_pInstance->RegisterUpdateEventCallback(LocalFacadeUpdateEventCallback, nullptr);
    m_IsStarted = true;
    return nn::Result();
}

// 0x0041493C
// 0x00414938 (deleting dtor)
nn::pia::local::LocalFacade::~LocalFacade()
{
    // empty (in the original too)
}

// 0x0072FAD4 | fefates:bytes [tier B]
nn::Result nn::pia::local::LocalFacade::GetHostStationConnectionInfo(nn::pia::transport::StationConnectionInfo* pInfo) const
{
    u8 hostTransportId = LocalNetwork::s_pInstance->m_pNetworkManager->m_HostTransportId;
    if (hostTransportId == TRANSPORT_ID_INVALID) {
        return common::RESULT_INVALID_STATE;
    }
    return ConvertTransportIdToStationConnectionInfo(hostTransportId, pInfo);
}

// 0x0072FB04 | fefates:bytes [tier B]
nn::Result nn::pia::local::LocalFacade::ConvertTransportIdToStationConnectionInfo(u8 transportId, nn::pia::transport::StationConnectionInfo* pInfo) const
{
    if (transportId == TRANSPORT_ID_INVALID || !common::IsValidPointer(pInfo)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    common::StationAddress address;
    transport::StationLocation location;
    address.SetExtensionId(transportId);
    location.SetStationAddress(address);
    // the local network has no connection id: the time makes one
    location.m_ConnectionId = static_cast<u32>(common::Scheduler::s_pInstance->m_DispatchTime.m_Tick);
    pInfo->m_PublicLocation.SetStationLocation(location);
    return nn::Result();
}

// 0x0072FBAC
void nn::pia::local::LocalFacade::vf_0x10()
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn
