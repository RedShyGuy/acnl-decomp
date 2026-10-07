#include "nn/pia/local/local_LocalProcessHostMigrationJobNew.h"
#include "nn/pia/common/common_Scheduler.h"
#include "nn/pia/common/common_StationAddress.h"
#include "nn/pia/common/common_TimeSpan.h"
#include "nn/pia/local/local_LocalMigrationManager.h"
#include "nn/pia/local/local_LocalNetwork.h"
#include "nn/pia/local/local_LocalNetworkManager.h"
#include "nn/pia/session/session_Mesh.h"
#include "nn/pia/transport/transport_Station.h"
#include "nn/pia/transport/transport_StationManager.h"

namespace nn {
namespace pia {
namespace local {
namespace {
const u8 TRANSPORT_ID_INVALID = 255;

inline LocalNetworkManager* GetNetworkManager()
{
    return LocalNetwork::s_pInstance->m_pNetworkManager;
}

inline bool IsDispatchTimeBefore(const common::Time& time)
{
    return common::Scheduler::s_pInstance->m_DispatchTime < time;
}

inline bool IsMigrationFailed()
{
    return LocalNetwork::s_pInstance->m_pMigrationManager->m_MigrationResult == LocalMigrationManager::MIGRATION_RESULT_FAILED;
}
} // namespace

// 0x004249DC
bool nn::pia::local::LocalProcessHostMigrationJobNew::StartupImpl(bool isFromMessage, nn::pia::StationIndex stationIndex)
{
    if (isFromMessage) {
        m_OldHostStationIndex = session::Mesh::s_pInstance->m_HostStationIndex;
        m_NewHostStationIndex = stationIndex;
        SetStep(&LocalProcessHostMigrationJobNew::LocalCleanupOldHostInfo, "LocalProcessHostMigrationJobNew::LocalCleanupOldHostInfo");
    } else {
        SetStep(&LocalProcessHostMigrationJobNew::LocalDecideNextHost, "LocalProcessHostMigrationJobNew::LocalDecideNextHost");
    }
    return true;
}

// 0x00424AB4
void nn::pia::local::LocalProcessHostMigrationJobNew::CleanupStatus()
{
    ProcessHostMigrationJob::CleanupStatus();
    m_OldHostTransportId = TRANSPORT_ID_INVALID;
    m_NewHostTransportId = TRANSPORT_ID_INVALID;
}

// 0x00424AD0
nn::pia::common::ExecuteResult nn::pia::local::LocalProcessHostMigrationJobNew::LocalDecideNextHost()
{
    StationIndex hostStationIndex = session::Mesh::s_pInstance->m_HostStationIndex;
    if (hostStationIndex > STATION_INDEX_MAX || session::Mesh::s_pInstance->m_LocalStationIndex > STATION_INDEX_MAX ||
        session::Mesh::s_pInstance->m_LocalStationIndex == hostStationIndex) {
        SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "LocalProcessHostMigrationJobNew::HostMigrationFailure");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }

    m_OldHostTransportId = GetNetworkManager()->m_HostTransportId;
    {
        common::StationAddress address;
        address.SetExtensionId(m_OldHostTransportId);
        transport::Station* pStation = transport::StationManager::s_pInstance->GetStation(address);
        u8 next;
        if (pStation == nullptr || pStation->m_StationIndex == hostStationIndex) {
            next = LocalNetwork::s_pInstance->m_pMigrationManager->GetNextHostCandidateTransportId();
        } else {
            // the station of the old host id is not the host of the mesh: it is the new host
            next = m_OldHostTransportId;
            m_OldHostTransportId = TRANSPORT_ID_INVALID;
        }
        m_NewHostTransportId = next;
    }
    if (m_NewHostTransportId == TRANSPORT_ID_INVALID) {
        SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "LocalProcessHostMigrationJobNew::HostMigrationFailure");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }

    common::StationAddress address;
    address.SetExtensionId(m_NewHostTransportId);
    transport::Station* pStation = transport::StationManager::s_pInstance->GetStation(address);
    if (pStation == nullptr) {
        SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "LocalProcessHostMigrationJobNew::HostMigrationFailure");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    m_NewHostStationIndex = pStation->m_StationIndex;

    u32 next = DecideNextHostCommonProc();
    m_Deadline += common::TimeSpan(common::TimeSpan::GetTicksPerMSec().GetTick() * NETWORK_MIGRATION_TIMEOUT_MSEC);
    if (next == 1) {
        SetStep(&LocalProcessHostMigrationJobNew::LocalPrepareForBecomingHost, "LocalProcessHostMigrationJobNew::LocalPrepareForBecomingHost");
    } else if (next == 2) {
        SetStep(&LocalProcessHostMigrationJobNew::WaitLocalHostMigrationClient, "LocalProcessHostMigrationJobNew::WaitLocalHostMigrationClient");
    } else {
        SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "LocalProcessHostMigrationJobNew::HostMigrationFailure");
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x00424E08
nn::pia::common::ExecuteResult nn::pia::local::LocalProcessHostMigrationJobNew::LocalCleanupOldHostInfo()
{
    transport::Station* pOldHost = transport::StationManager::s_pInstance->GetStation(m_OldHostStationIndex);
    transport::Station* pNewHost = transport::StationManager::s_pInstance->GetStation(m_NewHostStationIndex);
    if (pNewHost == nullptr) {
        SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "LocalProcessHostMigrationJobNew::HostMigrationFailure");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }

    u8 hostTransportId = GetNetworkManager()->m_HostTransportId;
    if (pOldHost != nullptr) {
        m_OldHostTransportId = pOldHost->m_StationAddress.GetExtensionId();
        if (m_OldHostTransportId == hostTransportId || hostTransportId == TRANSPORT_ID_INVALID) {
            m_NewHostTransportId = pNewHost->m_StationAddress.GetExtensionId();
        } else if (pNewHost->m_StationAddress.GetExtensionId() == hostTransportId) {
            // the network has the new host already
            m_NewHostTransportId = hostTransportId;
        } else {
            SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "LocalProcessHostMigrationJobNew::HostMigrationFailure");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
    } else {
        m_OldHostTransportId = TRANSPORT_ID_INVALID;
        m_NewHostTransportId = pNewHost->m_StationAddress.GetExtensionId();
    }
    if (m_NewHostTransportId == TRANSPORT_ID_INVALID) {
        SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "LocalProcessHostMigrationJobNew::HostMigrationFailure");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    if (!CleanupOldHostInfoCommonProc()) {
        SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "LocalProcessHostMigrationJobNew::HostMigrationFailure");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }

    u32 next = DecideNextHostCommonProc();
    m_Deadline += common::TimeSpan(common::TimeSpan::GetTicksPerMSec().GetTick() * NETWORK_MIGRATION_TIMEOUT_MSEC);
    if (next == 1) {
        SetStep(&LocalProcessHostMigrationJobNew::LocalPrepareForBecomingHost, "LocalProcessHostMigrationJobNew::LocalPrepareForBecomingHost");
    } else if (next == 2) {
        SetStep(&LocalProcessHostMigrationJobNew::WaitLocalHostMigrationClient, "LocalProcessHostMigrationJobNew::WaitLocalHostMigrationClient");
    } else {
        SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "LocalProcessHostMigrationJobNew::HostMigrationFailure");
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x004250A0
bool nn::pia::local::LocalProcessHostMigrationJobNew::IsValidHostMigrationSetting()
{
    return LocalNetwork::s_pInstance->IsEnableHostMigration();
}

// 0x004250C0
nn::pia::common::ExecuteResult nn::pia::local::LocalProcessHostMigrationJobNew::LocalPrepareForBecomingHost()
{
    u8 localTransportId = GetNetworkManager()->m_LocalTransportId;
    if (localTransportId != TRANSPORT_ID_INVALID && localTransportId != m_NewHostTransportId) {
        SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "LocalProcessHostMigrationJobNew::HostMigrationFailure");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    if (!PrepareForBecomingHostCommonProc()) {
        SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "LocalProcessHostMigrationJobNew::HostMigrationFailure");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    UpdateHostStationIndexByLocalStationIndex();
    SetStep(&LocalProcessHostMigrationJobNew::WaitLocalHostMigrationNewHost, "LocalProcessHostMigrationJobNew::WaitLocalHostMigrationNewHost");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x00425218
nn::pia::common::ExecuteResult nn::pia::local::LocalProcessHostMigrationJobNew::WaitLocalHostMigrationClient()
{
    u8 hostTransportId = GetNetworkManager()->m_HostTransportId;
    if (m_NewHostTransportId == hostTransportId && !LocalNetwork::s_pInstance->IsDuringHostMigration()) {
        SetStep(&ProcessHostMigrationJob::WaitNewHostGreeting, "LocalProcessHostMigrationJobNew::WaitNewHostGreeting");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    // the network still has the old host (or none) or the new one
    if ((m_OldHostTransportId == TRANSPORT_ID_INVALID || hostTransportId == TRANSPORT_ID_INVALID ||
         m_OldHostTransportId == hostTransportId || m_NewHostTransportId == hostTransportId) &&
        IsDispatchTimeBefore(m_Deadline) && !IsMigrationFailed()) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "LocalProcessHostMigrationJobNew::HostMigrationFailure");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0042538C
nn::pia::common::ExecuteResult nn::pia::local::LocalProcessHostMigrationJobNew::WaitLocalHostMigrationNewHost()
{
    if (!LocalNetwork::s_pInstance->IsHost() || LocalNetwork::s_pInstance->IsDuringHostMigration()) {
        if (!IsDispatchTimeBefore(m_Deadline) || IsMigrationFailed()) {
            SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "LocalProcessHostMigrationJobNew::HostMigrationFailure");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    if (GetNetworkManager()->m_LocalTransportId != m_NewHostTransportId) {
        SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "LocalProcessHostMigrationJobNew::HostMigrationFailure");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    SetStep(&ProcessHostMigrationJob::SendGreetingMessage, "LocalProcessHostMigrationJobNew::SendGreetingMessage");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x00425518
nn::pia::local::LocalProcessHostMigrationJobNew::LocalProcessHostMigrationJobNew()
    : m_OldHostTransportId(TRANSPORT_ID_INVALID), m_NewHostTransportId(TRANSPORT_ID_INVALID)
{
}

// 0x00442AEC
// 0x0042553C (deleting dtor)
nn::pia::local::LocalProcessHostMigrationJobNew::~LocalProcessHostMigrationJobNew()
{
    // empty (in the original too)
}

// 0x007317F4
void nn::pia::local::LocalProcessHostMigrationJobNew::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn
