#include "nn/pia/session/session_ConfigParticipationJobBase.h"
#include "nn/pia/common/common_Api.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_Scheduler.h"
#include "nn/pia/common/common_SessionStateMonitoringContent.h"
#include "nn/pia/common/common_TimeSpan.h"
#include "nn/pia/session/session_CommonMatchmakeSession.h"
#include "nn/pia/session/session_Mesh.h"
#include "nn/pia/session/session_Session.h"
#include "nn/pia/session/session_SessionProtocol.h"
#include "nn/pia/transport/transport_StationIdTable.h"
#include "nn/pia/transport/transport_Transport.h"

namespace nn {
namespace pia {
namespace session {
namespace {
// the dispatch time of the scheduler plus msec (name is ours)
inline common::Time GetDeadline(s32 msec)
{
    return common::Scheduler::s_pInstance->m_DispatchTime + common::TimeSpan(common::TimeSpan::GetTicksPerMSec().GetTick() * msec);
}

// the deadline is not reached yet (name is ours)
inline bool IsBeforeDeadline(const common::Time& deadline)
{
    return deadline >= common::Scheduler::s_pInstance->m_DispatchTime;
}

// the milliseconds since the time (name is ours)
inline s32 GetElapsedMSec(const common::Time& time)
{
    common::Time now;
    now.SetNow();
    return (now - time).m_Tick / common::TimeSpan::GetTicksPerMSec().m_Tick;
}
} // namespace

bool nn::pia::session::ConfigParticipationJobBase::IsSessionHost()
{
    Session* pSession = Session::s_pInstance;
    if (pSession->m_State == 4) {
        return pSession->IsHostOfBothSessions();
    }
    if (pSession->m_State == 2) {
        return pSession->IsHost();
    }
    return false;
}

void nn::pia::session::ConfigParticipationJobBase::FinishByTimeout()
{
    bool isFailure261 = IsSessionHost() ? m_Phase >= 5 : m_IsRequestedByHost;
    if (isFailure261) {
        Finish(common::RESULT_CONFIG_PARTICIPATION_FAILED_261);
    } else {
        Finish(common::RESULT_CONFIG_PARTICIPATION_FAILED_262);
    }
}

// 0x004435E0
nn::pia::common::ExecuteResult nn::pia::session::ConfigParticipationJobBase::SendFinish()
{
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_4) {
        m_pCallContext->SignalFailure(common::RESULT_SESSION_DISCONNECTED);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    m_Phase = 10;
    if (m_IsRestartRequested) {
        ClearState();
        SetupTargetStations();
        m_IsRestarted = true;
        m_Deadline = GetDeadline(START_TIMEOUT_MSEC);
        SetStep(&ConfigParticipationJobBase::SendStart, "ConfigParticipationJobBase::SendStart");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    if (m_Mode == MODE_CLOSE_PARTICIPATION) {
        Mesh::s_pInstance->m_Unknown0x61 = true;
    }
    m_Deadline = GetDeadline(FINISH_TIMEOUT_MSEC);
    RemoveInvalidTargetStations();
    // whether the session can be joined after the change
    bool value = false;
    Session* pSession = Session::s_pInstance;
    if (m_Mode == MODE_CLOSE_PARTICIPATION) {
        value = !Session::s_pInstance->IsJoinable(pSession->GetJoinedSessionId());
    } else if (m_Mode == MODE_OPEN_PARTICIPATION) {
        value = Session::s_pInstance->IsJoinable(pSession->GetJoinedSessionId());
    }
    nn::Result result =
        pSession->m_pSessionProtocol->SendMessage14(pSession->GetJoinedSessionId(), value, m_Mode, m_TargetStationIds, m_TargetStationNum);
    if (result == common::RESULT_BUFFER_IS_FULL) {
        if (IsBeforeDeadline(m_Deadline)) {
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
        FinishByTimeout();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (result != common::RESULT_NOT_FOUND && result.IsFailure()) {
        Finish(result);
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    m_RespondedStationNum = 0;
    m_Deadline = GetDeadline(FINISH_TIMEOUT_MSEC);
    SetStep(&ConfigParticipationJobBase::WaitFinishHost, "ConfigParticipationJobBase::WaitFinishHost");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x004439D0
nn::pia::common::ExecuteResult nn::pia::session::ConfigParticipationJobBase::ClientStart()
{
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_4) {
        m_pCallContext->SignalFailure(common::RESULT_SESSION_DISCONNECTED);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    m_Phase = 14;
    RemoveInvalidTargetStations();
    if (!Session::s_pInstance->IsValidStation(m_HostStationId)) {
        // the host left: the new one
        if (Session::s_pInstance->IsUsingStationIdTable()) {
            m_HostStationId = StationId(m_pSession->vf_0x90(), 0);
        } else {
            m_HostStationId = Session::s_pInstance->m_HostStationId;
        }
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    if (IsSessionHost()) {
        // the local station took over as the host
        ClearState();
        SetupTargetStations();
        SetStep(&ConfigParticipationJobBase::SendStart, "ConfigParticipationJobBase::SendStart");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    Session* pSession = Session::s_pInstance;
    if (m_HostStationId == pSession->m_LocalStationId) {
        if (IsBeforeDeadline(m_Deadline)) {
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
        FinishByTimeout();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (m_IsRequestedByHost) {
        // the answer to the host
        nn::Result result = pSession->m_pSessionProtocol->SendMessage13(m_HostStationId, m_Mode);
        if (result == common::RESULT_BUFFER_IS_FULL) {
            if (IsBeforeDeadline(m_Deadline)) {
                return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
            }
            FinishByTimeout();
            return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
        }
        if (result != common::RESULT_NOT_FOUND) {
            if (result.IsFailure()) {
                Finish(result);
                return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
            }
            SetStep(&ConfigParticipationJobBase::WaitFinishClient, "ConfigParticipationJobBase::WaitFinishClient");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
    } else if (IsSessionHost()) {
        ClearState();
        SetupTargetStations();
        SetStep(&ConfigParticipationJobBase::SendStart, "ConfigParticipationJobBase::SendStart");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    if (IsBeforeDeadline(m_Deadline)) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    FinishByTimeout();
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x00443F14
nn::pia::common::ExecuteResult nn::pia::session::ConfigParticipationJobBase::WaitP2PStable()
{
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_4) {
        m_pCallContext->SignalFailure(common::RESULT_SESSION_DISCONNECTED);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    m_Phase = 7;
    if (m_IsRestartRequested) {
        ClearState();
        SetupTargetStations();
        m_IsRestarted = true;
        m_Deadline = GetDeadline(START_TIMEOUT_MSEC);
        SetStep(&ConfigParticipationJobBase::SendStart, "ConfigParticipationJobBase::SendStart");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    RemoveInvalidTargetStations();
    // all stations of the session agree after a while
    if (GetElapsedMSec(m_StartTime) > P2P_STABLE_WAIT_MSEC) {
        Session* pSession = Session::s_pInstance;
        if (pSession->GetMeshLayerControllerValue() == pSession->GetStationNum()) {
            SetStep(&ConfigParticipationJobBase::SendFinish, "ConfigParticipationJobBase::SendFinish");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00444154 (name is ours)
void nn::pia::session::ConfigParticipationJobBase::Finish(const nn::Result& result)
{
    m_Result = result;
    if (m_pCallContext != nullptr) {
        if (result.IsSuccess()) {
            m_pCallContext->SignalSuccess(result);
        } else {
            m_pCallContext->SignalFailure(result);
        }
        m_pCallContext = nullptr;
    }
    m_Mode = MODE_NONE;
    ClearState();
    m_Counter = (m_Counter + 1) % 10;
}

// 0x004441D8
nn::pia::common::ExecuteResult nn::pia::session::ConfigParticipationJobBase::WaitFinishHost()
{
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_4) {
        m_pCallContext->SignalFailure(common::RESULT_SESSION_DISCONNECTED);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    m_Phase = 11;
    RemoveInvalidTargetStations();
    if (IsAllResponded()) {
        Session::s_pInstance->SetJoinable(m_SessionId, m_Mode == MODE_OPEN_PARTICIPATION);
        SetStep(&ConfigParticipationJobBase::SendEndConfigParticipation, "ConfigParticipationJobBase::SendEndConfigParticipation");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    if (IsBeforeDeadline(m_Deadline)) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    FinishByTimeout();
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x004443D8 (name is ours)
void nn::pia::session::ConfigParticipationJobBase::SetupTargetStations()
{
    for (s32 i = 0; i < 12; i++) {
        m_TargetStationIds[i] = GetStationIdOfIndex253();
    }
    m_TargetStationNum = 0;
    typedef common::ObjList<transport::StationIdTable::Entry> EntryList;
    transport::StationIdTable* pTable = transport::Transport::s_pInstance->m_pStationIdTable;
    Session* pSession = Session::s_pInstance;
    for (EntryList::Node* pNode = pTable->m_List.Begin(); pNode != pTable->m_List.End(); pNode = EntryList::Advance(pNode)) {
        if (pSession->IsValidStation(pNode->m_Value.m_StationId) && m_TargetStationNum < 12) {
            m_TargetStationIds[m_TargetStationNum++] = pNode->m_Value.m_StationId;
        }
    }
}

// 0x00444494 (name is ours)
void nn::pia::session::ConfigParticipationJobBase::AddRespondedStation(const nn::pia::StationId& stationId)
{
    for (u32 i = 0; i < m_TargetStationNum; i++) {
        if (m_TargetStationIds[i] == stationId) {
            for (u32 j = 0; j < m_RespondedStationNum; j++) {
                if (m_RespondedStationIds[j] == stationId) {
                    return;
                }
            }
            m_RespondedStationIds[m_RespondedStationNum++] = stationId;
            return;
        }
    }
}

// 0x0044453C
void nn::pia::session::ConfigParticipationJobBase::RemoveInvalidTargetStations()
{
    Session* pSession = Session::s_pInstance;
    for (u32 i = 0; i < 12; i++) {
        if (m_TargetStationIds[i] == GetStationIdOfIndex253() || pSession->IsValidStation(m_TargetStationIds[i])) {
            continue;
        }
        for (s32 j = i; j < 11; j++) {
            m_TargetStationIds[j] = m_TargetStationIds[j + 1];
        }
        m_TargetStationIds[11] = GetStationIdOfIndex253();
        m_TargetStationNum--;
    }
}

// 0x004445F8
nn::pia::common::ExecuteResult nn::pia::session::ConfigParticipationJobBase::WaitFinishClient()
{
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_4) {
        m_pCallContext->SignalFailure(common::RESULT_SESSION_DISCONNECTED);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    m_Phase = 15;
    RemoveInvalidTargetStations();
    if (IsSessionHost()) {
        ClearState();
        SetupTargetStations();
        SetStep(&ConfigParticipationJobBase::SendStart, "ConfigParticipationJobBase::SendStart");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    if (m_IsFinishReceived) {
        Session::s_pInstance->SetJoinable(m_SessionId, m_Mode == MODE_OPEN_PARTICIPATION);
        m_StartTime.SetNow();
        m_Deadline = GetDeadline(FINISH_TIMEOUT_MSEC);
        SetStep(&ConfigParticipationJobBase::WaitEndConfigParticipation, "ConfigParticipationJobBase::WaitEndConfigParticipation");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    if (IsBeforeDeadline(m_Deadline)) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    FinishByTimeout();
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x004448E0
nn::pia::common::ExecuteResult nn::pia::session::ConfigParticipationJobBase::OpenParticipation()
{
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_4) {
        m_pCallContext->SignalFailure(common::RESULT_SESSION_DISCONNECTED);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    m_Phase = 8;
    RemoveInvalidTargetStations();
    if (m_Mode == MODE_OPEN_PARTICIPATION) {
        Mesh::s_pInstance->m_Unknown0x61 = true;
    }
    Session::s_pInstance->SetJoinable(m_SessionId, true);
    nn::Result result = m_pSession->OpenParticipationAsync(&m_CallContext, m_SessionId);
    if (result.IsFailure()) {
        Finish(result);
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    SetStep(&ConfigParticipationJobBase::WaitOpenParticipation, "ConfigParticipationJobBase::WaitOpenParticipation");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00444A48 (name is ours)
void nn::pia::session::ConfigParticipationJobBase::AddRespondedStation2(const nn::pia::StationId& stationId)
{
    for (u32 i = 0; i < m_TargetStationNum; i++) {
        if (m_TargetStationIds[i] == stationId) {
            for (u32 j = 0; j < m_RespondedStationNum; j++) {
                if (m_RespondedStationIds[j] == stationId) {
                    return;
                }
            }
            m_RespondedStationIds[m_RespondedStationNum++] = stationId;
            return;
        }
    }
}

// 0x00444AF0 (name is ours)
bool nn::pia::session::ConfigParticipationJobBase::IsAllResponded()
{
    bool isAllResponded = true;
    StationId localStationId = Session::s_pInstance->m_LocalStationId;
    for (u32 i = 0; i < m_TargetStationNum; i++) {
        const StationId& stationId = m_TargetStationIds[i];
        if (stationId == GetStationIdOfIndex253() || stationId == localStationId || !Session::s_pInstance->IsValidStation(stationId)) {
            continue;
        }
        bool isResponded = false;
        for (u32 j = 0; j < m_RespondedStationNum; j++) {
            if (stationId == m_RespondedStationIds[j]) {
                isResponded = true;
                break;
            }
        }
        if (!isResponded) {
            isAllResponded = false;
            break;
        }
    }
    return isAllResponded;
}

// 0x00444BD8
nn::pia::common::ExecuteResult nn::pia::session::ConfigParticipationJobBase::CloseParticipation()
{
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_4) {
        m_pCallContext->SignalFailure(common::RESULT_SESSION_DISCONNECTED);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    m_Phase = 5;
    RemoveInvalidTargetStations();
    nn::Result result = m_pSession->CloseParticipationAsync(&m_CallContext, m_SessionId);
    if (result.IsFailure()) {
        Finish(result);
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    SetStep(&ConfigParticipationJobBase::WaitCloseParticipation, "ConfigParticipationJobBase::WaitCloseParticipation");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00444D0C
void nn::pia::session::ConfigParticipationJobBase::vf_0x2C()
{
    // empty (in the original too)
}

// 0x00444D10
void nn::pia::session::ConfigParticipationJobBase::vf_0x28()
{
    // empty (in the original too)
}

// 0x00444D14 (name is ours)
void nn::pia::session::ConfigParticipationJobBase::RequestRestart()
{
    m_IsRestartRequested = true;
}

// 0x00444D20
void nn::pia::session::ConfigParticipationJobBase::ReceiveFinish(u8)
{
    m_IsFinishReceived = true;
    m_Result = nn::Result();
}

// 0x00444D34 (name is ours)
void nn::pia::session::ConfigParticipationJobBase::SetEndReceived()
{
    m_IsEndReceived = true;
}

// 0x00444D40
nn::Result nn::pia::session::ConfigParticipationJobBase::ReceiveStartRequest(const nn::pia::StationId& hostStationId, u32 sessionId, u8 mode, u8 counter)
{
    m_HostStationId = hostStationId;
    m_SessionId = sessionId;
    m_pSession = Session::s_pInstance->GetJoinedMatchmakeSession();
    m_IsRequestedByHost = true;
    m_IsFinishReceived = false;
    if (m_Phase == 17) {
        // still waiting for the end of the last change: a new one starts after it
        if (m_Counter != counter) {
            m_PendingMode = mode;
            m_IsPendingRequest = true;
        } else {
            m_Deadline = GetDeadline(START_TIMEOUT_MSEC);
            SetStep(&ConfigParticipationJobBase::ClientStart, "ConfigParticipationJobBase::ClientStart");
        }
        return nn::Result();
    }
    if (m_Mode == MODE_NONE) {
        m_Mode = mode;
    } else if (m_Mode != mode) {
        SetStep(&ConfigParticipationJobBase::StartupPassiveProcessFailure, "ConfigParticipationJobBase::StartupPassiveProcessFailure");
        return nn::Result();
    }
    m_Counter = counter;
    m_Deadline = GetDeadline(START_TIMEOUT_MSEC);
    SetStep(&ConfigParticipationJobBase::ClientStart, "ConfigParticipationJobBase::ClientStart");
    return nn::Result();
}

// 0x00444EF8
nn::pia::common::ExecuteResult nn::pia::session::ConfigParticipationJobBase::WaitOpenParticipation()
{
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_4) {
        m_pCallContext->SignalFailure(common::RESULT_SESSION_DISCONNECTED);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    m_Phase = 9;
    RemoveInvalidTargetStations();
    if (!m_pSession->IsOpenParticipationCompleted()) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    switch (m_CallContext.m_State) {
    case common::CallContext::STATE_CALL_SUCCESS:
        SetStep(&ConfigParticipationJobBase::SendFinish, "ConfigParticipationJobBase::SendFinish");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    case common::CallContext::STATE_CALL_FAILURE:
        Finish(m_CallContext.m_Result);
        // falls through (in the original too)
    default:
        Finish(common::RESULT_INVALID_STATE);
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
}

// 0x00445078
nn::pia::common::ExecuteResult nn::pia::session::ConfigParticipationJobBase::WaitCloseParticipation()
{
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_4) {
        m_pCallContext->SignalFailure(common::RESULT_SESSION_DISCONNECTED);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    m_Phase = 6;
    RemoveInvalidTargetStations();
    if (!m_pSession->IsCloseParticipationCompleted()) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    switch (m_CallContext.m_State) {
    case common::CallContext::STATE_CALL_SUCCESS:
        SetStep(&ConfigParticipationJobBase::WaitP2PStable, "ConfigParticipationJobBase::WaitP2PStable");
        m_StartTime.SetNow();
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    case common::CallContext::STATE_CALL_FAILURE:
        Finish(m_CallContext.m_Result);
        // falls through (in the original too)
    default:
        Finish(common::RESULT_INVALID_STATE);
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
}

// 0x00445204
nn::pia::common::ExecuteResult nn::pia::session::ConfigParticipationJobBase::SendEndConfigParticipation()
{
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_4) {
        m_pCallContext->SignalFailure(common::RESULT_SESSION_DISCONNECTED);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    m_Phase = 16;
    RemoveInvalidTargetStations();
    Session* pSession = Session::s_pInstance;
    nn::Result result = pSession->m_pSessionProtocol->SendMessage15(pSession->GetJoinedSessionId(), m_Mode, m_TargetStationIds, m_TargetStationNum);
    if (result == common::RESULT_BUFFER_IS_FULL) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    if (result != common::RESULT_NOT_FOUND && result.IsFailure()) {
        Mesh::s_pInstance->m_Unknown0x61 = false;
        Finish(result);
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    Mesh::s_pInstance->m_Unknown0x61 = false;
    Finish(nn::Result());
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x00445378
nn::pia::common::ExecuteResult nn::pia::session::ConfigParticipationJobBase::WaitEndConfigParticipation()
{
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_4) {
        m_pCallContext->SignalFailure(common::RESULT_SESSION_DISCONNECTED);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    m_Phase = 17;
    RemoveInvalidTargetStations();
    if (IsSessionHost()) {
        // the local station took over as the host: it tells the stations to finish
        SetupTargetStations();
        m_Deadline = GetDeadline(FINISH_TIMEOUT_MSEC);
        Session* pSession = Session::s_pInstance;
        bool value = vf_0x24(pSession->GetJoinedSessionId());
        nn::Result result =
            pSession->m_pSessionProtocol->SendMessage14(pSession->GetJoinedSessionId(), value, m_Mode, m_TargetStationIds, m_TargetStationNum);
        if (result == common::RESULT_BUFFER_IS_FULL) {
            if (IsBeforeDeadline(m_Deadline)) {
                return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
            }
            FinishByTimeout();
            return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
        }
        if (result != common::RESULT_NOT_FOUND && result.IsFailure()) {
            Finish(result);
            return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
        }
        SetStep(&ConfigParticipationJobBase::WaitFinishHost, "ConfigParticipationJobBase::WaitFinishHost");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    if (m_IsEndReceived) {
        Finish(nn::Result());
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (GetElapsedMSec(m_StartTime) > END_REQUEST_INTERVAL_MSEC) {
        if (!Session::s_pInstance->IsValidStation(m_HostStationId)) {
            // the host left: the new one
            if (Session::s_pInstance->IsUsingStationIdTable()) {
                m_HostStationId = StationId(m_pSession->vf_0x90(), 0);
            } else {
                m_HostStationId = Session::s_pInstance->m_HostStationId;
            }
            m_Deadline = GetDeadline(FINISH_TIMEOUT_MSEC);
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
        Session* pSession = Session::s_pInstance;
        if (m_HostStationId == pSession->m_LocalStationId) {
            m_Deadline = GetDeadline(FINISH_TIMEOUT_MSEC);
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
        // asks the host whether the job still runs
        pSession->m_pSessionProtocol->SendMessage23(m_HostStationId, m_Mode);
        m_StartTime.SetNow();
    }
    if (IsBeforeDeadline(m_Deadline)) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    FinishByTimeout();
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x00445958
nn::pia::common::ExecuteResult nn::pia::session::ConfigParticipationJobBase::StartupPassiveProcessFailure()
{
    Finish(common::RESULT_CONFIG_PARTICIPATION_FAILED_261);
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x00445984
void nn::pia::session::ConfigParticipationJobBase::vf_0x38()
{
    // empty (in the original too)
}

// 0x00445988
void nn::pia::session::ConfigParticipationJobBase::vf_0x3C()
{
    // empty (in the original too)
}

// 0x0044598C
void nn::pia::session::ConfigParticipationJobBase::Cleanup()
{
    if (m_pCallContext != nullptr) {
        if (m_pCallContext->m_State == common::CallContext::STATE_CALL_IN_PROGRESS) {
            m_pCallContext->SignalCancel();
        }
        m_pCallContext = nullptr;
    }
    if (m_CallContext.m_State == common::CallContext::STATE_CALL_IN_PROGRESS) {
        m_CallContext.SignalCancel();
    }
    m_CallContext.Reset();
    ClearState();
    m_IsRequestedByHost = false;
    m_Mode = MODE_NONE;
    m_Unknown0x13C = 0;
}

// 0x004459F0
nn::Result nn::pia::session::ConfigParticipationJobBase::Startup(nn::pia::common::CallContext* pCallContext, u32 sessionId,
                                                                  nn::pia::session::CommonMatchmakeSession* pSession, u8 mode)
{
    m_Phase = 0;
    if (!common::IsValidPointer(pCallContext) || !common::IsValidPointer(pSession)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_Mode != MODE_NONE && m_Mode != mode) {
        return common::RESULT_CONFIG_PARTICIPATION_FAILED_262;
    }
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        return common::RESULT_NOT_IN_SESSION;
    }
    m_pCallContext = pCallContext;
    pCallContext->InitiateCall();
    Reset(true);
    m_Mode = mode;
    if (m_HostStationId == GetStationIdOfIndex253()) {
        // not asked by a host yet
        m_pSession = pSession;
        m_SessionId = sessionId;
        if (IsSessionHost()) {
            SetupTargetStations();
            SetStep(&ConfigParticipationJobBase::SendStart, "ConfigParticipationJobBase::SendStart");
        } else {
            if (Session::s_pInstance->IsUsingStationIdTable()) {
                m_HostStationId = StationId(m_pSession->vf_0x90(), 0);
            } else {
                m_HostStationId = Session::s_pInstance->m_HostStationId;
            }
            SetStep(&ConfigParticipationJobBase::ClientStart, "ConfigParticipationJobBase::ClientStart");
        }
    }
    // the monitoring data counts the changes
    common::SessionStateMonitoringContent& content = common::g_SessionStateMonitoringContent;
    if (m_Mode == MODE_CLOSE_PARTICIPATION) {
        u8 count = content.m_Unknown0x3D7;
        content.m_Unknown0x3D7 = count == 0xFF ? 1 : count + 1;
    } else if (m_Mode == MODE_OPEN_PARTICIPATION) {
        u8 count = content.m_Unknown0x3D6;
        content.m_Unknown0x3D6 = count == 0xFF ? 1 : count + 1;
    }
    m_Deadline = GetDeadline(START_TIMEOUT_MSEC);
    return nn::Result();
}

// 0x00445C64
void nn::pia::session::ConfigParticipationJobBase::ClearState()
{
    m_IsRequestedByHost = false;
    m_IsFinishReceived = false;
    m_Unknown0x66 = 0;
    m_Phase = 0;
    for (s32 i = 0; i < 12; i++) {
        m_TargetStationIds[i] = GetStationIdOfIndex253();
        m_RespondedStationIds[i] = GetStationIdOfIndex253();
    }
    m_TargetStationNum = 0;
    m_RespondedStationNum = 0;
    m_HostStationId = GetStationIdOfIndex253();
    m_Unknown0x13C = 0;
    m_Unknown0x13D = 0;
    m_Unknown0x157 = 0;
    m_Unknown0x158 = 0;
    m_IsRestartRequested = false;
    m_IsRestarted = false;
    m_IsEndReceived = false;
    // a start request that arrived while waiting is taken now
    if (m_IsPendingRequest) {
        m_IsRequestedByHost = true;
        m_Mode = m_PendingMode;
    }
    m_IsPendingRequest = false;
    m_PendingMode = 0;
}

// 0x00445D24
nn::pia::common::ExecuteResult nn::pia::session::ConfigParticipationJobBase::SendStart()
{
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_4) {
        m_pCallContext->SignalFailure(common::RESULT_SESSION_DISCONNECTED);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    m_Phase = 3;
    RemoveInvalidTargetStations();
    Session* pSession = Session::s_pInstance;
    nn::Result result =
        pSession->m_pSessionProtocol->SendMessage12(pSession->GetJoinedSessionId(), m_Mode, m_TargetStationIds, m_TargetStationNum, m_Counter);
    if (result == common::RESULT_BUFFER_IS_FULL) {
        if (IsBeforeDeadline(m_Deadline)) {
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
        FinishByTimeout();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (result != common::RESULT_NOT_FOUND && result.IsFailure()) {
        Finish(result);
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    SetStep(&ConfigParticipationJobBase::WaitStart, "ConfigParticipationJobBase::WaitStart");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00445F74
nn::pia::common::ExecuteResult nn::pia::session::ConfigParticipationJobBase::WaitStart()
{
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_4) {
        m_pCallContext->SignalFailure(common::RESULT_SESSION_DISCONNECTED);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    m_Phase = 4;
    if (m_IsRestartRequested) {
        ClearState();
        SetupTargetStations();
        m_Deadline = GetDeadline(START_TIMEOUT_MSEC);
        SetStep(&ConfigParticipationJobBase::SendStart, "ConfigParticipationJobBase::SendStart");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    RemoveInvalidTargetStations();
    if (IsAllResponded()) {
        if (m_IsRestarted) {
            SetStep(&ConfigParticipationJobBase::SendFinish, "ConfigParticipationJobBase::SendFinish");
        } else if (m_Mode == MODE_CLOSE_PARTICIPATION) {
            SetStep(&ConfigParticipationJobBase::CloseParticipation, "ConfigParticipationJobBase::CloseParticipation");
        } else if (m_Mode == MODE_OPEN_PARTICIPATION) {
            SetStep(&ConfigParticipationJobBase::OpenParticipation, "ConfigParticipationJobBase::OpenParticipation");
        }
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    if (IsBeforeDeadline(m_Deadline)) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    FinishByTimeout();
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x004462AC
nn::pia::session::ConfigParticipationJobBase::ConfigParticipationJobBase()
    : m_pCallContext(nullptr), m_Mode(MODE_NONE), m_SessionId(0), m_pSession(nullptr), m_IsRequestedByHost(false), m_IsFinishReceived(false),
      m_Unknown0x66(0), m_TargetStationNum(0), m_RespondedStationNum(0), m_HostStationId(GetStationIdOfIndex253()), m_Result(common::RESULT_NOT_SET),
      m_Unknown0x13C(0), m_Unknown0x13D(0), m_IsRestartRequested(false), m_IsRestarted(false), m_IsEndReceived(false), m_IsPendingRequest(false),
      m_Counter(0), m_Phase(0), m_Unknown0x157(0), m_Unknown0x158(0)
{
}

// 0x004463AC
// 0x00446388 (deleting dtor)
nn::pia::session::ConfigParticipationJobBase::~ConfigParticipationJobBase()
{
    // empty (in the original too)
}

// 0x00734178
bool nn::pia::session::ConfigParticipationJobBase::vf_0x24(u32)
{
    return false;
}

// 0x00734180
u8 nn::pia::session::ConfigParticipationJobBase::vf_0x34(const nn::pia::StationId& stationId, u32 sessionId)
{
    u8 targetState = 2;
    for (s32 i = 0; i < 12; i++) {
        if (m_TargetStationIds[i] == stationId) {
            targetState = 1;
            break;
        }
    }
    u8 value = vf_0x24(sessionId);
    u8 state = Session::s_pInstance->m_State;
    bool isInSession = state == 4 || state == 2;
    return static_cast<u8>(targetState << 4 | value << 2 | isInSession);
}

// 0x0073420C
void nn::pia::session::ConfigParticipationJobBase::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace session
} // namespace pia
} // namespace nn
