#include "nn/pia/local/local_LocalMatchmakeSession.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/local/local_LocalNetwork.h"
#include "nn/pia/local/local_LocalNetworkManager.h"

namespace nn {
namespace pia {
namespace local {
namespace {
// a call needs a call context that does not run
inline bool IsValidCallContext(const common::CallContext* pCallContext)
{
    return common::IsValidPointer(pCallContext) && pCallContext->m_State != common::CallContext::STATE_CALL_IN_PROGRESS;
}

// the lost network is the end of the session for the application
inline nn::Result ConvertResult(nn::Result result)
{
    if (result == common::RESULT_LOCAL_NETWORK_LOST) {
        return common::RESULT_NOT_IN_SESSION;
    }
    return result;
}
} // namespace

// 0x0041C3B0
nn::Result nn::pia::local::LocalMatchmakeSession::vf_0x60(nn::pia::common::CallContext*, u32)
{
    return common::RESULT_INTERNAL_ERROR;
}

// 0x0041C3BC
nn::Result nn::pia::local::LocalMatchmakeSession::ModifyAttributeAsync(nn::pia::common::CallContext*, u32, u32, u32)
{
    return common::RESULT_INTERNAL_ERROR;
}

// 0x0041C3C8
nn::Result nn::pia::local::LocalMatchmakeSession::OpenParticipationAsync(nn::pia::common::CallContext* pCallContext, u32)
{
    if (!IsValidCallContext(pCallContext)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_pCallContext != nullptr) {
        return common::RESULT_INVALID_STATE;
    }
    nn::Result result = m_pNetwork->AllowParticipating();
    if (result.IsFailure()) {
        return ConvertResult(result);
    }
    // done at once
    m_pCallContext = pCallContext;
    m_Unknown0x4 = true;
    pCallContext->InitiateCall();
    m_pCallContext->SignalSuccess(nn::Result());
    m_pCallContext = nullptr;
    return nn::Result();
}

// 0x0041C464
nn::Result nn::pia::local::LocalMatchmakeSession::CloseParticipationAsync(nn::pia::common::CallContext* pCallContext, u32)
{
    if (!IsValidCallContext(pCallContext)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_pCallContext != nullptr) {
        return common::RESULT_INVALID_STATE;
    }
    nn::Result result = m_pNetwork->DisallowParticipating(true);
    if (result.IsFailure()) {
        return ConvertResult(result);
    }
    m_pCallContext = pCallContext;
    m_Unknown0x4 = false;
    pCallContext->InitiateCall();
    m_pCallContext->SignalSuccess(nn::Result());
    m_pCallContext = nullptr;
    return nn::Result();
}

// 0x0041C500
bool nn::pia::local::LocalMatchmakeSession::vf_0x64(u32, u32*)
{
    return false;
}

// 0x0041C508
bool nn::pia::local::LocalMatchmakeSession::IsModifyAttributeCompleted()
{
    return false;
}

// 0x0041C510
bool nn::pia::local::LocalMatchmakeSession::IsOpenParticipationCompleted()
{
    return true;
}

// 0x0041C518
nn::Result nn::pia::local::LocalMatchmakeSession::AutoMatchmakeAsync(nn::pia::common::CallContext*)
{
    return common::RESULT_INTERNAL_ERROR;
}

// 0x0041C524
nn::Result nn::pia::local::LocalMatchmakeSession::JoinAsync(nn::pia::common::CallContext* pCallContext, u32)
{
    if (!IsValidCallContext(pCallContext)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_pCallContext != nullptr || m_Request != REQUEST_NONE) {
        return common::RESULT_INVALID_STATE;
    }
    nn::Result result = m_pNetwork->ConnectNetworkAsync(GetConnectNetworkSetting());
    if (result.IsFailure()) {
        return result;
    }
    m_pCallContext = pCallContext;
    m_Request = REQUEST_CONNECT;
    pCallContext->InitiateCall();
    return nn::Result();
}

// 0x0041C5BC
bool nn::pia::local::LocalMatchmakeSession::IsCloseParticipationCompleted()
{
    return true;
}

// 0x0041C5C4
nn::Result nn::pia::local::LocalMatchmakeSession::LeaveAsync(nn::pia::common::CallContext* pCallContext, u32)
{
    if (!IsValidCallContext(pCallContext)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_pCallContext != nullptr || m_Request != REQUEST_NONE) {
        return common::RESULT_INVALID_STATE;
    }
    m_pCallContext = pCallContext;
    nn::Result result = m_pNetwork->DisconnectNetworkAsync();
    if (result.IsFailure()) {
        m_pCallContext = nullptr;
        return result;
    }
    m_Request = REQUEST_DISCONNECT;
    m_pCallContext->InitiateCall();
    return nn::Result();
}

// 0x0041C64C
nn::Result nn::pia::local::LocalMatchmakeSession::BrowseAsync(nn::pia::common::CallContext* pCallContext)
{
    if (!IsValidCallContext(pCallContext)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_pCallContext != nullptr || m_Request != REQUEST_NONE) {
        return common::RESULT_INVALID_STATE;
    }
    m_pCallContext = pCallContext;
    nn::Result result = m_pNetwork->ScanNetworkAsync(m_SearchCriteria.m_LocalCommunicationId, m_SearchCriteria.m_SubId);
    if (result.IsFailure()) {
        m_pCallContext = nullptr;
        return result;
    }
    m_Request = REQUEST_SCAN;
    m_pCallContext->InitiateCall();
    return nn::Result();
}

// 0x0041C6DC
nn::Result nn::pia::local::LocalMatchmakeSession::CreateAsync(nn::pia::common::CallContext* pCallContext)
{
    if (!IsValidCallContext(pCallContext)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_pCallContext != nullptr || m_Request != REQUEST_NONE) {
        return common::RESULT_INVALID_STATE;
    }
    m_pCallContext = pCallContext;
    nn::Result result = m_pNetwork->CreateNetworkAsync(m_pCreateNetworkSetting);
    if (result.IsFailure()) {
        m_pCallContext = nullptr;
        return result;
    }
    m_Request = REQUEST_CREATE;
    m_pCallContext->InitiateCall();
    return nn::Result();
}

// 0x0041C764
void nn::pia::local::LocalMatchmakeSession::SetSearchCriteria(const nn::pia::local::LocalSessionSearchCriteria* pCriteria)
{
    m_SearchCriteria = *pCriteria;
}

// 0x0041C7B0
nn::Result nn::pia::local::LocalMatchmakeSession::vf_0x78(nn::pia::common::CallContext* pCallContext, u32)
{
    // nothing to check
    pCallContext->InitiateCall();
    pCallContext->SignalSuccess(nn::Result());
    return nn::Result();
}

// 0x0041C7D4
nn::Result nn::pia::local::LocalMatchmakeSession::vf_0x50(nn::pia::common::CallContext*, u32)
{
    return common::RESULT_INTERNAL_ERROR;
}

// 0x0041C7E0
bool nn::pia::local::LocalMatchmakeSession::IsAutoMatchmakeCompleted(u32*, bool*, u32*, void*, u32*)
{
    return false;
}

// 0x0041C7E8
bool nn::pia::local::LocalMatchmakeSession::IsJoinCompleted(u32*, void*, u32*)
{
    if (!m_pNetwork->IsConnectNetworkAsyncCompleted()) {
        return false;
    }
    nn::Result result = m_pNetwork->GetConnectNetworkAsyncResult();
    if (result.IsFailure()) {
        if (result == common::RESULT_LOCAL_CONNECT_FAILED_108) {
            m_pCallContext->SignalFailure(common::RESULT_MATCHMAKE_SESSION_GONE);
        } else if (result == common::RESULT_LOCAL_CONNECT_FAILED_109) {
            m_pCallContext->SignalFailure(common::RESULT_JOIN_REFUSED);
        } else if (result == common::RESULT_LOCAL_NETWORK_LOST) {
            m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        } else {
            m_pCallContext->SignalFailure(result);
        }
    } else {
        m_pCallContext->SignalSuccess(nn::Result());
    }
    m_pCallContext = nullptr;
    m_Request = REQUEST_NONE;
    return true;
}

// 0x0041C8B8
bool nn::pia::local::LocalMatchmakeSession::IsLeaveCompleted()
{
    if (!m_pNetwork->IsDisconnectNetworkAsyncCompleted()) {
        return false;
    }
    nn::Result result = m_pNetwork->GetDisconnectNetworkAsyncResult();
    if (result.IsFailure()) {
        m_pCallContext->SignalFailure(ConvertResult(result));
    } else {
        m_pCallContext->SignalSuccess(nn::Result());
    }
    m_pCallContext = nullptr;
    m_Request = REQUEST_NONE;
    return true;
}

// 0x0041C938
nn::Result nn::pia::local::LocalMatchmakeSession::SetApplicationData(const void* pData, u32 size)
{
    nn::Result result = m_pNetwork->SetApplicationData(pData, size);
    if (result.IsFailure()) {
        return ConvertResult(result);
    }
    return result;
}

// 0x0041C960
nn::Result nn::pia::local::LocalMatchmakeSession::UnregisterAsync(nn::pia::common::CallContext* pCallContext, u32)
{
    if (!IsValidCallContext(pCallContext)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_pCallContext != nullptr || m_Request != REQUEST_NONE) {
        return common::RESULT_INVALID_STATE;
    }
    m_pCallContext = pCallContext;
    nn::Result result = m_pNetwork->DestroyNetworkAsync();
    if (result.IsFailure()) {
        m_pCallContext = nullptr;
        return result;
    }
    m_Request = REQUEST_DESTROY;
    m_pCallContext->InitiateCall();
    return nn::Result();
}

// 0x0041C9E8
nn::Result nn::pia::local::LocalMatchmakeSession::vf_0x74(u32)
{
    return nn::Result();
}

// 0x0041C9F0
bool nn::pia::local::LocalMatchmakeSession::vf_0x7C()
{
    m_Unknown0x4 = LocalNetwork::s_pInstance->GetParticipationState() == 1;
    return true;
}

// 0x0041CA1C
bool nn::pia::local::LocalMatchmakeSession::vf_0x54(nn::pia::transport::StationConnectionInfo*)
{
    return false;
}

// 0x0041CA24
bool nn::pia::local::LocalMatchmakeSession::IsUnregisterCompleted()
{
    if (!m_pNetwork->IsDestroyNetworkAsyncCompleted()) {
        return false;
    }
    nn::Result result = m_pNetwork->GetDestroyNetworkAsyncResult();
    if (result.IsFailure()) {
        m_pCallContext->SignalFailure(ConvertResult(result));
    } else {
        m_pCallContext->SignalSuccess(nn::Result());
    }
    m_pCallContext = nullptr;
    m_Request = REQUEST_NONE;
    return true;
}

// 0x0041CAA4
void nn::pia::local::LocalMatchmakeSession::SetCreateSessionSetting(const nn::pia::local::LocalCreateSessionSetting*)
{
    // empty (in the original too)
}

// 0x0041CAA8
void nn::pia::local::LocalMatchmakeSession::Cleanup()
{
    switch (m_Request) {
    case REQUEST_SCAN:
        m_pNetwork->CancelScanNetworkAsync();
        break;
    case REQUEST_CONNECT:
        m_pNetwork->CancelConnectNetworkAsync();
        break;
    default:
        break;
    }
    if (m_pCallContext != nullptr) {
        if (m_pCallContext->m_State == common::CallContext::STATE_CALL_IN_PROGRESS) {
            m_pCallContext->SignalFailure(common::RESULT_CANCELED);
        }
        m_pCallContext = nullptr;
    }
    m_Request = REQUEST_NONE;
    m_Unknown0x4 = false;
    CommonMatchmakeSession::Cleanup();
}

// 0x0041CB2C
nn::Result nn::pia::local::LocalMatchmakeSession::vf_0xA4()
{
    return common::RESULT_INTERNAL_ERROR;
}

// 0x0041CB38
nn::pia::local::LocalMatchmakeSession::LocalMatchmakeSession()
    : m_pNetwork(LocalNetwork::s_pInstance), m_pCreateNetworkSetting(nullptr), m_pConnectNetworkSetting(nullptr), m_SearchCriteria(),
      m_Request(REQUEST_NONE), m_pCallContext(nullptr)
{
}

// 0x0041CBCC
// 0x0041CBB4 (deleting dtor)
nn::pia::local::LocalMatchmakeSession::~LocalMatchmakeSession()
{
    m_pNetwork = nullptr;
}

// 0x007311E8
bool nn::pia::local::LocalMatchmakeSession::vf_0x88() const
{
    if (!common::IsValidPointer(m_pNetwork)) {
        return false;
    }
    return m_pNetwork->IsHost();
}

// 0x0073120C
u32 nn::pia::local::LocalMatchmakeSession::vf_0x90() const
{
    if (!common::IsValidPointer(m_pNetwork)) {
        return 255;
    }
    return m_pNetwork->m_pNetworkManager->m_HostTransportId;
}

// 0x00731230
u16 nn::pia::local::LocalMatchmakeSession::vf_0x68() const
{
    return 0;
}

} // namespace local
} // namespace pia
} // namespace nn
