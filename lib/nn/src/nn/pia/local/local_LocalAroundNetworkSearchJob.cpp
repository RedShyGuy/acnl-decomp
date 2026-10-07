#include "nn/pia/local/local_LocalAroundNetworkSearchJob.h"
#include "nn/pia/common/common_NewArray.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_TimeSpan.h"
#include "nn/pia/local/local_LocalAroundNetworkSearchBackgroundJob.h"
#include "nn/pia/local/local_LocalAroundNetworkSearchManager.h"
#include "nn/pia/local/local_LocalNetwork.h"
#include "pead/peadRandom.h"

namespace nn {
namespace pia {
namespace local {
namespace {
inline LocalAroundNetworkSearchManager* GetManager()
{
    return LocalNetwork::s_pInstance->m_pAroundNetworkSearchManager;
}

// the milliseconds since the time
inline u32 GetElapsedMSec(const common::Time& time)
{
    common::Time now;
    now.SetNow();
    return static_cast<s32>((now - time).GetTick() / common::TimeSpan::GetTicksPerMSec().GetTick());
}

inline common::ExecuteResult Continue()
{
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

inline common::ExecuteResult NextDispatch()
{
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}
} // namespace

// the host stops the search when it is not commanded; the others wait for the next command
inline nn::pia::common::ExecuteResult nn::pia::local::LocalAroundNetworkSearchJob::StopSearch()
{
    if (LocalNetwork::s_pInstance->IsHost()) {
        GetManager()->PrepareNextCommandStatus();
        SetStep(&LocalAroundNetworkSearchJob::SendStopAroundNetworkSearchMessage, "LocalAroundNetworkSearchJob::SendStopAroundNetworkSearchMessage");
        return Continue();
    }
    SetStep(&LocalAroundNetworkSearchJob::WaitAroundNetworkSearchActivated, "LocalAroundNetworkSearchJob::WaitAroundNetworkSearchActivated");
    return NextDispatch();
}

// the station is in the host migration or out of the network: the messages are not sent any more
inline nn::pia::common::ExecuteResult nn::pia::local::LocalAroundNetworkSearchJob::EndSearch(bool isDuringHostMigration)
{
    GetManager()->EndSendMessage();
    if (isDuringHostMigration) {
        SetStep(&LocalAroundNetworkSearchJob::WaitHostMigrationCompleted, "LocalAroundNetworkSearchJob::WaitHostMigrationCompleted");
    } else {
        SetStep(&LocalAroundNetworkSearchJob::WaitAroundNetworkSearchActivated, "LocalAroundNetworkSearchJob::WaitAroundNetworkSearchActivated");
    }
    return NextDispatch();
}

// 0x0042151C | fefates:bytes [tier B]
void nn::pia::local::LocalAroundNetworkSearchJob::LifeTimeProcess()
{
    u32 elapsed = GetElapsedMSec(m_LifeTimeTime);
    if (elapsed < LIFE_TIME_INTERVAL_MSEC) {
        return;
    }
    m_LifeTimeTime.SetNow();
    common::CriticalSection& criticalSection = GetManager()->m_CriticalSection;
    criticalSection.Lock();
    for (u32 i = 0; i < LocalAroundNetworkSearchManager::AROUND_NETWORK_STATUS_NUM; i++) {
        LocalAroundNetworkSearchManager::AroundNetworkStatus* pStatus = GetManager()->GetAroundNetworkStatus(i);
        if (pStatus->m_LifeTime == 0) {
            continue;
        }
        if (pStatus->m_LifeTime <= elapsed) {
            pStatus->m_Version = 0;
            pStatus->m_DestinationBitmap = 0;
            pStatus->m_LifeTime = 0;
            pStatus->m_Key = 0;
        } else {
            pStatus->m_LifeTime -= elapsed;
        }
    }
    criticalSection.Unlock();
}

// 0x004215F8 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::local::LocalAroundNetworkSearchJob::SendAroundNetworkStatus()
{
    GetManager()->SendAroundNetworkStatusMessage();
    m_SendTime.SetNow();
    SetStep(&LocalAroundNetworkSearchJob::WaitSendAroundNetworkStatusCompleted, "LocalAroundNetworkSearchJob::WaitSendAroundNetworkStatusCompleted");
    return NextDispatch();
}

// 0x0042164C | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::local::LocalAroundNetworkSearchJob::WaitAroundNetworkSearch()
{
    LifeTimeProcess();
    switch (m_pBackgroundCallContext->GetState()) {
    case common::CallContext::STATE_CALL_IN_PROGRESS:
        return NextDispatch();
    case common::CallContext::STATE_CALL_SUCCESS:
        if (GetManager()->m_pBackgroundJob->IsRunning()) {
            return NextDispatch();
        }
        if (!GetManager()->m_IsSearching) {
            return StopSearch();
        }
        if (LocalNetwork::s_pInstance->IsDuringHostMigration()) {
            return EndSearch(true);
        }
        if (LocalNetwork::s_pInstance->IsHost() || LocalNetwork::s_pInstance->IsClient()) {
            SetStep(&LocalAroundNetworkSearchJob::SendAroundNetworkStatus, "LocalAroundNetworkSearchJob::SendAroundNetworkStatus");
            return Continue();
        }
        return EndSearch(false);
    case common::CallContext::STATE_CALL_FAILURE:
        SetStep(&LocalAroundNetworkSearchJob::WaitAroundNetworkSearchActivated, "LocalAroundNetworkSearchJob::WaitAroundNetworkSearchActivated");
        return NextDispatch();
    default:
        SetStep(&LocalAroundNetworkSearchJob::WaitAroundNetworkSearchActivated, "LocalAroundNetworkSearchJob::WaitAroundNetworkSearchActivated");
        return NextDispatch();
    }
}

// 0x0042194C | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::local::LocalAroundNetworkSearchJob::ReceiveAroundNetworkInfo()
{
    if (!GetManager()->m_IsSearching) {
        return StopSearch();
    }
    if (LocalNetwork::s_pInstance->IsDuringHostMigration()) {
        return EndSearch(true);
    }
    if (!LocalNetwork::s_pInstance->IsHost() && !LocalNetwork::s_pInstance->IsClient()) {
        return EndSearch(false);
    }
    // the host sends its command again to the stations that did not answer
    if (LocalNetwork::s_pInstance->IsHost() && !LocalNetwork::s_pInstance->IsDuringHostMigration() && GetManager()->IsSendingCommandMessage()) {
        if (GetManager()->m_IsSearching) {
            SetStep(&LocalAroundNetworkSearchJob::SendStartAroundNetworkSearchMessage, "LocalAroundNetworkSearchJob::SendStartAroundNetworkSearchMessage");
        } else {
            SetStep(&LocalAroundNetworkSearchJob::SendStopAroundNetworkSearchMessage, "LocalAroundNetworkSearchJob::SendStopAroundNetworkSearchMessage");
        }
        return Continue();
    }
    LifeTimeProcess();
    return NextDispatch();
}

// 0x00421C04 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::local::LocalAroundNetworkSearchJob::StartAroundNetworkSearch()
{
    LocalAroundNetworkSearchManager* pManager = GetManager();
    if (GetManager()->m_pBackgroundJob->Startup(m_pBackgroundCallContext, pManager->m_Setting).IsFailure()) {
        SetStep(&LocalAroundNetworkSearchJob::WaitAroundNetworkSearchActivated, "LocalAroundNetworkSearchJob::WaitAroundNetworkSearchActivated");
        return NextDispatch();
    }
    GetManager()->m_pBackgroundJob->Ready(true);
    SetStep(&LocalAroundNetworkSearchJob::WaitAroundNetworkSearch, "LocalAroundNetworkSearchJob::WaitAroundNetworkSearch");
    m_SearchStartTime.SetNow();
    // the stations do not scan at the same time
    pead::Random random;
    u32 value = random.getU32();
    m_SearchIntervalMsec = value % 3 * SEARCH_INTERVAL_STEP_MSEC + GetManager()->m_Setting.m_SearchIntervalMsec;
    return NextDispatch();
}

// 0x00421D88 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::local::LocalAroundNetworkSearchJob::WaitHostMigrationCompleted()
{
    LifeTimeProcess();
    if (LocalNetwork::s_pInstance->IsDuringHostMigration()) {
        return NextDispatch();
    }
    if (LocalNetwork::s_pInstance->IsHost()) {
        // the new host commands the search again
        GetManager()->PrepareNextCommandStatus();
        if (GetManager()->m_IsSearching) {
            SetStep(&LocalAroundNetworkSearchJob::SendStartAroundNetworkSearchMessage, "LocalAroundNetworkSearchJob::SendStartAroundNetworkSearchMessage");
        } else {
            SetStep(&LocalAroundNetworkSearchJob::SendStopAroundNetworkSearchMessage, "LocalAroundNetworkSearchJob::SendStopAroundNetworkSearchMessage");
        }
        return Continue();
    }
    SetStep(&LocalAroundNetworkSearchJob::WaitAroundNetworkSearchActivated, "LocalAroundNetworkSearchJob::WaitAroundNetworkSearchActivated");
    return Continue();
}

// 0x00421ED0 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::local::LocalAroundNetworkSearchJob::WaitAroundNetworkSearchActivated()
{
    LifeTimeProcess();
    if (GetManager()->m_IsSearching && !LocalNetwork::s_pInstance->IsDuringHostMigration()) {
        if (LocalNetwork::s_pInstance->IsHost()) {
            GetManager()->PrepareNextCommandStatus();
            SetStep(&LocalAroundNetworkSearchJob::SendStartAroundNetworkSearchMessage, "LocalAroundNetworkSearchJob::SendStartAroundNetworkSearchMessage");
            return Continue();
        }
        if (LocalNetwork::s_pInstance->IsClient()) {
            SetStep(&LocalAroundNetworkSearchJob::StartAroundNetworkSearch, "LocalAroundNetworkSearchJob::StartAroundNetworkSearch");
            return Continue();
        }
    }
    if (LocalNetwork::s_pInstance->IsHost() && !LocalNetwork::s_pInstance->IsDuringHostMigration() && GetManager()->IsSendingCommandMessage()) {
        if (GetManager()->m_IsSearching) {
            SetStep(&LocalAroundNetworkSearchJob::SendStartAroundNetworkSearchMessage, "LocalAroundNetworkSearchJob::SendStartAroundNetworkSearchMessage");
        } else {
            SetStep(&LocalAroundNetworkSearchJob::SendStopAroundNetworkSearchMessage, "LocalAroundNetworkSearchJob::SendStopAroundNetworkSearchMessage");
        }
        return Continue();
    }
    return NextDispatch();
}

// 0x004220B8 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::local::LocalAroundNetworkSearchJob::SendStopAroundNetworkSearchMessage()
{
    GetManager()->SendStopAroundNetworkSearchMessage();
    SetStep(&LocalAroundNetworkSearchJob::WaitSendStopAroundNetworkSearchMessageCompleted, "LocalAroundNetworkSearchJob::WaitSendStopAroundNetworkSearchMessageCompleted");
    m_SendTime.SetNow();
    return NextDispatch();
}

// 0x0042210C | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::local::LocalAroundNetworkSearchJob::SendStartAroundNetworkSearchMessage()
{
    GetManager()->SendStartAroundNetworkSearchMessage();
    SetStep(&LocalAroundNetworkSearchJob::WaitSendStartAroundNetworkSearchMessageCompleted, "LocalAroundNetworkSearchJob::WaitSendStartAroundNetworkSearchMessageCompleted");
    m_SendTime.SetNow();
    return NextDispatch();
}

// 0x00422160 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::local::LocalAroundNetworkSearchJob::WaitSendAroundNetworkStatusCompleted()
{
    if (!GetManager()->m_IsSearching) {
        return StopSearch();
    }
    if (LocalNetwork::s_pInstance->IsDuringHostMigration()) {
        return EndSearch(true);
    }
    if (!LocalNetwork::s_pInstance->IsHost() && !LocalNetwork::s_pInstance->IsClient()) {
        return EndSearch(false);
    }
    LifeTimeProcess();
    if (GetManager()->IsSendingStatusMessage()) {
        if (GetElapsedMSec(m_SendTime) > RESEND_INTERVAL_MSEC) {
            SetStep(&LocalAroundNetworkSearchJob::SendAroundNetworkStatus, "LocalAroundNetworkSearchJob::SendAroundNetworkStatus");
            return Continue();
        }
        return NextDispatch();
    }
    // the next scan
    if (GetElapsedMSec(m_SearchStartTime) > m_SearchIntervalMsec) {
        SetStep(&LocalAroundNetworkSearchJob::StartAroundNetworkSearch, "LocalAroundNetworkSearchJob::StartAroundNetworkSearch");
        return Continue();
    }
    return NextDispatch();
}

// 0x00422504 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::local::LocalAroundNetworkSearchJob::WaitSendStopAroundNetworkSearchMessageCompleted()
{
    if (LocalNetwork::s_pInstance->IsDuringHostMigration()) {
        return EndSearch(true);
    }
    if (!LocalNetwork::s_pInstance->IsHost() && !LocalNetwork::s_pInstance->IsClient()) {
        return EndSearch(false);
    }
    LifeTimeProcess();
    if (GetManager()->IsSendingCommandMessage()) {
        if (GetElapsedMSec(m_SendTime) > RESEND_INTERVAL_MSEC) {
            SetStep(&LocalAroundNetworkSearchJob::SendStopAroundNetworkSearchMessage, "LocalAroundNetworkSearchJob::SendStopAroundNetworkSearchMessage");
            return Continue();
        }
        return NextDispatch();
    }
    SetStep(&LocalAroundNetworkSearchJob::WaitAroundNetworkSearchActivated, "LocalAroundNetworkSearchJob::WaitAroundNetworkSearchActivated");
    return NextDispatch();
}

// 0x00422750 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::local::LocalAroundNetworkSearchJob::WaitSendStartAroundNetworkSearchMessageCompleted()
{
    if (!GetManager()->m_IsSearching) {
        return StopSearch();
    }
    if (LocalNetwork::s_pInstance->IsDuringHostMigration()) {
        return EndSearch(true);
    }
    if (!LocalNetwork::s_pInstance->IsHost() && !LocalNetwork::s_pInstance->IsClient()) {
        return EndSearch(false);
    }
    LifeTimeProcess();
    if (GetManager()->IsSendingCommandMessage()) {
        if (GetElapsedMSec(m_SendTime) > RESEND_INTERVAL_MSEC) {
            SetStep(&LocalAroundNetworkSearchJob::SendStartAroundNetworkSearchMessage, "LocalAroundNetworkSearchJob::SendStartAroundNetworkSearchMessage");
            return Continue();
        }
        return NextDispatch();
    }
    SetStep(&LocalAroundNetworkSearchJob::ReceiveAroundNetworkInfo, "LocalAroundNetworkSearchJob::ReceiveAroundNetworkInfo");
    return NextDispatch();
}

// 0x00422A64 (name is ours)
void nn::pia::local::LocalAroundNetworkSearchJob::Cleanup()
{
    m_pBackgroundCallContext->Cancel();
    LocalAroundNetworkSearchBackgroundJob* pBackgroundJob = GetManager()->m_pBackgroundJob;
    if (pBackgroundJob != nullptr) {
        pBackgroundJob->m_IsCancelRequested = true;
        pBackgroundJob->WaitForCompletion(2);
        pBackgroundJob->Cleanup();
    }
    if (m_pCallContext != nullptr) {
        if (m_pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
            m_pCallContext->SignalFailure(common::RESULT_CANCELED);
        }
        m_pCallContext = nullptr;
    }
}

// 0x00422AD8 | fefates:bytes [tier B]
nn::Result nn::pia::local::LocalAroundNetworkSearchJob::Startup(nn::pia::common::CallContext* pCallContext)
{
    m_pCallContext = pCallContext;
    pCallContext->Reset();
    m_pCallContext->InitiateCall();
    SetStep(&LocalAroundNetworkSearchJob::WaitAroundNetworkSearchActivated, "LocalAroundNetworkSearchJob::WaitAroundNetworkSearchActivated");
    return nn::Result();
}

// 0x00422B58
nn::pia::local::LocalAroundNetworkSearchJob::LocalAroundNetworkSearchJob()
    : m_pCallContext(nullptr), m_SendTime(), m_SearchStartTime(), m_LifeTimeTime(), m_SearchIntervalMsec(0)
{
    m_pBackgroundCallContext = common::NewObject<common::CallContext>();
}

// 0x00422C08
// 0x00422BC4 (deleting dtor)
nn::pia::local::LocalAroundNetworkSearchJob::~LocalAroundNetworkSearchJob()
{
    if (m_pBackgroundCallContext != nullptr) {
        common::DeleteObject(m_pBackgroundCallContext);
    }
}

// 0x007316C0
void nn::pia::local::LocalAroundNetworkSearchJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn
