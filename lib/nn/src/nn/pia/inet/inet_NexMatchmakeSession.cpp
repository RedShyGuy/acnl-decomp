#include "nn/pia/inet/inet_NexMatchmakeSession.h"
#include "nn/nex/nex_AutoMatchmakeParam.h"
#include "nn/nex/nex_CreateMatchmakeSessionParam.h"
#include "nn/nex/nex_ErrorCodeConverter.h"
#include "nn/nex/nex_JoinMatchmakeSessionParam.h"
#include "nn/nex/nex_MatchmakeExtensionClient.h"
#include "nn/nex/nex_MatchmakeParam.h"
#include "nn/nex/nex_MatchmakeSession.h"
#include "nn/nex/nex_MatchmakeSessionSearchCriteria.h"
#include "nn/nex/nex_NgsBridgeInterface.h"
#include "nn/nex/nex_PlayingSession.h"
#include "nn/nex/nex_ResultRange.h"
#include "nn/nex/nex_StationURL.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_UpdateMatchmakeSessionParam.h"
#include "nn/nex/nex_Variant.h"
#include "nn/pia/common/common_DateTime.h"
#include "nn/pia/common/common_HeapManager.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_Scheduler.h"
#include "nn/pia/common/common_SessionBeginMonitoringContent.h"
#include "nn/pia/common/common_SessionStateMonitoringContent.h"
#include "nn/pia/common/common_TimeSpan.h"
#include "nn/pia/inet/inet_NexCreateSessionSetting.h"
#include "nn/pia/inet/inet_NexFacade.h"
#include "nn/pia/inet/inet_NexJoinSessionSetting.h"
#include "nn/pia/inet/inet_NexMatchMeshLayerController.h"
#include "nn/pia/inet/inet_NexSessionInfo.h"
#include "nn/pia/inet/inet_NexSessionSearchCriteria.h"
#include "nn/pia/inet/inet_NexUpdateSessionSetting.h"
#include "nn/pia/session/session_Session.h"
#include "nn/pia/session/session_SessionInfoList.h"
#include "pead/peadHeapMgr.h"
#include <cstring>

namespace nn {
namespace pia {
namespace inet {
namespace {
// the results of nex the session maps to the ones of pia (the names are ours)
const s32 NEX_RESULT_INVALID_ARGUMENT = static_cast<s32>(0x8001000A);
const s32 NEX_RESULT_80030073 = static_cast<s32>(0x80030073);
const s32 NEX_RESULT_800300C8 = static_cast<s32>(0x800300C8);
const s32 NEX_RESULT_800300CD = static_cast<s32>(0x800300CD);
const s32 NEX_RESULT_800300CE = static_cast<s32>(0x800300CE);
const s32 NEX_RESULT_800300CF = static_cast<s32>(0x800300CF);
const s32 NEX_RESULT_800300D1 = static_cast<s32>(0x800300D1);
const s32 NEX_RESULT_800300D4 = static_cast<s32>(0x800300D4);
const s32 NEX_RESULT_800300D5 = static_cast<s32>(0x800300D5);
const s32 NEX_RESULT_800300D6 = static_cast<s32>(0x800300D6);
const s32 NEX_RESULT_800300D7 = static_cast<s32>(0x800300D7);
const s32 NEX_RESULT_800300D8 = static_cast<s32>(0x800300D8);
const s32 NEX_RESULT_800300D9 = static_cast<s32>(0x800300D9);
const s32 NEX_RESULT_800300DA = static_cast<s32>(0x800300DA);
const s32 NEX_RESULT_800300DB = static_cast<s32>(0x800300DB);

// the flag of the trace of the session infos
const u64 TRACE_FLAG = 0x40000000;

typedef session::SessionInfoList<NexSessionInfo> NexSessionInfoList;

inline common::Time GetTimeAfter(s64 msec)
{
    return common::Scheduler::s_pInstance->m_DispatchTime + common::TimeSpan(common::TimeSpan::GetTicksPerMSec().GetTick() * msec);
}

inline bool IsExpired(const common::Time& deadline)
{
    return !(deadline >= common::Scheduler::s_pInstance->m_DispatchTime);
}

inline void RemoveSessionEntry(u32 sessionId)
{
    if (session::Session::s_pInstance->m_pMeshLayerController != nullptr) {
        static_cast<NexMatchMeshLayerController*>(session::Session::s_pInstance->m_pMeshLayerController)->RemoveSessionEntry(sessionId);
    }
}

// the counter of the monitoring data goes from 255 back to 1
inline void IncrementCount(u8* pCount)
{
    if (*pCount == 0xFF) {
        *pCount = 1;
    } else {
        (*pCount)++;
    }
}

// the type of pia for the one of nex (inline; name is ours)
inline u8 ConvertNexMatchmakeSystemType(u32 type)
{
    switch (static_cast<u8>(type)) {
    case 1:
        return 0;
    case 2:
        return 1;
    default:
        return 0;
    }
}

// the values of the matchmake session go into the session info (inline; name is ours)
void SetSessionInfo(NexSessionInfo* pInfo, const nex::MatchmakeSession* pSession, const nex::qVector<u8>& buffer)
{
    pInfo->vf_0x5C(pSession->m_GameMode);
    pInfo->vf_0x60(pSession->m_Id);
    for (u8 i = 0; i < NexCreateSessionSetting::ATTRIBUTE_NUM; i++) {
        pInfo->vf_0x74(pSession->GetAttribute(i), i);
    }
    pInfo->vf_0x64(pSession->m_ParticipationCount);
    pInfo->vf_0x68(pSession->m_MinParticipants);
    pInfo->vf_0x6C(pSession->m_MaxParticipants);
    pInfo->vf_0x70(pSession->m_IsOpenParticipation);
    u8 data[512];
    u32 size = buffer.size();
    if (size > sizeof(data)) {
        size = sizeof(data);
    }
    for (u16 i = 0; i < size; i++) {
        data[i] = buffer[i];
    }
    pInfo->vf_0x7C(data, size);
}
} // namespace

// 0x00975A98 (name is ours)
u32 nn::pia::inet::NexMatchmakeSession::s_NetworkErrorCode;

// 0x003F92E8 (name is ours)
nn::Result nn::pia::inet::NexMatchmakeSession::Bind(nn::nex::NgsBridgeInterface* pNgsBridge)
{
    m_pNgsBridge = pNgsBridge;
    nex::Credentials* pCredentials = pNgsBridge->GetCredentials();
    if (pCredentials == nullptr) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (!m_pClient->Bind(pCredentials)) {
        return common::RESULT_INVALID_STATE;
    }
    return nn::Result();
}

// 0x003F9338 slot 0x60
nn::Result nn::pia::inet::NexMatchmakeSession::vf_0x60(nn::pia::common::CallContext* pCallContext, u32 sessionId)
{
    if (!common::IsValidPointer(pCallContext) || pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_pCallContext != nullptr || m_NexCallContext.m_State == nex::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_STATE;
    }
    m_pCallContext = pCallContext;
    if (!m_pClient->FindBySingleID(&m_NexCallContext, sessionId, &m_Gathering)) {
        m_pCallContext = nullptr;
        return common::RESULT_UNREGISTER_FAILED;
    }
    m_pCallContext->InitiateCall();
    return nn::Result();
}

// 0x003F93C4 slot 0xF0 (name is ours)
bool nn::pia::inet::NexMatchmakeSession::IsFindGatheringCompleted(nn::nex::Gathering** ppGathering)
{
    switch (m_NexCallContext.m_State) {
    case nex::CallContext::STATE_FAILURE: {
        nex::qResult result = m_NexCallContext.m_Result;
        if (result.m_Code == NEX_RESULT_80030073) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_SESSION_GONE);
        } else if (result.m_Code == NEX_RESULT_800300CF) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_247);
        } else {
            s_NetworkErrorCode = nex::ErrorCodeConverter::ConvertToNetworkErrorCode(result);
            m_pCallContext->SignalFailure(common::RESULT_FATAL_196);
        }
        delete m_Gathering.Release();
        break;
    }
    case nex::CallContext::STATE_SUCCESS:
        *ppGathering = m_Gathering.Release();
        m_pCallContext->SignalSuccess(nn::Result());
        break;
    default:
        return false;
    }
    m_pCallContext = nullptr;
    m_NexCallContext.Reset();
    return true;
}

// 0x003F94C4 slot 0x58
nn::Result nn::pia::inet::NexMatchmakeSession::ModifyAttributeAsync(nn::pia::common::CallContext* pCallContext, u32 sessionId, u32 index, u32 value)
{
    if (!common::IsValidPointer(pCallContext) || pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_pCallContext != nullptr || m_NexCallContext.m_State == nex::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_STATE;
    }
    if (index >= NexCreateSessionSetting::ATTRIBUTE_NUM) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    m_pCallContext = pCallContext;
    if (!m_pClient->ModifyCurrentGameAttribute(&m_NexCallContext, sessionId, index, value)) {
        m_pCallContext = nullptr;
        return common::RESULT_UNREGISTER_FAILED;
    }
    m_pCallContext->InitiateCall();
    return nn::Result();
}

// 0x003F9564 slot 0x40
nn::Result nn::pia::inet::NexMatchmakeSession::OpenParticipationAsync(nn::pia::common::CallContext* pCallContext, u32 sessionId)
{
    if (!common::IsValidPointer(pCallContext) || pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_pCallContext != nullptr || m_NexCallContext.m_State == nex::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_STATE;
    }
    m_pCallContext = pCallContext;
    if (!m_pClient->OpenParticipation(&m_NexCallContext, sessionId)) {
        m_pCallContext = nullptr;
        return common::RESULT_UNREGISTER_FAILED;
    }
    m_pCallContext->InitiateCall();
    return nn::Result();
}

// 0x003F95EC slot 0x48
nn::Result nn::pia::inet::NexMatchmakeSession::CloseParticipationAsync(nn::pia::common::CallContext* pCallContext, u32 sessionId)
{
    if (!common::IsValidPointer(pCallContext) || pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_pCallContext != nullptr || m_NexCallContext.m_State == nex::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_STATE;
    }
    m_pCallContext = pCallContext;
    if (!m_pClient->CloseParticipation(&m_NexCallContext, sessionId)) {
        m_pCallContext = nullptr;
        return common::RESULT_UNREGISTER_FAILED;
    }
    m_pCallContext->InitiateCall();
    return nn::Result();
}

// 0x003F9674 slot 0x64
bool nn::pia::inet::NexMatchmakeSession::vf_0x64(u32 index, u32* pValue)
{
    switch (m_NexCallContext.m_State) {
    case nex::CallContext::STATE_FAILURE: {
        nex::qResult result = m_NexCallContext.m_Result;
        if (result.m_Code == NEX_RESULT_80030073) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_SESSION_GONE);
        } else if (result.m_Code == NEX_RESULT_800300CF) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_247);
        } else {
            s_NetworkErrorCode = nex::ErrorCodeConverter::ConvertToNetworkErrorCode(result);
            m_pCallContext->SignalFailure(common::RESULT_FATAL_196);
        }
        m_pCallContext = nullptr;
        break;
    }
    case nex::CallContext::STATE_SUCCESS: {
        nex::MatchmakeSession* pSession = static_cast<nex::MatchmakeSession*>(m_Gathering.Get());
        *pValue = pSession->GetAttribute(index);
        m_pCallContext->SignalSuccess(nn::Result());
        m_pCallContext = nullptr;
        pSession->Reset();
        m_Gathering.m_pObject = nullptr;
        delete pSession;
        break;
    }
    default:
        return false;
    }
    m_NexCallContext.Reset();
    return true;
}

// 0x003F9788 slot 0xCC
nn::Result nn::pia::inet::NexMatchmakeSession::FindSessionByOwnerAsync(nn::pia::common::CallContext* pCallContext, u32 ownerPrincipalId, u32 offset, u32 num)
{
    if (!common::IsValidPointer(pCallContext) || pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_pCallContext != nullptr || m_NexCallContext.m_State == nex::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_STATE;
    }
    m_pCallContext = pCallContext;
    nex::ResultRange range(offset, num);
    if (!m_pClient->FindByOwner(&m_NexCallContext, ownerPrincipalId, range, m_pGatherings)) {
        m_pCallContext = nullptr;
        return common::RESULT_INVALID_STATE;
    }
    m_pCallContext->InitiateCall();
    m_pSessionInfoList->Clear();
    return nn::Result();
}

// 0x003F9858 slot 0xE4 (name is ours)
nn::Result nn::pia::inet::NexMatchmakeSession::GetJoinedSessionsAsync(nn::pia::common::CallContext* pCallContext, u32 principalId)
{
    if (!common::IsValidPointer(pCallContext) || pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_pCallContext != nullptr || m_NexCallContext.m_State == nex::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_STATE;
    }
    m_pCallContext = pCallContext;
    m_pPrincipalIds->push_back(principalId);
    if (!m_pClient->GetPlayingSession(&m_NexCallContext, *m_pPrincipalIds, m_pPlayingSessions)) {
        m_pCallContext = nullptr;
        return common::RESULT_UNREGISTER_FAILED;
    }
    m_pCallContext->InitiateCall();
    return nn::Result();
}

// 0x003F9958 (name is ours)
void nn::pia::inet::NexMatchmakeSession::SetJoinSetting(const NexJoinSessionSetting* pSetting)
{
    m_pJoinParam->m_Unknown0x30 = nex::String(pSetting->m_Unknown0x8);
    m_pJoinParam->m_Unknown0x38 = nex::String(pSetting->m_Unknown0x4A);
}

// 0x003F99C8 slot 0xC4
nn::Result nn::pia::inet::NexMatchmakeSession::ClearSystemPasswordAsync(nn::pia::common::CallContext* pCallContext, u32 sessionId)
{
    if (!common::IsValidPointer(pCallContext) || pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_pCallContext != nullptr) {
        return common::RESULT_INVALID_STATE;
    }
    m_pCallContext = pCallContext;
    if (!m_pClient->ClearMatchmakeSessionSystemPassword(&m_NexCallContext, sessionId)) {
        m_pCallContext = nullptr;
        return common::RESULT_UNREGISTER_FAILED;
    }
    m_pCallContext->InitiateCall();
    return nn::Result();
}

// 0x003F9A44 slot 0x5C
bool nn::pia::inet::NexMatchmakeSession::IsModifyAttributeCompleted()
{
    switch (m_NexCallContext.m_State) {
    case nex::CallContext::STATE_FAILURE: {
        nex::qResult result = m_NexCallContext.m_Result;
        if (result.m_Code == NEX_RESULT_80030073) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_SESSION_GONE);
        } else if (result.m_Code == NEX_RESULT_800300D9) {
            m_pCallContext->SignalFailure(common::RESULT_INVALID_STATE);
        } else {
            s_NetworkErrorCode = nex::ErrorCodeConverter::ConvertToNetworkErrorCode(result);
            m_pCallContext->SignalFailure(common::RESULT_FATAL_196);
        }
        break;
    }
    case nex::CallContext::STATE_SUCCESS:
        m_pCallContext->SignalSuccess(nn::Result());
        break;
    default:
        return false;
    }
    m_pCallContext = nullptr;
    m_NexCallContext.Reset();
    return true;
}

// 0x003F9B1C slot 0x44
bool nn::pia::inet::NexMatchmakeSession::IsOpenParticipationCompleted()
{
    switch (m_NexCallContext.m_State) {
    case nex::CallContext::STATE_FAILURE: {
        nex::qResult result = m_NexCallContext.m_Result;
        if (result.m_Code == NEX_RESULT_80030073) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_SESSION_GONE);
        } else if (result.m_Code == NEX_RESULT_800300D9) {
            m_pCallContext->SignalFailure(common::RESULT_INVALID_STATE);
        } else {
            s_NetworkErrorCode = nex::ErrorCodeConverter::ConvertToNetworkErrorCode(result);
            m_pCallContext->SignalFailure(common::RESULT_FATAL_196);
        }
        m_pCallContext = nullptr;
        m_NexCallContext.Reset();
        return true;
    }
    case nex::CallContext::STATE_SUCCESS:
        m_pCallContext->SignalSuccess(nn::Result());
        m_pCallContext = nullptr;
        m_NexCallContext.Reset();
        m_Unknown0x4 = true;
        common::g_SessionStateMonitoringContent.m_Unknown0x3C9 = 1;
        return true;
    default:
        return false;
    }
}

// 0x003F9C1C slot 0x0C
nn::Result nn::pia::inet::NexMatchmakeSession::AutoMatchmakeAsync(nn::pia::common::CallContext* pCallContext)
{
    if (!common::IsValidPointer(pCallContext) || pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_pCallContext != nullptr || m_NexCallContext.m_State == nex::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_STATE;
    }
    m_pCallContext = pCallContext;
    bool isCalled;
    switch (m_MatchmakeSystemType) {
    case 0: {
        nex::AutoMatchmakeParam param;
        param.SetSearchCriteria(*m_pSearchCriteria);
        param.m_SourceMatchmakeSession = *m_pSourceSession;
        param.m_JoinMessage = m_pSourceSession->m_Description;
        isCalled = m_pClient->AutoMatchmake(&m_NexCallContext, param, m_pMatchmakeSession);
        break;
    }
    case 2:
        m_SourceGathering.Reset(m_pSourceSession);
        isCalled = m_pClient->AutoMatchmakePostpone(&m_NexCallContext, m_SourceGathering, &m_Gathering, nex::String(L""));
        break;
    }
    if (!isCalled) {
        m_pCallContext = nullptr;
        return common::RESULT_UNREGISTER_FAILED;
    }
    m_pCallContext->InitiateCall();
    return nn::Result();
}

// 0x003F9D90 slot 0x28
nn::Result nn::pia::inet::NexMatchmakeSession::JoinAsync(nn::pia::common::CallContext* pCallContext, u32 sessionId)
{
    if (!common::IsValidPointer(pCallContext) || pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_pCallContext != nullptr || m_NexCallContext.m_State == nex::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_STATE;
    }
    m_pCallContext = pCallContext;
    m_pJoinParam->m_GatheringId = sessionId;
    if (!m_pClient->JoinMatchmakeSession(&m_NexCallContext, *m_pJoinParam, m_pMatchmakeSession)) {
        m_pCallContext = nullptr;
        return common::RESULT_UNREGISTER_FAILED;
    }
    pCallContext->InitiateCall();
    return nn::Result();
}

// 0x003F9E2C (name is ours)
void nn::pia::inet::NexMatchmakeSession::SetSessionSetting(const NexUpdateSessionSetting* pSetting)
{
    m_pUpdateParam->Reset();
    m_pUpdateParam->m_ModificationFlags = pSetting->m_ModificationFlags;
    m_pUpdateParam->m_MaxParticipants = pSetting->m_MaxParticipants;
    m_pUpdateParam->m_MinParticipants = pSetting->m_MinParticipants;
    nex::UpdateMatchmakeSessionParam* pParam = m_pUpdateParam;
    pParam->m_MatchmakeSystemType = ConvertMatchmakeSystemType(pSetting->m_MatchmakeSystemType);
    if (pParam->m_MatchmakeSystemType == 1) {
        pParam->m_ParticipationPolicy = 95;
        pParam->m_PolicyArgument = 0;
    } else if (pParam->m_MatchmakeSystemType == 2) {
        pParam->m_ParticipationPolicy = 98;
        pParam->m_PolicyArgument = 0;
    } else {
        pParam->m_ParticipationPolicy = 96;
        pParam->m_PolicyArgument = 0;
    }
    m_pUpdateParam->m_Description = nex::String(pSetting->m_Description);
    m_pUpdateParam->m_IsOpenParticipation = pSetting->IsOpenParticipation();
    m_pUpdateParam->m_UserPassword = nex::String(pSetting->GetUserPassword());
    if (pSetting->m_ProgressScore <= 100) {
        m_pUpdateParam->m_ProgressScore = pSetting->m_ProgressScore;
    }
    nex::qVector<u8> data;
    pSetting->GetApplicationData(&data);
    m_pUpdateParam->SetApplicationBuffer(data);
    u32 attributes[NexUpdateSessionSetting::ATTRIBUTE_NUM];
    for (u32 i = 0; i < NexUpdateSessionSetting::ATTRIBUTE_NUM; i++) {
        attributes[i] = pSetting->GetAttribute(i);
    }
    m_pUpdateParam->m_Attributes.assign(attributes, attributes + NexUpdateSessionSetting::ATTRIBUTE_NUM);
    if (pSetting->IsAnyParamSet()) {
        nex::MatchmakeParam param;
        if (pSetting->IsParamRVSet()) {
            param.SetParam(nex::String(nex::g_MatchmakeParamKeyRV), nex::Variant(pSetting->m_ParamRV));
        }
        if (pSetting->IsParamDRSet()) {
            param.SetParam(nex::String(nex::g_MatchmakeParamKeyDR), nex::Variant(pSetting->m_ParamDR));
        }
        if (pSetting->IsParamVRSet()) {
            param.SetParam(nex::String(nex::g_MatchmakeParamKeyVR), nex::Variant(pSetting->m_ParamVR));
        }
        if (pSetting->IsParamNCCSet()) {
            param.SetParam(nex::String(nex::g_MatchmakeParamKeyNCC), nex::Variant(pSetting->m_ParamNCC));
        }
        if (pSetting->GetParamUpGI()) {
            param.SetParam(nex::String(nex::g_MatchmakeParamKeyUpGI), nex::Variant(pSetting->GetParamUpGI()));
        }
        m_pUpdateParam->m_MatchmakeParam = param;
    }
}

// 0x003FA218 slot 0x6C
nn::Result nn::pia::inet::NexMatchmakeSession::UpdateSessionSettingAsync(nn::pia::common::CallContext* pCallContext, u32 sessionId)
{
    if (!common::IsValidPointer(pCallContext) || pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_pCallContext != nullptr || m_NexCallContext.m_State == nex::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_STATE;
    }
    m_pCallContext = pCallContext;
    m_pUpdateParam->m_GatheringId = sessionId;
    if (m_NexCallContext.m_State == nex::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_STATE;
    }
    if (!m_pClient->UpdateMatchmakeSession(&m_NexCallContext, *m_pUpdateParam)) {
        m_pCallContext = nullptr;
        return common::RESULT_UNREGISTER_FAILED;
    }
    m_pCallContext->InitiateCall();
    return nn::Result();
}

// 0x003FA2B8 slot 0x4C
bool nn::pia::inet::NexMatchmakeSession::IsCloseParticipationCompleted()
{
    switch (m_NexCallContext.m_State) {
    case nex::CallContext::STATE_FAILURE: {
        nex::qResult result = m_NexCallContext.m_Result;
        if (result.m_Code == NEX_RESULT_80030073) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_SESSION_GONE);
        } else if (result.m_Code == NEX_RESULT_800300D9) {
            m_pCallContext->SignalFailure(common::RESULT_INVALID_STATE);
        } else {
            s_NetworkErrorCode = nex::ErrorCodeConverter::ConvertToNetworkErrorCode(result);
            m_pCallContext->SignalFailure(common::RESULT_FATAL_196);
        }
        m_pCallContext = nullptr;
        m_NexCallContext.Reset();
        return true;
    }
    case nex::CallContext::STATE_SUCCESS:
        m_pCallContext->SignalSuccess(nn::Result());
        m_pCallContext = nullptr;
        m_NexCallContext.Reset();
        m_Unknown0x4 = false;
        common::g_SessionStateMonitoringContent.m_Unknown0x3C9 = 0;
        return true;
    default:
        return false;
    }
}

// 0x003FA3B4 slot 0x30
nn::Result nn::pia::inet::NexMatchmakeSession::LeaveAsync(nn::pia::common::CallContext* pCallContext, u32 sessionId)
{
    nn::Result result;
    if (!common::IsValidPointer(pCallContext) || pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        result = common::RESULT_INVALID_ARGUMENT;
    } else if (m_pCallContext != nullptr || m_NexCallContext.m_State == nex::CallContext::STATE_CALL_IN_PROGRESS) {
        result = common::RESULT_INVALID_STATE;
    } else {
        m_pCallContext = pCallContext;
        m_SessionId = sessionId;
        if (!m_pClient->EndParticipation(&m_NexCallContext, m_SessionId, nex::String(L""))) {
            result = common::RESULT_UNREGISTER_FAILED;
            m_pCallContext = nullptr;
        } else {
            m_Deadline = GetTimeAfter(CALL_TIMEOUT_MSEC);
            m_pCallContext->InitiateCall();
        }
    }
    if (result.IsFailure()) {
        RemoveSessionEntry(m_SessionId);
    }
    return result;
}

// 0x003FA4E8 slot 0x14
nn::Result nn::pia::inet::NexMatchmakeSession::BrowseAsync(nn::pia::common::CallContext* pCallContext)
{
    if (!common::IsValidPointer(pCallContext)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_pSearchCriteria->GetSize() == 0 || m_pResultRanges->GetSize() == 0) {
        return common::RESULT_INVALID_STATE;
    }
    if (pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_pCallContext != nullptr || m_NexCallContext.m_State == nex::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_STATE;
    }
    m_pCallContext = pCallContext;
    if (!m_pClient->BrowseMatchmakeSession(&m_NexCallContext, m_pSearchCriteria->front(), m_pResultRanges->front(), m_pGatherings)) {
        m_pCallContext = nullptr;
        return common::RESULT_INVALID_STATE;
    }
    m_pCallContext->InitiateCall();
    m_pSessionInfoList->Clear();
    return nn::Result();
}

// 0x003FA5D0 slot 0x20
nn::Result nn::pia::inet::NexMatchmakeSession::CreateAsync(nn::pia::common::CallContext* pCallContext)
{
    if (!common::IsValidPointer(pCallContext) || pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_pCallContext != nullptr || m_NexCallContext.m_State == nex::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_STATE;
    }
    m_pCallContext = pCallContext;
    nex::CreateMatchmakeSessionParam param;
    param.m_SourceMatchmakeSession = *m_pSourceSession;
    param.m_JoinMessage = m_pSourceSession->m_Description;
    if (!m_pClient->CreateMatchmakeSession(&m_NexCallContext, param, m_pMatchmakeSession)) {
        m_pCallContext = nullptr;
        return common::RESULT_UNREGISTER_FAILED;
    }
    m_pCallContext->InitiateCall();
    return nn::Result();
}

// 0x003FA6AC slot 0xBC
nn::Result nn::pia::inet::NexMatchmakeSession::GenerateSystemPasswordAsync(nn::pia::common::CallContext* pCallContext, u32 sessionId)
{
    if (!common::IsValidPointer(pCallContext) || pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_pCallContext != nullptr) {
        return common::RESULT_INVALID_STATE;
    }
    m_pCallContext = pCallContext;
    if (!m_pClient->GenerateMatchmakeSessionSystemPassword(&m_NexCallContext, sessionId, m_pSystemPassword)) {
        m_pCallContext = nullptr;
        return common::RESULT_UNREGISTER_FAILED;
    }
    m_pCallContext->InitiateCall();
    return nn::Result();
}

// 0x003FA72C (name is ours)
bool nn::pia::inet::NexMatchmakeSession::SetSearchCriteria(const NexSessionSearchCriteria* pCriteria, u32 criteriaNum)
{
    bool isValid = true;
    u8 type = pCriteria->m_Unknown0x4;
    m_MatchmakeSystemType = type;
    if (type != 0 && type != 2) {
        return false;
    }
    for (u32 i = 0; i < criteriaNum; i++) {
        if (pCriteria[i].m_Unknown0x4 != type) {
            return false;
        }
    }
    for (u32 i = 0; i < criteriaNum; i++) {
        nex::MatchmakeSessionSearchCriteria criteria;
        nex::MatchmakeParam param;
        nex::ResultRange range(0, 20);
        pCriteria->ConvertTo(&criteria, &param, &range, i);
        m_pSearchCriteria->push_back(criteria);
        m_pResultRanges->push_back(range);
        pCriteria++;
    }
    return isValid;
}

// 0x003FAD68 slot 0xD0
bool nn::pia::inet::NexMatchmakeSession::IsFindSessionByOwnerCompleted()
{
    switch (m_NexCallContext.m_State) {
    case nex::CallContext::STATE_FAILURE: {
        nex::qResult result = m_NexCallContext.m_Result;
        if (result.m_Code == NEX_RESULT_INVALID_ARGUMENT) {
            m_pCallContext->SignalFailure(common::RESULT_INVALID_ARGUMENT);
        } else if (result.m_Code == NEX_RESULT_800300CF) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_247);
        } else {
            s_NetworkErrorCode = nex::ErrorCodeConverter::ConvertToNetworkErrorCode(result);
            m_pCallContext->SignalFailure(common::RESULT_FATAL_196);
        }
        break;
    }
    case nex::CallContext::STATE_SUCCESS: {
        for (nex::qList<GatheringHolder>::Node* pNode = m_pGatherings->GetFirst(); !m_pGatherings->IsEnd(pNode); pNode = pNode->m_pNext) {
            if (!pNode->m_Value.Get()->IsA(nex::String("MatchmakeSession"))) {
                continue;
            }
            const nex::MatchmakeSession* pSession = static_cast<const nex::MatchmakeSession*>(pNode->m_Value.Get());
            NexSessionInfo* pInfo = static_cast<NexSessionInfoList*>(m_pSessionInfoList)->Add();
            if (pInfo == nullptr) {
                break;
            }
            nex::qVector<u8> buffer(pSession->m_ApplicationBuffer.begin(), pSession->m_ApplicationBuffer.end());
            SetSessionInfo(pInfo, pSession, buffer);
            nex::String description(pSession->m_Description);
            pInfo->vf_0x78(reinterpret_cast<const u16*>(description.m_pString), description.GetLength());
            pInfo->vf_0x80(pSession->m_IsUserPasswordEnabled);
            pInfo->vf_0x84(pSession->m_IsSystemPasswordEnabled);
            pInfo->vf_0x88(ConvertNexMatchmakeSystemType(pSession->m_MatchmakeSystemType));
            pInfo->vf_0x8C(pSession->m_HostPrincipalId);
            pInfo->vf_0x90(pSession->m_OwnerPrincipalId);
            pInfo->vf_0x94(pSession->m_ProgressScore);
            pInfo->vf_0x98(common::DateTime(pSession->m_StartedTime.GetYear(), pSession->m_StartedTime.GetMonth(), pSession->m_StartedTime.GetDay(),
                                            pSession->m_StartedTime.GetHour(), pSession->m_StartedTime.GetMinute(),
                                            pSession->m_StartedTime.GetSecond()));
            pInfo->Trace(TRACE_FLAG);
        }
        m_pCallContext->SignalSuccess(nn::Result());
        break;
    }
    default:
        return false;
    }
    m_pCallContext = nullptr;
    m_pGatherings->clear();
    m_NexCallContext.Reset();
    return true;
}

// 0x003FB1DC slot 0xE8
bool nn::pia::inet::NexMatchmakeSession::IsGetJoinedSessionsCompleted(u32 numMax, u32* pSessionIds, u32* pNum)
{
    switch (m_NexCallContext.m_State) {
    case nex::CallContext::STATE_FAILURE: {
        nex::qResult result = m_NexCallContext.m_Result;
        if (result.m_Code == NEX_RESULT_INVALID_ARGUMENT) {
            m_pCallContext->SignalFailure(common::RESULT_INVALID_ARGUMENT);
        } else {
            s_NetworkErrorCode = nex::ErrorCodeConverter::ConvertToNetworkErrorCode(result);
            m_pCallContext->SignalFailure(common::RESULT_FATAL_196);
        }
        break;
    }
    case nex::CallContext::STATE_SUCCESS:
        if (m_pPlayingSessions->GetSize() > numMax) {
            m_pCallContext->SignalFailure(common::RESULT_BUFFER_SHORTAGE);
        } else {
            u32 num = 0;
            for (nex::qList<nex::PlayingSession>::Node* pNode = m_pPlayingSessions->GetFirst(); !m_pPlayingSessions->IsEnd(pNode); pNode = pNode->m_pNext) {
                pSessionIds[num] = pNode->m_Value.GetMatchmakeSession()->m_Id;
                num++;
            }
            *pNum = num;
            m_pCallContext->SignalSuccess(nn::Result());
        }
        break;
    default:
        return false;
    }
    m_pCallContext = nullptr;
    m_pPlayingSessions->clear();
    m_pPrincipalIds->clear();
    m_NexCallContext.Reset();
    return true;
}

// 0x003FB32C slot 0xF4
nn::Result nn::pia::inet::NexMatchmakeSession::UpdateApplicationBufferAsync(nn::pia::common::CallContext* pCallContext, u32 sessionId, const nn::nex::qVector<u8>& data)
{
    if (!common::IsValidPointer(pCallContext) || pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_pCallContext != nullptr || m_NexCallContext.m_State == nex::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_STATE;
    }
    m_pCallContext = pCallContext;
    bool isFailed;
    {
        nex::qVector<u8> buffer(data);
        isFailed = !m_pClient->UpdateApplicationBuffer(&m_NexCallContext, sessionId, buffer);
    }
    if (isFailed) {
        m_pCallContext = nullptr;
        return common::RESULT_UNREGISTER_FAILED;
    }
    m_pCallContext->InitiateCall();
    return nn::Result();
}

// 0x003FB454 slot 0x98
nn::Result nn::pia::inet::NexMatchmakeSession::UpdateProgressScore(u32 sessionId, u8 score)
{
    if (score > 100) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (!IsProgressScoreUpdatable(score)) {
        return common::RESULT_INVALID_STATE_TEMPORARY;
    }
    m_ProgressScoreUpdateTime.SetNow();
    if (!m_pClient->UpdateProgressScore(sessionId, score)) {
        return common::RESULT_UNREGISTER_FAILED;
    }
    IncrementCount(&common::g_SessionStateMonitoringContent.m_Unknown0x3CC);
    return nn::Result();
}

// 0x003FB4E4 slot 0xC8
bool nn::pia::inet::NexMatchmakeSession::IsClearSystemPasswordCompleted()
{
    switch (m_NexCallContext.m_State) {
    case nex::CallContext::STATE_FAILURE: {
        nex::qResult result = m_NexCallContext.m_Result;
        if (result.m_Code == NEX_RESULT_80030073) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_SESSION_GONE);
        } else if (result.m_Code == NEX_RESULT_800300D9) {
            m_pCallContext->SignalFailure(common::RESULT_INVALID_STATE);
        } else {
            s_NetworkErrorCode = nex::ErrorCodeConverter::ConvertToNetworkErrorCode(result);
            m_pCallContext->SignalFailure(common::RESULT_FATAL_196);
        }
        break;
    }
    case nex::CallContext::STATE_SUCCESS:
        m_pCallContext->SignalSuccess(nn::Result());
        break;
    default:
        return false;
    }
    m_pCallContext = nullptr;
    m_NexCallContext.Reset();
    return true;
}

// 0x003FB5BC slot 0xD4
nn::Result nn::pia::inet::NexMatchmakeSession::GetOwnerAsync(nn::pia::common::CallContext* pCallContext, u32 sessionId)
{
    if (!common::IsValidPointer(pCallContext) || pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_pCallContext != nullptr || m_NexCallContext.m_State == nex::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_STATE;
    }
    m_pCallContext = pCallContext;
    if (!m_pClient->FindBySingleID(&m_NexCallContext, sessionId, &m_Gathering)) {
        m_pCallContext = nullptr;
        return common::RESULT_UNREGISTER_FAILED;
    }
    m_Deadline = GetTimeAfter(CALL_TIMEOUT_MSEC);
    m_pCallContext->InitiateCall();
    return nn::Result();
}

// 0x003FB68C slot 0x50
nn::Result nn::pia::inet::NexMatchmakeSession::vf_0x50(nn::pia::common::CallContext* pCallContext, u32 sessionId)
{
    if (!common::IsValidPointer(pCallContext) || pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_pCallContext != nullptr) {
        return common::RESULT_INVALID_STATE;
    }
    m_pCallContext = pCallContext;
    if (m_NexCallContext.m_State == nex::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_STATE;
    }
    if (!m_pClient->GetSessionURLs(&m_NexCallContext, sessionId, m_pSessionUrls)) {
        m_pCallContext = nullptr;
        return common::RESULT_UNREGISTER_FAILED;
    }
    m_pCallContext->InitiateCall();
    return nn::Result();
}

// 0x003FB718 (name is ours)
void nn::pia::inet::NexMatchmakeSession::FinishBrowse()
{
    m_pSearchCriteria->clear();
    m_pResultRanges->clear();
}

// 0x003FB734 slot 0x10
bool nn::pia::inet::NexMatchmakeSession::IsAutoMatchmakeCompleted(u32* pSessionId, bool* pIsCreator, u32* pJointSessionId, void* pApplicationData,
                                                                  u32* pApplicationDataSize)
{
    switch (m_NexCallContext.m_State) {
    case nex::CallContext::STATE_FAILURE: {
        nex::qResult result = m_NexCallContext.m_Result;
        if (result.m_Code == NEX_RESULT_INVALID_ARGUMENT) {
            m_pCallContext->SignalFailure(common::RESULT_INVALID_ARGUMENT);
        } else if (result.m_Code == NEX_RESULT_80030073) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_SESSION_GONE);
        } else if (result.m_Code == NEX_RESULT_800300C8) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_244);
        } else if (result.m_Code == NEX_RESULT_800300CD) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_245);
        } else if (result.m_Code == NEX_RESULT_800300CE) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_246);
        } else if (result.m_Code == NEX_RESULT_800300CF) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_247);
        } else if (result.m_Code == NEX_RESULT_800300D1) {
            m_pCallContext->SignalFailure(common::RESULT_INVALID_STATE);
        } else if (result.m_Code == NEX_RESULT_800300DB) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_251);
        } else if (result.m_Code == NEX_RESULT_800300DA) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_253);
        } else if (result.m_Code == NEX_RESULT_800300D4 || result.m_Code == NEX_RESULT_800300D8) {
            m_pCallContext->SignalFailure(common::RESULT_INVALID_STATE);
        } else if (result.m_Code == NEX_RESULT_800300D7) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_252);
        } else {
            s_NetworkErrorCode = nex::ErrorCodeConverter::ConvertToNetworkErrorCode(result);
            m_pCallContext->SignalFailure(common::RESULT_FATAL_196);
        }
        m_NexCallContext.Reset();
        m_pCallContext = nullptr;
        return true;
    }
    case nex::CallContext::STATE_SUCCESS:
        break;
    default:
        return false;
    }

    nex::MatchmakeSession* pSession;
    if (m_MatchmakeSystemType == 0) {
        pSession = m_pMatchmakeSession;
    } else if (m_MatchmakeSystemType == 2) {
        pSession = static_cast<nex::MatchmakeSession*>(m_Gathering.Get());
    }
    nn::Result result = NexFacade::ConvertNexSessionKeyToSignatureSetting(pSession->m_SessionKey, &m_SignatureSetting);
    if (result.IsFailure()) {
        m_pCallContext->SignalFailure(result);
        m_pCallContext = nullptr;
        m_NexCallContext.Reset();
    } else {
        session::Session* pPiaSession = session::Session::s_pInstance;
        m_IsHost = pPiaSession->m_Unknown0x8C == pSession->m_HostPrincipalId;
        m_Unknown0x3C = pPiaSession->m_Unknown0x8C == pSession->m_OwnerPrincipalId;
        if (m_IsHost ? !m_Unknown0x3C : m_Unknown0x3C) {
            m_pCallContext->SignalFailure(common::RESULT_HOST_OWNER_MISMATCH);
            m_pCallContext = nullptr;
            m_NexCallContext.Reset();
        } else {
            m_Unknown0x38 = pSession->m_OwnerPrincipalId;
            *pIsCreator = m_Unknown0x3C;
            m_SessionId = pSession->m_Id;
            *pSessionId = m_SessionId;
            m_Unknown0x4 = pSession->m_IsOpenParticipation;
            if (pApplicationData != nullptr && pApplicationDataSize != nullptr) {
                nex::qVector<u8> buffer;
                buffer.assign(pSession->m_ApplicationBuffer.begin(), pSession->m_ApplicationBuffer.end());
                std::memcpy(pApplicationData, buffer.begin(), buffer.end() - buffer.begin());
                *pApplicationDataSize = buffer.end() - buffer.begin();
            }
            if (pJointSessionId != nullptr && session::Session::s_pInstance->m_pMeshLayerController->vf_0x34()) {
                if (!pSession->m_MatchmakeParam.GetParamLGFPC(pJointSessionId)) {
                    *pJointSessionId = 0;
                }
            }
            m_pCallContext->SignalSuccess(nn::Result());
            m_pCallContext = nullptr;
            m_NexCallContext.Reset();
        }
    }
    if (m_MatchmakeSystemType == 2) {
        m_SourceGathering.m_pObject = nullptr;
        m_Gathering.m_pObject = nullptr;
    }
    return true;
}

// 0x003FBB5C slot 0x2C
bool nn::pia::inet::NexMatchmakeSession::IsJoinCompleted(u32* pJointSessionId, void* pApplicationData, u32* pApplicationDataSize)
{
    switch (m_NexCallContext.m_State) {
    case nex::CallContext::STATE_FAILURE: {
        nex::qResult result = m_NexCallContext.m_Result;
        if (result.m_Code == NEX_RESULT_INVALID_ARGUMENT) {
            m_pCallContext->SignalFailure(common::RESULT_INVALID_ARGUMENT);
        } else if (result.m_Code == NEX_RESULT_80030073) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_SESSION_GONE);
        } else if (result.m_Code == NEX_RESULT_800300C8) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_244);
        } else if (result.m_Code == NEX_RESULT_800300CD) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_245);
        } else if (result.m_Code == NEX_RESULT_800300CE) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_246);
        } else if (result.m_Code == NEX_RESULT_800300CF) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_247);
        } else if (result.m_Code == NEX_RESULT_800300D5) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_249);
        } else if (result.m_Code == NEX_RESULT_800300D6) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_250);
        } else if (result.m_Code == NEX_RESULT_800300DB) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_251);
        } else if (result.m_Code == NEX_RESULT_800300DA) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_253);
        } else if (result.m_Code == NEX_RESULT_800300D4 || result.m_Code == NEX_RESULT_800300D8) {
            m_pCallContext->SignalFailure(common::RESULT_INVALID_STATE);
        } else if (result.m_Code == NEX_RESULT_800300D7) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_252);
        } else {
            s_NetworkErrorCode = nex::ErrorCodeConverter::ConvertToNetworkErrorCode(result);
            m_pCallContext->SignalFailure(common::RESULT_FATAL_196);
        }
        break;
    }
    case nex::CallContext::STATE_SUCCESS: {
        m_SessionId = m_pMatchmakeSession->m_Id;
        if (NexFacade::ConvertNexSessionKeyToSignatureSetting(m_pMatchmakeSession->m_SessionKey, &m_SignatureSetting).IsFailure()) {
            m_pCallContext->SignalFailure(common::RESULT_INVALID_STATE);
            break;
        }
        nex::MatchmakeSession* pSession = m_pMatchmakeSession;
        m_IsHost = session::Session::s_pInstance->m_Unknown0x8C == pSession->m_HostPrincipalId;
        m_Unknown0x3C = session::Session::s_pInstance->m_Unknown0x8C == pSession->m_OwnerPrincipalId;
        if (m_IsHost || m_Unknown0x3C) {
            m_pCallContext->SignalFailure(common::RESULT_HOST_OWNER_MISMATCH);
            break;
        }
        m_Unknown0x38 = pSession->m_OwnerPrincipalId;
        m_Unknown0x4 = pSession->m_IsOpenParticipation;
        if (pApplicationData != nullptr && pApplicationDataSize != nullptr) {
            nex::qVector<u8> buffer;
            buffer.assign(m_pMatchmakeSession->m_ApplicationBuffer.begin(), m_pMatchmakeSession->m_ApplicationBuffer.end());
            std::memcpy(pApplicationData, buffer.begin(), buffer.end() - buffer.begin());
            *pApplicationDataSize = buffer.end() - buffer.begin();
        }
        if (pJointSessionId != nullptr && session::Session::s_pInstance->m_pMeshLayerController->vf_0x34()) {
            if (!m_pMatchmakeSession->m_MatchmakeParam.GetParamLGFPC(pJointSessionId)) {
                *pJointSessionId = 0;
            }
        }
        m_pCallContext->SignalSuccess(nn::Result());
        break;
    }
    default:
        return false;
    }
    m_pCallContext = nullptr;
    m_NexCallContext.Reset();
    return true;
}

// 0x003FBF44 slot 0x70
bool nn::pia::inet::NexMatchmakeSession::IsUpdateSessionSettingCompleted()
{
    switch (m_NexCallContext.m_State) {
    case nex::CallContext::STATE_FAILURE: {
        nex::qResult result = m_NexCallContext.m_Result;
        if (result.m_Code == NEX_RESULT_INVALID_ARGUMENT) {
            m_pCallContext->SignalFailure(common::RESULT_INVALID_ARGUMENT);
        } else if (result.m_Code == NEX_RESULT_80030073) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_SESSION_GONE);
        } else if (result.m_Code == NEX_RESULT_800300D9) {
            m_pCallContext->SignalFailure(common::RESULT_INVALID_STATE);
        } else {
            s_NetworkErrorCode = nex::ErrorCodeConverter::ConvertToNetworkErrorCode(result);
            m_pCallContext->SignalFailure(common::RESULT_FATAL_196);
        }
        m_pUpdateParam->Reset();
        m_NexCallContext.Reset();
        m_pCallContext = nullptr;
        return true;
    }
    case nex::CallContext::STATE_SUCCESS:
        m_pCallContext->SignalSuccess(nn::Result());
        m_pCallContext = nullptr;
        m_pUpdateParam->Reset();
        m_NexCallContext.Reset();
        return true;
    default:
        return false;
    }
}

// 0x003FC080 slot 0x34
bool nn::pia::inet::NexMatchmakeSession::IsLeaveCompleted()
{
    switch (m_NexCallContext.m_State) {
    case nex::CallContext::STATE_FAILURE: {
        nex::qResult result = m_NexCallContext.m_Result;
        if (result.m_Code == NEX_RESULT_INVALID_ARGUMENT) {
            m_pCallContext->SignalFailure(common::RESULT_INVALID_ARGUMENT);
        } else if (result.m_Code == NEX_RESULT_80030073) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_SESSION_GONE);
        } else if (result.m_Code == NEX_RESULT_800300D9) {
            m_pCallContext->SignalFailure(common::RESULT_INVALID_STATE);
        } else {
            s_NetworkErrorCode = nex::ErrorCodeConverter::ConvertToNetworkErrorCode(result);
            m_pCallContext->SignalFailure(common::RESULT_FATAL_196);
        }
        m_NexCallContext.Reset();
        m_pMatchmakeSession->Reset();
        m_pCallContext = nullptr;
        break;
    }
    case nex::CallContext::STATE_SUCCESS:
        m_IsHost = false;
        m_Unknown0x3C = false;
        m_Unknown0x38 = 0;
        m_NexCallContext.Reset();
        m_pMatchmakeSession->Reset();
        m_pCallContext->SignalSuccess(nn::Result());
        m_pCallContext = nullptr;
        break;
    default:
        if (!IsExpired(m_Deadline)) {
            return false;
        }
        m_NexCallContext.Cancel(nex::CallContext::STATE_CANCELLED);
        m_pCallContext->SignalFailure(common::RESULT_UNREGISTER_FAILED);
        m_NexCallContext.Reset();
        m_pMatchmakeSession->Reset();
        m_pCallContext = nullptr;
        break;
    }
    RemoveSessionEntry(m_SessionId);
    m_SessionId = 0;
    return true;
}

// 0x003FC25C slot 0x38
nn::Result nn::pia::inet::NexMatchmakeSession::UnregisterAsync(nn::pia::common::CallContext* pCallContext, u32 sessionId)
{
    nn::Result result;
    if (!common::IsValidPointer(pCallContext) || pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        result = common::RESULT_INVALID_ARGUMENT;
    } else if (m_pCallContext != nullptr || m_NexCallContext.m_State == nex::CallContext::STATE_CALL_IN_PROGRESS) {
        result = common::RESULT_INVALID_STATE;
    } else {
        m_pCallContext = pCallContext;
        m_SessionId = sessionId;
        if (!m_pClient->UnregisterGathering(&m_NexCallContext, sessionId)) {
            result = common::RESULT_UNREGISTER_FAILED;
            m_pCallContext = nullptr;
        } else {
            m_Deadline = GetTimeAfter(CALL_TIMEOUT_MSEC);
            m_pCallContext->InitiateCall();
        }
    }
    if (result.IsFailure()) {
        RemoveSessionEntry(sessionId);
    }
    return result;
}

// 0x003FC364 slot 0x74
nn::Result nn::pia::inet::NexMatchmakeSession::vf_0x74(u32 sessionId)
{
    if (sessionId == 0) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    nex::ProtocolCallContext callContext;
    if (!m_pClient->UpdateSessionHost(&callContext, sessionId, true)) {
        return common::RESULT_INVALID_STATE;
    }
    return nn::Result();
}

// 0x003FC3E0 (name is ours)
nn::Result nn::pia::inet::NexMatchmakeSession::GetSessionStatusAsync(nn::pia::common::CallContext* pCallContext, u32 sessionId)
{
    if (!common::IsValidPointer(pCallContext) || pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_pCallContext != nullptr || m_NexCallContext.m_State == nex::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_STATE;
    }
    if (m_SessionId != sessionId) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    m_pCallContext = pCallContext;
    if (!m_pClient->GetMatchmakeSession(&m_NexCallContext, m_SessionId, m_pMatchmakeSession)) {
        m_pCallContext = nullptr;
        return common::RESULT_UNREGISTER_FAILED;
    }
    m_Deadline = GetTimeAfter(CALL_TIMEOUT_MSEC);
    m_pCallContext->InitiateCall();
    return nn::Result();
}

// 0x003FC4C8 slot 0x18
bool nn::pia::inet::NexMatchmakeSession::IsBrowseCompleted()
{
    switch (m_NexCallContext.m_State) {
    case nex::CallContext::STATE_FAILURE: {
        nex::qResult result = m_NexCallContext.m_Result;
        if (result.m_Code == NEX_RESULT_INVALID_ARGUMENT) {
            m_pCallContext->SignalFailure(common::RESULT_INVALID_ARGUMENT);
        } else if (result.m_Code == NEX_RESULT_800300CF) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_247);
        } else {
            s_NetworkErrorCode = nex::ErrorCodeConverter::ConvertToNetworkErrorCode(result);
            m_pCallContext->SignalFailure(common::RESULT_FATAL_196);
        }
        break;
    }
    case nex::CallContext::STATE_SUCCESS: {
        for (nex::qList<GatheringHolder>::Node* pNode = m_pGatherings->GetFirst(); !m_pGatherings->IsEnd(pNode); pNode = pNode->m_pNext) {
            const nex::MatchmakeSession* pSession = static_cast<const nex::MatchmakeSession*>(pNode->m_Value.Get());
            nex::qVector<u8> buffer(pSession->m_ApplicationBuffer.begin(), pSession->m_ApplicationBuffer.end());
            NexSessionInfo* pInfo = static_cast<NexSessionInfoList*>(m_pSessionInfoList)->Add();
            if (pInfo == nullptr) {
                break;
            }
            SetSessionInfo(pInfo, pSession, buffer);
            nex::String description(pSession->m_Description);
            pInfo->vf_0x78(reinterpret_cast<const u16*>(description.m_pString), description.GetLength());
            pInfo->vf_0x80(pSession->m_IsUserPasswordEnabled);
            pInfo->vf_0x84(pSession->m_IsSystemPasswordEnabled);
            pInfo->vf_0x88(ConvertNexMatchmakeSystemType(pSession->m_MatchmakeSystemType));
            pInfo->vf_0x8C(pSession->m_HostPrincipalId);
            pInfo->vf_0x90(pSession->m_OwnerPrincipalId);
            pInfo->vf_0x94(pSession->m_ProgressScore);
            pInfo->vf_0x98(common::DateTime(pSession->m_StartedTime.GetYear(), pSession->m_StartedTime.GetMonth(), pSession->m_StartedTime.GetDay(),
                                            pSession->m_StartedTime.GetHour(), pSession->m_StartedTime.GetMinute(),
                                            pSession->m_StartedTime.GetSecond()));
            pInfo->Trace(TRACE_FLAG);
        }
        m_pCallContext->SignalSuccess(nn::Result());
        break;
    }
    default:
        return false;
    }
    m_pCallContext = nullptr;
    m_pGatherings->clear();
    m_NexCallContext.Reset();
    return true;
}

// 0x003FC8F0 slot 0x24
bool nn::pia::inet::NexMatchmakeSession::IsCreateCompleted(u32* pSessionId)
{
    switch (m_NexCallContext.m_State) {
    case nex::CallContext::STATE_FAILURE: {
        nex::qResult result = m_NexCallContext.m_Result;
        if (result.m_Code == NEX_RESULT_INVALID_ARGUMENT) {
            m_pCallContext->SignalFailure(common::RESULT_INVALID_ARGUMENT);
        } else if (result.m_Code == NEX_RESULT_800300CF) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_247);
        } else if (result.m_Code == NEX_RESULT_800300D7 || result.m_Code == NEX_RESULT_800300D4) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_252);
        } else {
            s_NetworkErrorCode = nex::ErrorCodeConverter::ConvertToNetworkErrorCode(result);
            m_pCallContext->SignalFailure(common::RESULT_FATAL_196);
        }
        break;
    }
    case nex::CallContext::STATE_SUCCESS: {
        nn::Result result = NexFacade::ConvertNexSessionKeyToSignatureSetting(m_pMatchmakeSession->m_SessionKey, &m_SignatureSetting);
        if (result.IsFailure()) {
            m_pCallContext->SignalFailure(result);
            m_pCallContext = nullptr;
            return true;
        }
        m_SessionId = m_pMatchmakeSession->m_Id;
        *pSessionId = m_SessionId;
        nex::MatchmakeSession* pSession = m_pMatchmakeSession;
        m_IsHost = session::Session::s_pInstance->m_Unknown0x8C == pSession->m_HostPrincipalId;
        m_Unknown0x3C = session::Session::s_pInstance->m_Unknown0x8C == pSession->m_OwnerPrincipalId;
        if (!m_IsHost || !m_Unknown0x3C) {
            m_pCallContext->SignalFailure(common::RESULT_HOST_OWNER_MISMATCH);
            break;
        }
        m_Unknown0x38 = pSession->m_OwnerPrincipalId;
        m_Unknown0x4 = pSession->m_IsOpenParticipation;
        m_pCallContext->SignalSuccess(nn::Result());
        m_pCallContext = nullptr;
        m_NexCallContext.Reset();
        return true;
    }
    default:
        return false;
    }
    m_pCallContext = nullptr;
    m_NexCallContext.Reset();
    return true;
}

// 0x003FCAD4 slot 0xC0
bool nn::pia::inet::NexMatchmakeSession::IsGenerateSystemPasswordCompleted(wchar_t* pPassword)
{
    switch (m_NexCallContext.m_State) {
    case nex::CallContext::STATE_FAILURE: {
        nex::qResult result = m_NexCallContext.m_Result;
        if (result.m_Code == NEX_RESULT_80030073) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_SESSION_GONE);
        } else if (result.m_Code == NEX_RESULT_800300D9) {
            m_pCallContext->SignalFailure(common::RESULT_INVALID_STATE);
        } else {
            s_NetworkErrorCode = nex::ErrorCodeConverter::ConvertToNetworkErrorCode(result);
            m_pCallContext->SignalFailure(common::RESULT_FATAL_196);
        }
        break;
    }
    case nex::CallContext::STATE_SUCCESS: {
        const wchar_t* pSource = m_pSystemPassword->m_pString;
        for (u32 i = 0; i < m_pSystemPassword->GetLength(); i++) {
            *pPassword++ = *pSource++;
        }
        m_pCallContext->SignalSuccess(nn::Result());
        break;
    }
    default:
        return false;
    }
    m_pCallContext = nullptr;
    m_NexCallContext.Reset();
    return true;
}

// 0x003FCBF0 slot 0x7C
bool nn::pia::inet::NexMatchmakeSession::vf_0x7C()
{
    nex::MatchmakeSession* pSession;
    bool isCompleted = IsGetMatchmakeSessionCompleted(&pSession);
    if (isCompleted && pSession->m_IsOpenParticipation) {
        m_Unknown0x4 = true;
    }
    return isCompleted;
}

// 0x003FCC20 slot 0xF8
bool nn::pia::inet::NexMatchmakeSession::IsUpdateApplicationBufferCompleted()
{
    switch (m_NexCallContext.m_State) {
    case nex::CallContext::STATE_FAILURE: {
        nex::qResult result = m_NexCallContext.m_Result;
        if (result.m_Code == NEX_RESULT_80030073) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_SESSION_GONE);
        } else if (result.m_Code == NEX_RESULT_INVALID_ARGUMENT) {
            m_pCallContext->SignalFailure(common::RESULT_INVALID_ARGUMENT);
        } else if (result.m_Code == NEX_RESULT_800300CF) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_247);
        } else if (result.m_Code == NEX_RESULT_800300D9) {
            m_pCallContext->SignalFailure(common::RESULT_INVALID_STATE);
        } else {
            s_NetworkErrorCode = nex::ErrorCodeConverter::ConvertToNetworkErrorCode(result);
            m_pCallContext->SignalFailure(common::RESULT_FATAL_196);
        }
        break;
    }
    case nex::CallContext::STATE_SUCCESS:
        m_pCallContext->SignalSuccess(nn::Result());
        break;
    default:
        return false;
    }
    m_pCallContext = nullptr;
    m_NexCallContext.Reset();
    return true;
}

// 0x003FCD50 slot 0xA4 (name is ours)
nn::Result nn::pia::inet::NexMatchmakeSession::AutoMatchmakeWithParticipantsAsync(nn::pia::common::CallContext* pCallContext, const u32* pPrincipalIds,
                                                                                    u32 principalNum, u32 gatheringIdForParticipationCheck,
                                                                                    bool isOptionSet)
{
    if (!common::IsValidPointer(pCallContext) || pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_pCallContext != nullptr || m_NexCallContext.m_State == nex::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_STATE;
    }
    m_pCallContext = pCallContext;
    bool isCalled;
    {
        nex::AutoMatchmakeParam param;
        param.SetSearchCriteria(*m_pSearchCriteria);
        param.m_SourceMatchmakeSession = *m_pSourceSession;
        param.m_JoinMessage = m_pSourceSession->m_Description;
        nex::qList<u32> participants;
        for (u32 i = 0; i < principalNum; i++) {
            participants.push_back(pPrincipalIds[i]);
        }
        param.m_AdditionalParticipants = participants;
        param.m_GatheringIdForParticipationCheck = gatheringIdForParticipationCheck;
        if (isOptionSet) {
            param.m_AutoMatchmakeOption = 1;
        }
        isCalled = m_pClient->AutoMatchmake(&m_NexCallContext, param, m_pMatchmakeSession);
    }
    if (!isCalled) {
        m_pCallContext = nullptr;
        return common::RESULT_UNREGISTER_FAILED;
    }
    m_pCallContext->InitiateCall();
    return nn::Result();
}

// 0x003FCF4C slot 0x9C
bool nn::pia::inet::NexMatchmakeSession::IsProgressScoreUpdatable(u8 score)
{
    if (score == 0 || score == 100) {
        return true;
    }
    if (m_ProgressScoreUpdateTime.m_Tick == 0) {
        return true;
    }
    common::Time now;
    now.SetNow();
    u32 elapsedSec = (now - m_ProgressScoreUpdateTime).GetTick() / common::TimeSpan::GetTicksPerSec().GetTick();
    return elapsedSec > PROGRESS_SCORE_INTERVAL_SEC;
}

// 0x003FCFC8 slot 0xB4 (name is ours)
nn::Result nn::pia::inet::NexMatchmakeSession::JoinWithParticipantsAsync(nn::pia::common::CallContext* pCallContext, u32 sessionId, const u32* pPrincipalIds,
                                                                           u32 principalNum, u32 gatheringIdForParticipationCheck, bool isOptionSet)
{
    if (!common::IsValidPointer(pCallContext) || pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_pCallContext != nullptr || m_NexCallContext.m_State == nex::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_STATE;
    }
    m_pCallContext = pCallContext;
    m_pJoinParam->m_GatheringId = sessionId;
    m_pJoinParam->m_JoinMessage = m_pSourceSession->m_Description;
    nex::qList<u32> participants;
    for (u32 i = 0; i < principalNum; i++) {
        participants.push_back(pPrincipalIds[i]);
    }
    m_pJoinParam->m_AdditionalParticipants = participants;
    m_pJoinParam->m_GatheringIdForParticipationCheck = gatheringIdForParticipationCheck;
    if (isOptionSet) {
        m_pJoinParam->m_JoinMatchmakeSessionOption = 1;
    }
    if (!m_pClient->JoinMatchmakeSession(&m_NexCallContext, *m_pJoinParam, m_pMatchmakeSession)) {
        m_pCallContext = nullptr;
        return common::RESULT_UNREGISTER_FAILED;
    }
    m_pCallContext->InitiateCall();
    return nn::Result();
}

// 0x003FD1D0 slot 0xDC
nn::Result nn::pia::inet::NexMatchmakeSession::MigrateOwnerAsync(nn::pia::common::CallContext* pCallContext, u32 sessionId, const u32* pPrincipalIds,
                                                                   u32 principalNum)
{
    if (!common::IsValidPointer(pCallContext) || pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_pCallContext != nullptr || m_NexCallContext.m_State == nex::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_STATE;
    }
    m_pCallContext = pCallContext;
    for (u32 i = 0; i < principalNum; i++) {
        m_pPrincipalIds->push_back(pPrincipalIds[i]);
    }
    if (!m_pClient->MigrateGatheringOwnership(&m_NexCallContext, sessionId, *m_pPrincipalIds)) {
        m_pCallContext = nullptr;
        return common::RESULT_UNREGISTER_FAILED;
    }
    m_Deadline = GetTimeAfter(CALL_TIMEOUT_MSEC);
    m_pCallContext->InitiateCall();
    return nn::Result();
}

// 0x003FD3AC slot 0xD8
bool nn::pia::inet::NexMatchmakeSession::IsGetOwnerCompleted(u32* pOwnerPrincipalId)
{
    switch (m_NexCallContext.m_State) {
    case nex::CallContext::STATE_FAILURE: {
        nex::qResult result = m_NexCallContext.m_Result;
        if (result.m_Code == NEX_RESULT_80030073) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_SESSION_GONE);
        } else if (result.m_Code == NEX_RESULT_800300CF) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_247);
        } else {
            s_NetworkErrorCode = nex::ErrorCodeConverter::ConvertToNetworkErrorCode(result);
            m_pCallContext->SignalFailure(common::RESULT_FATAL_196);
        }
        m_pCallContext = nullptr;
        m_NexCallContext.Reset();
        return true;
    }
    case nex::CallContext::STATE_SUCCESS: {
        nex::MatchmakeSession* pSession = static_cast<nex::MatchmakeSession*>(m_Gathering.Get());
        *pOwnerPrincipalId = pSession->m_OwnerPrincipalId;
        m_pCallContext->SignalSuccess(nn::Result());
        m_pCallContext = nullptr;
        pSession->Reset();
        m_NexCallContext.Reset();
        m_Gathering.m_pObject = nullptr;
        delete pSession;
        return true;
    }
    default:
        if (!IsExpired(m_Deadline)) {
            return false;
        }
        m_NexCallContext.Cancel(nex::CallContext::STATE_CANCELLED);
        m_pCallContext->SignalFailure(common::RESULT_UNREGISTER_FAILED);
        m_NexCallContext.Reset();
        m_pCallContext = nullptr;
        return true;
    }
}

// 0x003FD524 slot 0x54
bool nn::pia::inet::NexMatchmakeSession::vf_0x54(nn::pia::transport::StationConnectionInfo* pInfo)
{
    switch (m_NexCallContext.m_State) {
    case nex::CallContext::STATE_FAILURE: {
        nex::qResult result = m_NexCallContext.m_Result;
        if (result.m_Code == NEX_RESULT_80030073) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_SESSION_GONE);
        } else if (result.m_Code == NEX_RESULT_800300D9) {
            m_pCallContext->SignalFailure(common::RESULT_INVALID_STATE);
        } else {
            s_NetworkErrorCode = nex::ErrorCodeConverter::ConvertToNetworkErrorCode(result);
            m_pCallContext->SignalFailure(common::RESULT_FATAL_196);
        }
        m_pCallContext = nullptr;
        m_pSessionUrls->clear();
        m_NexCallContext.Reset();
        return true;
    }
    case nex::CallContext::STATE_SUCCESS: {
        nn::Result result = NexFacade::ConvertNexStationUrlToStationConnectionInfo(*m_pSessionUrls, pInfo);
        m_pSessionUrls->clear();
        if (result.IsFailure()) {
            m_pCallContext->SignalFailure(result);
            m_pCallContext = nullptr;
            return true;
        }
        m_pCallContext->SignalSuccess(nn::Result());
        m_pCallContext = nullptr;
        m_NexCallContext.Reset();
        return true;
    }
    default:
        return false;
    }
}

// 0x003FD650 slot 0xAC (name is ours)
nn::Result nn::pia::inet::NexMatchmakeSession::CreateWithParticipantsAsync(nn::pia::common::CallContext* pCallContext, const u32* pPrincipalIds,
                                                                             u32 principalNum, u32 gatheringIdForParticipationCheck, bool isOptionSet)
{
    if (!common::IsValidPointer(pCallContext) || pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_pCallContext != nullptr || m_NexCallContext.m_State == nex::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_STATE;
    }
    m_pCallContext = pCallContext;
    nex::CreateMatchmakeSessionParam param;
    param.m_SourceMatchmakeSession = *m_pSourceSession;
    param.m_JoinMessage = m_pSourceSession->m_Description;
    nex::qList<u32> participants;
    for (u32 i = 0; i < principalNum; i++) {
        participants.push_back(pPrincipalIds[i]);
    }
    param.m_AdditionalParticipants = participants;
    param.m_GatheringIdForParticipationCheck = gatheringIdForParticipationCheck;
    if (isOptionSet) {
        param.m_CreateMatchmakeSessionOption = 1;
    }
    if (!m_pClient->CreateMatchmakeSession(&m_NexCallContext, param, m_pMatchmakeSession)) {
        m_pCallContext = nullptr;
        return common::RESULT_UNREGISTER_FAILED;
    }
    m_pCallContext->InitiateCall();
    return nn::Result();
}

// 0x003FD868 slot 0x3C
bool nn::pia::inet::NexMatchmakeSession::IsUnregisterCompleted()
{
    switch (m_NexCallContext.m_State) {
    case nex::CallContext::STATE_FAILURE: {
        nex::qResult result = m_NexCallContext.m_Result;
        if (result.m_Code == NEX_RESULT_80030073) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_SESSION_GONE);
        } else if (result.m_Code == NEX_RESULT_800300D9) {
            m_pCallContext->SignalFailure(common::RESULT_INVALID_STATE);
        } else {
            s_NetworkErrorCode = nex::ErrorCodeConverter::ConvertToNetworkErrorCode(result);
            m_pCallContext->SignalFailure(common::RESULT_FATAL_196);
        }
        m_NexCallContext.Reset();
        m_pMatchmakeSession->Reset();
        m_pCallContext = nullptr;
        break;
    }
    case nex::CallContext::STATE_SUCCESS:
        m_Unknown0x3C = false;
        m_IsHost = false;
        m_Unknown0x38 = 0;
        m_NexCallContext.Reset();
        m_pMatchmakeSession->Reset();
        m_pCallContext->SignalSuccess(nn::Result());
        m_pCallContext = nullptr;
        break;
    default:
        if (!IsExpired(m_Deadline)) {
            return false;
        }
        m_NexCallContext.Cancel(nex::CallContext::STATE_CANCELLED);
        m_pCallContext->SignalFailure(common::RESULT_UNREGISTER_FAILED);
        m_NexCallContext.Reset();
        m_pMatchmakeSession->Reset();
        m_pCallContext = nullptr;
        break;
    }
    RemoveSessionEntry(m_SessionId);
    m_SessionId = 0;
    return true;
}

// 0x003FDA14 (name is ours)
bool nn::pia::inet::NexMatchmakeSession::IsGetSessionStatusCompleted()
{
    switch (m_NexCallContext.m_State) {
    case nex::CallContext::STATE_CALL_IN_PROGRESS:
        return false;
    case nex::CallContext::STATE_FAILURE: {
        nex::qResult result = m_NexCallContext.m_Result;
        if (result.m_Code == NEX_RESULT_80030073) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_SESSION_GONE);
        } else if (result.m_Code == NEX_RESULT_800300D9) {
            m_pCallContext->SignalFailure(common::RESULT_INVALID_STATE);
        } else {
            s_NetworkErrorCode = nex::ErrorCodeConverter::ConvertToNetworkErrorCode(result);
            m_pCallContext->SignalFailure(common::RESULT_FATAL_196);
        }
        m_pCallContext = nullptr;
        m_NexCallContext.Reset();
        return true;
    }
    case nex::CallContext::STATE_SUCCESS: {
        if (NexFacade::ConvertNexSessionKeyToSignatureSetting(m_pMatchmakeSession->m_SessionKey, &m_SignatureSetting).IsFailure()) {
            m_pCallContext->SignalFailure(common::RESULT_INVALID_STATE);
        }
        nex::MatchmakeSession* pSession = m_pMatchmakeSession;
        m_Unknown0x4 = pSession->m_IsOpenParticipation;
        m_Unknown0x38 = pSession->m_OwnerPrincipalId;
        m_Unknown0x3C = m_Unknown0x38 == session::Session::s_pInstance->m_Unknown0x8C;
        m_IsHost = pSession->m_HostPrincipalId == session::Session::s_pInstance->m_Unknown0x8C;
        m_Unknown0x6 = pSession->m_ParticipationCount;
        m_MaxParticipants = pSession->m_MaxParticipants;
        m_MinParticipants = pSession->m_MinParticipants;
        m_pCallContext->SignalSuccess(nn::Result());
        m_pCallContext = nullptr;
        m_NexCallContext.Reset();
        return true;
    }
    default:
        if (!IsExpired(m_Deadline)) {
            return false;
        }
        m_NexCallContext.Cancel(nex::CallContext::STATE_CANCELLED);
        m_pCallContext->SignalFailure(common::RESULT_UNREGISTER_FAILED);
        m_NexCallContext.Reset();
        m_pCallContext = nullptr;
        return true;
    }
}

// 0x003FDBFC slot 0x1C
nn::pia::session::ISessionInfoList* nn::pia::inet::NexMatchmakeSession::GetSessionInfoList()
{
    return m_pSessionInfoList;
}

// 0x003FDC04 (name is ours)
void nn::pia::inet::NexMatchmakeSession::SetCreateSetting(const NexCreateSessionSetting* pSetting, bool isHostMigrationEnabled)
{
    m_pSourceSession->Reset();
    m_pSourceSession->m_GameMode = pSetting->m_Unknown0x8;
    m_pSourceSession->m_MaxParticipants = pSetting->m_Unknown0x6;
    m_pSourceSession->m_MinParticipants = pSetting->m_Unknown0x4;
    m_pSourceSession->SetMatchmakeSystemType(static_cast<nex::MatchmakeSystemType>(ConvertMatchmakeSystemType(pSetting->m_Unknown0xC)), 0);
    m_pSourceSession->SetDescription(nex::String(pSetting->m_Description));
    m_pSourceSession->m_IsOpenParticipation = pSetting->IsOpenParticipation();
    m_pSourceSession->m_UserPassword = nex::String(pSetting->GetUserPassword());
    m_pSourceSession->SetProgressScore(pSetting->m_ProgressScore);
    m_pSourceSession->m_ReferGatheringId = pSetting->m_ReferGatheringId;
    nex::MatchmakeParam param;
    if (pSetting->IsParamRVSet()) {
        param.SetParam(nex::String(nex::g_MatchmakeParamKeyRV), nex::Variant(pSetting->m_ParamRV));
    }
    if (pSetting->IsParamDRSet()) {
        param.SetParam(nex::String(nex::g_MatchmakeParamKeyDR), nex::Variant(pSetting->m_ParamDR));
    }
    if (pSetting->IsParamVRSet()) {
        param.SetParam(nex::String(nex::g_MatchmakeParamKeyVR), nex::Variant(pSetting->m_ParamVR));
    }
    if (pSetting->IsParamUsGISet()) {
        param.SetParam(nex::String(nex::g_MatchmakeParamKeyUsGI), nex::Variant(pSetting->GetParamUsGI()));
    }
    if (pSetting->IsParamNCCSet()) {
        param.SetParam(nex::String(nex::g_MatchmakeParamKeyNCC), nex::Variant(pSetting->m_ParamNCC));
    }
    if (pSetting->IsParamOIASet()) {
        nex::String oia(pSetting->GetParamOIA());
        param.SetParam(nex::String(nex::g_MatchmakeParamKeyOIA), nex::Variant(oia));
    }
    if (pSetting->IsAnyParamSet()) {
        m_pSourceSession->m_MatchmakeParam = param;
        // (the value is not used)
        u32 ncc = 0;
        param.GetParam(nex::String(nex::g_MatchmakeParamKeyNCC), &ncc);
    }
    common::g_SessionBeginMonitoringContent.m_Unknown0x184 = pSetting->m_Unknown0x8;
    common::g_SessionBeginMonitoringContent.m_Unknown0x1A0 = pSetting->m_Unknown0x6;
    common::g_SessionBeginMonitoringContent.m_Unknown0x22D = pSetting->GetUnknown0x474();
    if (*pSetting->GetUserPassword() != 0) {
        IncrementCount(&common::g_SessionStateMonitoringContent.m_Unknown0x3CA);
    }
    common::g_SessionStateMonitoringContent.m_Unknown0x3C9 = pSetting->IsOpenParticipation();
    nex::qVector<u8> data;
    pSetting->GetApplicationData(&data);
    m_pSourceSession->SetApplicationBuffer(data);
    for (u32 i = 0; i < NexCreateSessionSetting::ATTRIBUTE_NUM; i++) {
        m_pSourceSession->SetAttribute(i, pSetting->GetAttribute(i));
        common::g_SessionBeginMonitoringContent.m_Unknown0x188[i] = pSetting->GetAttribute(i);
    }
    m_pSourceSession->m_Flags |= (isHostMigrationEnabled ? 0x210 : 0) | 0x800;
}

// 0x003FE1D8 slot 0xA8 (name is ours)
bool nn::pia::inet::NexMatchmakeSession::IsAutoMatchmakeWithParticipantsCompleted(u32* pSessionId, bool* pIsCreator, u32* pJointSessionId,
                                                                                  void* pApplicationData, u32* pApplicationDataSize)
{
    switch (m_NexCallContext.m_State) {
    case nex::CallContext::STATE_FAILURE: {
        nex::qResult result = m_NexCallContext.m_Result;
        if (result.m_Code == NEX_RESULT_800300D4) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_252);
        } else if (result.m_Code == NEX_RESULT_INVALID_ARGUMENT) {
            m_pCallContext->SignalFailure(common::RESULT_INVALID_ARGUMENT);
        } else if (result.m_Code == NEX_RESULT_800300CF) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_247);
        } else if (result.m_Code == NEX_RESULT_800300C8) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_244);
        } else if (result.m_Code == NEX_RESULT_800300CD) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_245);
        } else if (result.m_Code == NEX_RESULT_800300CE) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_246);
        } else if (result.m_Code == NEX_RESULT_800300DB) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_251);
        } else if (result.m_Code == NEX_RESULT_800300DA) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_253);
        } else if (result.m_Code == NEX_RESULT_800300D8) {
            m_pCallContext->SignalFailure(common::RESULT_INVALID_STATE);
        } else if (result.m_Code == NEX_RESULT_800300D7) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_252);
        } else {
            s_NetworkErrorCode = nex::ErrorCodeConverter::ConvertToNetworkErrorCode(result);
            m_pCallContext->SignalFailure(common::RESULT_FATAL_196);
        }
        m_NexCallContext.Reset();
        m_pCallContext = nullptr;
        return true;
    }
    case nex::CallContext::STATE_SUCCESS:
        break;
    default:
        return false;
    }

    nn::Result result = NexFacade::ConvertNexSessionKeyToSignatureSetting(m_pMatchmakeSession->m_SessionKey, &m_SignatureSetting);
    if (result.IsFailure()) {
        m_pCallContext->SignalFailure(result);
        m_pCallContext = nullptr;
        m_NexCallContext.Reset();
        return true;
    }
    nex::MatchmakeSession* pSession = m_pMatchmakeSession;
    m_IsHost = session::Session::s_pInstance->m_Unknown0x8C == pSession->m_HostPrincipalId;
    m_Unknown0x3C = session::Session::s_pInstance->m_Unknown0x8C == pSession->m_OwnerPrincipalId;
    if (m_IsHost ? !m_Unknown0x3C : m_Unknown0x3C) {
        m_pCallContext->SignalFailure(common::RESULT_HOST_OWNER_MISMATCH);
        m_pCallContext = nullptr;
        m_NexCallContext.Reset();
        return true;
    }
    m_Unknown0x38 = pSession->m_OwnerPrincipalId;
    *pIsCreator = m_Unknown0x3C;
    m_SessionId = m_pMatchmakeSession->m_Id;
    *pSessionId = m_SessionId;
    m_Unknown0x4 = m_pMatchmakeSession->m_IsOpenParticipation;
    if (pApplicationData != nullptr && pApplicationDataSize != nullptr) {
        nex::qVector<u8> buffer;
        buffer.assign(m_pMatchmakeSession->m_ApplicationBuffer.begin(), m_pMatchmakeSession->m_ApplicationBuffer.end());
        std::memcpy(pApplicationData, buffer.begin(), buffer.end() - buffer.begin());
        *pApplicationDataSize = buffer.end() - buffer.begin();
    }
    if (pJointSessionId != nullptr && session::Session::s_pInstance->m_pMeshLayerController->vf_0x34()) {
        if (!m_pMatchmakeSession->m_MatchmakeParam.GetParamLGFPC(pJointSessionId)) {
            *pJointSessionId = 0;
        }
    }
    if (m_IsHost) {
        session::Session::s_pInstance->SetJoinable(m_SessionId, m_pMatchmakeSession->m_IsOpenParticipation);
    }
    m_pCallContext->SignalSuccess(nn::Result());
    m_pCallContext = nullptr;
    m_NexCallContext.Reset();
    return true;
}

// 0x003FE5E8 slot 0xB8 (name is ours)
bool nn::pia::inet::NexMatchmakeSession::IsJoinWithParticipantsCompleted(u32* pJointSessionId, void* pApplicationData, u32* pApplicationDataSize)
{
    switch (m_NexCallContext.m_State) {
    case nex::CallContext::STATE_FAILURE: {
        nex::qResult result = m_NexCallContext.m_Result;
        if (result.m_Code == NEX_RESULT_800300D4) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_252);
        } else if (result.m_Code == NEX_RESULT_INVALID_ARGUMENT) {
            m_pCallContext->SignalFailure(common::RESULT_INVALID_ARGUMENT);
        } else if (result.m_Code == NEX_RESULT_80030073) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_SESSION_GONE);
        } else if (result.m_Code == NEX_RESULT_800300C8) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_244);
        } else if (result.m_Code == NEX_RESULT_800300CD) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_245);
        } else if (result.m_Code == NEX_RESULT_800300CE) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_246);
        } else if (result.m_Code == NEX_RESULT_800300D5) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_249);
        } else if (result.m_Code == NEX_RESULT_800300D6) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_250);
        } else if (result.m_Code == NEX_RESULT_800300DB) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_251);
        } else if (result.m_Code == NEX_RESULT_800300DA) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_253);
        } else if (result.m_Code == NEX_RESULT_800300CF) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_247);
        } else if (result.m_Code == NEX_RESULT_800300D8) {
            m_pCallContext->SignalFailure(common::RESULT_INVALID_STATE);
        } else if (result.m_Code == NEX_RESULT_800300D7) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_252);
        } else {
            s_NetworkErrorCode = nex::ErrorCodeConverter::ConvertToNetworkErrorCode(result);
            m_pCallContext->SignalFailure(common::RESULT_FATAL_196);
        }
        break;
    }
    case nex::CallContext::STATE_SUCCESS: {
        m_SessionId = m_pMatchmakeSession->m_Id;
        if (NexFacade::ConvertNexSessionKeyToSignatureSetting(m_pMatchmakeSession->m_SessionKey, &m_SignatureSetting).IsFailure()) {
            m_pCallContext->SignalFailure(common::RESULT_INVALID_STATE);
            break;
        }
        nex::MatchmakeSession* pSession = m_pMatchmakeSession;
        m_IsHost = session::Session::s_pInstance->m_Unknown0x8C == pSession->m_HostPrincipalId;
        m_Unknown0x3C = session::Session::s_pInstance->m_Unknown0x8C == pSession->m_OwnerPrincipalId;
        if (m_IsHost || m_Unknown0x3C) {
            m_pCallContext->SignalFailure(common::RESULT_HOST_OWNER_MISMATCH);
            break;
        }
        m_Unknown0x38 = pSession->m_OwnerPrincipalId;
        m_Unknown0x4 = pSession->m_IsOpenParticipation;
        if (pApplicationData != nullptr && pApplicationDataSize != nullptr) {
            nex::qVector<u8> buffer;
            buffer.assign(m_pMatchmakeSession->m_ApplicationBuffer.begin(), m_pMatchmakeSession->m_ApplicationBuffer.end());
            std::memcpy(pApplicationData, buffer.begin(), buffer.end() - buffer.begin());
            *pApplicationDataSize = buffer.end() - buffer.begin();
        }
        if (pJointSessionId != nullptr && session::Session::s_pInstance->m_pMeshLayerController->vf_0x34()) {
            if (!m_pMatchmakeSession->m_MatchmakeParam.GetParamLGFPC(pJointSessionId)) {
                *pJointSessionId = 0;
            }
        }
        m_pCallContext->SignalSuccess(nn::Result());
        break;
    }
    default:
        return false;
    }
    m_pCallContext = nullptr;
    m_NexCallContext.Reset();
    return true;
}

// 0x003FE9D8 slot 0xE0
bool nn::pia::inet::NexMatchmakeSession::IsMigrateOwnerCompleted()
{
    switch (m_NexCallContext.m_State) {
    case nex::CallContext::STATE_FAILURE: {
        nex::qResult result = m_NexCallContext.m_Result;
        if (result.m_Code == NEX_RESULT_80030073) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_SESSION_GONE);
        } else if (result.m_Code == NEX_RESULT_800300D9 || result.m_Code == NEX_RESULT_800300D4 || result.m_Code == NEX_RESULT_800300D7) {
            m_pCallContext->SignalFailure(common::RESULT_INVALID_STATE);
        } else {
            s_NetworkErrorCode = nex::ErrorCodeConverter::ConvertToNetworkErrorCode(result);
            m_pCallContext->SignalFailure(common::RESULT_FATAL_196);
        }
        m_pCallContext = nullptr;
        m_pPrincipalIds->clear();
        m_NexCallContext.Reset();
        return true;
    }
    case nex::CallContext::STATE_SUCCESS:
        m_pCallContext->SignalSuccess(nn::Result());
        m_pCallContext = nullptr;
        m_pPrincipalIds->clear();
        m_NexCallContext.Reset();
        return true;
    default:
        if (!IsExpired(m_Deadline)) {
            return false;
        }
        m_NexCallContext.Cancel(nex::CallContext::STATE_CANCELLED);
        m_pCallContext->SignalFailure(common::RESULT_UNREGISTER_FAILED);
        m_pPrincipalIds->clear();
        m_NexCallContext.Reset();
        m_pCallContext = nullptr;
        return true;
    }
}

// 0x003FEB50 slot 0xB0 (name is ours)
bool nn::pia::inet::NexMatchmakeSession::IsCreateWithParticipantsCompleted(u32* pSessionId, void* pApplicationData, u32* pApplicationDataSize)
{
    switch (m_NexCallContext.m_State) {
    case nex::CallContext::STATE_FAILURE: {
        nex::qResult result = m_NexCallContext.m_Result;
        if (result.m_Code == NEX_RESULT_800300D4) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_252);
        } else if (result.m_Code == NEX_RESULT_INVALID_ARGUMENT) {
            m_pCallContext->SignalFailure(common::RESULT_INVALID_ARGUMENT);
        } else if (result.m_Code == NEX_RESULT_800300CF) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_247);
        } else if (result.m_Code == NEX_RESULT_800300D7) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_FAILED_252);
        } else {
            s_NetworkErrorCode = nex::ErrorCodeConverter::ConvertToNetworkErrorCode(result);
            m_pCallContext->SignalFailure(common::RESULT_FATAL_196);
        }
        break;
    }
    case nex::CallContext::STATE_SUCCESS: {
        if (NexFacade::ConvertNexSessionKeyToSignatureSetting(m_pMatchmakeSession->m_SessionKey, &m_SignatureSetting).IsFailure()) {
            m_pCallContext->SignalFailure(common::RESULT_INVALID_STATE);
            break;
        }
        nex::MatchmakeSession* pSession = m_pMatchmakeSession;
        m_IsHost = session::Session::s_pInstance->m_Unknown0x8C == pSession->m_HostPrincipalId;
        m_Unknown0x3C = session::Session::s_pInstance->m_Unknown0x8C == pSession->m_OwnerPrincipalId;
        if (!m_IsHost || !m_Unknown0x3C) {
            m_pCallContext->SignalFailure(common::RESULT_HOST_OWNER_MISMATCH);
            break;
        }
        m_Unknown0x38 = pSession->m_OwnerPrincipalId;
        m_SessionId = pSession->m_Id;
        *pSessionId = m_SessionId;
        if (pApplicationData != nullptr && pApplicationDataSize != nullptr) {
            nex::qVector<u8> buffer;
            buffer.assign(m_pMatchmakeSession->m_ApplicationBuffer.begin(), m_pMatchmakeSession->m_ApplicationBuffer.end());
            std::memcpy(pApplicationData, buffer.begin(), buffer.end() - buffer.begin());
            *pApplicationDataSize = buffer.end() - buffer.begin();
        }
        m_pCallContext->SignalSuccess(nn::Result());
        break;
    }
    default:
        return false;
    }
    m_pCallContext = nullptr;
    m_NexCallContext.Reset();
    return true;
}

// 0x003FED7C (name is ours)
u32 nn::pia::inet::NexMatchmakeSession::ConvertMatchmakeSystemType(u8 type)
{
    if (type == 0) {
        return 1;
    }
    if (type == 1) {
        return 2;
    }
    return 0;
}

// 0x003FED98 slot 0x78
nn::Result nn::pia::inet::NexMatchmakeSession::vf_0x78(nn::pia::common::CallContext* pCallContext, u32 sessionId)
{
    if (!common::IsValidPointer(pCallContext) || pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_pCallContext != nullptr || m_NexCallContext.m_State == nex::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_STATE;
    }
    m_pCallContext = pCallContext;
    if (!m_pClient->GetMatchmakeSession(&m_NexCallContext, sessionId, m_pMatchmakeSession)) {
        m_pCallContext = nullptr;
        return common::RESULT_UNREGISTER_FAILED;
    }
    m_Deadline = GetTimeAfter(CALL_TIMEOUT_MSEC);
    m_pCallContext->InitiateCall();
    return nn::Result();
}

// 0x003FEE6C (name is ours)
bool nn::pia::inet::NexMatchmakeSession::IsGetMatchmakeSessionCompleted(nn::nex::MatchmakeSession** ppSession)
{
    switch (m_NexCallContext.m_State) {
    case nex::CallContext::STATE_CALL_IN_PROGRESS:
        return false;
    case nex::CallContext::STATE_FAILURE: {
        nex::qResult result = m_NexCallContext.m_Result;
        if (result.m_Code == NEX_RESULT_80030073) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_SESSION_GONE);
        } else if (result.m_Code == NEX_RESULT_800300D9) {
            m_pCallContext->SignalFailure(common::RESULT_INVALID_STATE);
        } else {
            s_NetworkErrorCode = nex::ErrorCodeConverter::ConvertToNetworkErrorCode(result);
            m_pCallContext->SignalFailure(common::RESULT_FATAL_196);
        }
        m_pCallContext = nullptr;
        m_NexCallContext.Reset();
        return true;
    }
    case nex::CallContext::STATE_SUCCESS:
        *ppSession = m_pMatchmakeSession;
        m_pCallContext->SignalSuccess(nn::Result());
        m_pCallContext = nullptr;
        m_NexCallContext.Reset();
        return true;
    default:
        if (!IsExpired(m_Deadline)) {
            return false;
        }
        m_NexCallContext.Cancel(nex::CallContext::STATE_CANCELLED);
        m_pCallContext->SignalFailure(common::RESULT_UNREGISTER_FAILED);
        m_NexCallContext.Reset();
        m_pCallContext = nullptr;
        return true;
    }
}

// 0x003FEFCC (name is ours)
u32 nn::pia::inet::NexMatchmakeSession::ConvertSelectionMethod(u8 method)
{
    if (method == 0) {
        return 0;
    }
    if (method == 1) {
        return 4;
    }
    if (method == 2) {
        return 5;
    }
    return 0;
}

// 0x003FEFF0 slot 0x08
void nn::pia::inet::NexMatchmakeSession::Cleanup()
{
    m_SourceGathering.m_pObject = nullptr;
    m_Gathering.m_pObject = nullptr;
    if (m_pSearchCriteria != nullptr) {
        m_pSearchCriteria->clear();
    }
    if (m_pResultRanges != nullptr) {
        m_pResultRanges->clear();
    }
    m_pSessionInfoList->Clear();
    if (m_pGatherings != nullptr) {
        m_pGatherings->clear();
    }
    if (m_pSessionUrls != nullptr) {
        m_pSessionUrls->clear();
    }
    m_SessionId = 0;
    m_pMatchmakeSession->Reset();
    m_pJoinParam->Reset();
    m_pUpdateParam->Reset();
    if (m_pUnknown0x7C != nullptr) {
        m_pUnknown0x7C->clear();
    }
    if (m_pPlayingSessions != nullptr) {
        m_pPlayingSessions->clear();
    }
    if (m_pPrincipalIds != nullptr) {
        m_pPrincipalIds->clear();
    }
    if (m_NexCallContext.m_State == nex::CallContext::STATE_CALL_IN_PROGRESS) {
        m_NexCallContext.Cancel(nex::CallContext::STATE_CANCELLED);
        m_NexCallContext.Reset();
    }
    if (m_pCallContext != nullptr) {
        if (m_pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
            m_pCallContext->SignalFailure(common::RESULT_CANCELED);
        }
        m_pCallContext = nullptr;
    }
    CommonMatchmakeSession::Cleanup();
    m_Unknown0x3C = false;
    m_IsHost = false;
    m_Unknown0x38 = 0;
    m_Unknown0x4 = false;
    m_Unknown0x6 = 0;
    m_MatchmakeSystemType = 0;
}

// 0x003FF1A0 (name is ours)
nn::Result nn::pia::inet::NexMatchmakeSession::Unbind()
{
    m_pClient->Unbind();
    if (m_pNgsBridge != nullptr) {
        m_pNgsBridge = nullptr;
    }
    return nn::Result();
}

// 0x003FF1D0 slot 0xEC (name is ours)
nn::Result nn::pia::inet::NexMatchmakeSession::FindGatheringAsync(nn::pia::common::CallContext* pCallContext, u32 sessionId)
{
    if (!common::IsValidPointer(pCallContext) || pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_pCallContext != nullptr || m_NexCallContext.m_State == nex::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_STATE;
    }
    m_pCallContext = pCallContext;
    delete m_Gathering.Release();
    if (!m_pClient->FindBySingleID(&m_NexCallContext, sessionId, &m_Gathering)) {
        m_pCallContext = nullptr;
        return common::RESULT_UNREGISTER_FAILED;
    }
    m_pCallContext->InitiateCall();
    return nn::Result();
}

// 0x003FF280
nn::pia::inet::NexMatchmakeSession::NexMatchmakeSession()
    : m_SessionId(0), m_pCallContext(nullptr), m_IsHost(false), m_Unknown0x110(0), m_MatchmakeSystemType(0)
{
    m_Unknown0x3C = false;
    m_Unknown0x38 = 0;
    void* pBuffer = pead::AllocMemory(sizeof(nex::MatchmakeExtensionClient), common::HeapManager::GetHeap());
    m_pClient = ::new (pBuffer) nex::MatchmakeExtensionClient();
    pBuffer = pead::AllocMemory(sizeof(nex::MatchmakeSession), common::HeapManager::GetHeap());
    m_pSourceSession = ::new (pBuffer) nex::MatchmakeSession();
    pBuffer = pead::AllocMemory(sizeof(nex::qList<nex::MatchmakeSessionSearchCriteria>), common::HeapManager::GetHeap());
    m_pSearchCriteria = ::new (pBuffer) nex::qList<nex::MatchmakeSessionSearchCriteria>();
    pBuffer = pead::AllocMemory(sizeof(nex::qList<nex::ResultRange>), common::HeapManager::GetHeap());
    m_pResultRanges = ::new (pBuffer) nex::qList<nex::ResultRange>();
    pBuffer = pead::AllocMemory(sizeof(nex::qList<GatheringHolder>), common::HeapManager::GetHeap());
    m_pGatherings = ::new (pBuffer) nex::qList<GatheringHolder>();
    pBuffer = pead::AllocMemory(sizeof(nex::qList<nex::StationURL>), common::HeapManager::GetHeap());
    m_pSessionUrls = ::new (pBuffer) nex::qList<nex::StationURL>();
    pBuffer = pead::AllocMemory(sizeof(nex::String), common::HeapManager::GetHeap());
    m_pSystemPassword = ::new (pBuffer) nex::String();
    m_pSystemPassword->Reserve(16);
    pBuffer = pead::AllocMemory(sizeof(nex::MatchmakeSession), common::HeapManager::GetHeap());
    m_pMatchmakeSession = ::new (pBuffer) nex::MatchmakeSession();
    pBuffer = pead::AllocMemory(sizeof(nex::JoinMatchmakeSessionParam), common::HeapManager::GetHeap());
    m_pJoinParam = ::new (pBuffer) nex::JoinMatchmakeSessionParam();
    m_pSessionInfoList = session::Session::s_pInstance->m_pSessionInfoList;
    pBuffer = pead::AllocMemory(sizeof(nex::UpdateMatchmakeSessionParam), common::HeapManager::GetHeap());
    m_pUpdateParam = ::new (pBuffer) nex::UpdateMatchmakeSessionParam();
    pBuffer = pead::AllocMemory(sizeof(nex::qList<Unknown0x7CEntry>), common::HeapManager::GetHeap());
    m_pUnknown0x7C = ::new (pBuffer) nex::qList<Unknown0x7CEntry>();
    pBuffer = pead::AllocMemory(sizeof(nex::qList<nex::PlayingSession>), common::HeapManager::GetHeap());
    m_pPlayingSessions = ::new (pBuffer) nex::qList<nex::PlayingSession>();
    pBuffer = pead::AllocMemory(sizeof(nex::qList<u32>), common::HeapManager::GetHeap());
    m_pPrincipalIds = ::new (pBuffer) nex::qList<u32>();
}

// 0x003FF8FC
// 0x003FF8EC (deleting dtor)
nn::pia::inet::NexMatchmakeSession::~NexMatchmakeSession()
{
    if (m_pClient != nullptr) {
        m_pClient->~MatchmakeExtensionClient();
        pead::FreeMemory(m_pClient);
        m_pClient = nullptr;
    }
    if (m_pSourceSession != nullptr) {
        m_pSourceSession->~MatchmakeSession();
        pead::FreeMemory(m_pSourceSession);
        m_pSourceSession = nullptr;
    }
    if (m_pSearchCriteria != nullptr) {
        m_pSearchCriteria->~qList();
        pead::FreeMemory(m_pSearchCriteria);
        m_pSearchCriteria = nullptr;
    }
    if (m_pResultRanges != nullptr) {
        m_pResultRanges->~qList();
        pead::FreeMemory(m_pResultRanges);
        m_pResultRanges = nullptr;
    }
    if (m_pGatherings != nullptr) {
        m_pGatherings->~qList();
        pead::FreeMemory(m_pGatherings);
        m_pGatherings = nullptr;
    }
    if (m_pSessionUrls != nullptr) {
        m_pSessionUrls->~qList();
        pead::FreeMemory(m_pSessionUrls);
        m_pSessionUrls = nullptr;
    }
    if (m_pUpdateParam != nullptr) {
        m_pUpdateParam->~UpdateMatchmakeSessionParam();
        pead::FreeMemory(m_pUpdateParam);
        m_pUpdateParam = nullptr;
    }
    if (m_pMatchmakeSession != nullptr) {
        m_pMatchmakeSession->~MatchmakeSession();
        pead::FreeMemory(m_pMatchmakeSession);
        m_pMatchmakeSession = nullptr;
    }
    if (m_pJoinParam != nullptr) {
        m_pJoinParam->~JoinMatchmakeSessionParam();
        pead::FreeMemory(m_pJoinParam);
        m_pJoinParam = nullptr;
    }
    if (m_pSystemPassword != nullptr) {
        m_pSystemPassword->~String();
        pead::FreeMemory(m_pSystemPassword);
        m_pSystemPassword = nullptr;
    }
    m_pSessionInfoList = nullptr;
    if (m_pUnknown0x7C != nullptr) {
        m_pUnknown0x7C->~qList();
        pead::FreeMemory(m_pUnknown0x7C);
        m_pUnknown0x7C = nullptr;
    }
    if (m_pPlayingSessions != nullptr) {
        m_pPlayingSessions->~qList();
        pead::FreeMemory(m_pPlayingSessions);
        m_pPlayingSessions = nullptr;
    }
    if (m_pPrincipalIds != nullptr) {
        m_pPrincipalIds->~qList();
        pead::FreeMemory(m_pPrincipalIds);
        m_pPrincipalIds = nullptr;
    }
}

} // namespace inet
} // namespace pia
} // namespace nn
