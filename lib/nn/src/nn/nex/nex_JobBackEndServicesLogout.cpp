#include "nn/nex/nex_JobBackEndServicesLogout.h"
#include "nn/nex/nex_AuthenticationClient.h"
#include "nn/nex/nex_BackEndServices.h"
#include "nn/nex/nex_ConnectionManager.h"
#include "nn/nex/nex_Network.h"
#include "nn/nex/nex_SecureConnectionClient.h"
#include "nn/nex/nex_StreamManager.h"

namespace nn {
namespace nex {
namespace {
// the results (the names are ours)
const int RESULT_SUCCESS = qResult::SUCCESS;
const int RESULT_FAILURE = static_cast<int>(0x80010001);
// the time between the checks of the termination (ms)
const int WAIT_TIME = 50;
} // namespace

// 0x003B24C0 | fefates:bytes [tier B]
void nn::nex::JobBackEndServicesLogout::CompleteLogout()
{
    m_pBackEndServices->PostLogoutCleanup();
    SetToComplete();
    CallContext* pContext = Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId);
    if (pContext != nullptr) {
        pContext->SetStateImpl(CallContext::STATE_SUCCESS, qResult(RESULT_SUCCESS), true);
    }
}

// 0x003B2530 | fefates:bytes [tier B]
void nn::nex::JobBackEndServicesLogout::CompleteLogoutFailure()
{
    m_pBackEndServices->PostLogoutCleanup();
    SetToComplete();
    CallContext* pContext = Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId);
    if (pContext != nullptr) {
        pContext->SetStateImpl(CallContext::STATE_FAILURE, qResult(RESULT_FAILURE), false);
    }
}

// 0x003B25A0 (name is ours)
void nn::nex::JobBackEndServicesLogout::RemoveStationURLs()
{
    u32 connectionId = m_pCredentials->m_ConnectionId;
    if (connectionId != 0 && Network::GetInstance() != nullptr) {
        qProtectedList<StationURL>& urls = Network::GetInstance()->m_StationUrls;
        ScopedCS lock(urls.m_Lock);
        for (qList<StationURL>::Node* pNode = urls.m_List.GetFirst(); !urls.m_List.IsEnd(pNode);) {
            if (pNode->m_Value.GetRVConnectionID() != connectionId) {
                pNode = pNode->m_pNext;
            } else if ((pNode->m_Value.GetType() & 2) != 0) {
                pNode = urls.m_List.erase(pNode);
            } else {
                pNode->m_Value.SetRVConnectionID(0);
                pNode = pNode->m_pNext;
            }
        }
    }
    StreamManager* pStreamManager = m_pCredentials->m_pStreamManager;
    if (pStreamManager != nullptr) {
        m_pCredentials->m_pStreamManager = nullptr;
        m_pStreamManager = pStreamManager;
        SetStep(Step(&JobBackEndServicesLogout::TerminateStreamManager, ""));
    } else {
        SetStep(Step(&JobBackEndServicesLogout::CompleteLogout, ""));
    }
}

// 0x003B2720 | fefates:bytes [tier B]
void nn::nex::JobBackEndServicesLogout::TerminateStreamManager()
{
    m_pStreamManager->Terminate();
    SetStep(Step(&JobBackEndServicesLogout::WaitForStreamManagerTermination, ""));
}

// 0x003B2778 | fefates:bytes [tier B]
void nn::nex::JobBackEndServicesLogout::ProcessAuthDisconnectionResult()
{
    if (m_CallContext.m_State != CallContext::STATE_SUCCESS) {
        SetStep(Step(&JobBackEndServicesLogout::CompleteLogoutFailure, ""));
    } else {
        m_pBackEndServices->GetAuthenticationClient()->m_PrincipalId = 0;
        m_pBackEndServices->GetAuthenticationClient()->Unbind();
        m_pBackEndServices->m_pCredentials->m_pSecureConnection = nullptr;
        SetStep(Step(&JobBackEndServicesLogout::DisconnectFromSecureConnectionService, ""));
    }
    m_CallContext.Reset();
}

// 0x003B282C | fefates:bytes [tier B]
void nn::nex::JobBackEndServicesLogout::WaitForStreamManagerTermination()
{
    if (m_pStreamManager->IsTerminated()) {
        delete m_pStreamManager;
        m_pStreamManager = nullptr;
        SetStep(Step(&JobBackEndServicesLogout::CompleteLogout, ""));
    } else {
        SetToWaiting(WAIT_TIME);
    }
}

// 0x003B28B8 | fefates:bytes [tier B]
void nn::nex::JobBackEndServicesLogout::ProcessSecConnDisconnectionResult()
{
    m_pBackEndServices->GetSecureConnectionClient()->Unbind();
    m_pBackEndServices->m_pCredentials->m_pConnection = nullptr;
    m_pBackEndServices->UnregisterServerProtocols();
    m_pBackEndServices->m_URL = StationURL();
    SetStep(Step(&JobBackEndServicesLogout::RemoveStationURLs, ""));
}

// 0x003B2954 | fefates:bytes [tier B]
void nn::nex::JobBackEndServicesLogout::DisconnectFromSecureConnectionService()
{
    Credentials* pCredentials = m_pBackEndServices->m_pCredentials;
    if (pCredentials->m_pConnection == nullptr) {
        SetStep(Step(&JobBackEndServicesLogout::RemoveStationURLs, ""));
        return;
    }
    StreamManager* pStreamManager = pCredentials != nullptr ? pCredentials->m_pStreamManager : nullptr;
    if (!pStreamManager->m_pConnectionManager->Disconnect(&m_CallContext, pCredentials->m_pConnection)) {
        SetStep(Step(&JobBackEndServicesLogout::ProcessSecConnDisconnectionResult, ""));
        return;
    }
    SetToSuspended();
    ResumeOnCallCompletion(&m_CallContext, new Step(&JobBackEndServicesLogout::ProcessSecConnDisconnectionResult, ""));
}

// 0x003B2A6C slot 0x00
// 0x003B2A48 (deleting dtor)
nn::nex::JobBackEndServicesLogout::~JobBackEndServicesLogout()
{
    // nothing to do
}

} // namespace nex
} // namespace nn
