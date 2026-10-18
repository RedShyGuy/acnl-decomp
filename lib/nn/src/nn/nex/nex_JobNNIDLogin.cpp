#include "nn/nex/nex_JobNNIDLogin.h"
#include "nn/nex/nex_AuthenticationInfo.h"
#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_NgsFacade.h"
#include "nn/nex/nex_RendezVous.h"
#include "nn/nex/nex_StringStream.h"

namespace nn {
namespace nex {
namespace {
// the call could not be sent
const int RESULT_CALL_FAILED = static_cast<int>(0x8001000D);
// the time between the tests of the waiting step (ms)
const int WAIT_TIME = 250;
} // namespace

// 0x0035D194 | fefates:bytes [tier B]
void nn::nex::JobNNIDLogin::StepGameLogin()
{
    LoginData* pData = m_pLoginData;
    m_CallContext.Reset();
    m_pBackEndServices->m_LoginTimeout = m_Timeout;
    m_CustomData.Reset(new AuthenticationInfo());
    static_cast<AuthenticationInfo*>(m_CustomData.m_pObject)->m_Token = String(pData->m_Token);
    StringStream userName;
    userName.StreamNumber(m_PrincipalId);
    String host(pData->m_ServerAddress);
    if (m_pBackEndServices->m_Unknown0x140) {
        static_cast<AuthenticationInfo*>(m_CustomData.m_pObject)->m_ServerVersion = static_cast<u32>(-1);
    }
    static_cast<AuthenticationInfo*>(m_CustomData.m_pObject)->m_TokenType = 1;
    RendezVous* pRendezVous = static_cast<RendezVous*>(m_pBackEndServices);
    if (!pRendezVous->Login(&m_CallContext, userName.CStr(), pData->m_Password, host.m_pString, pData->m_ServerPort, m_CustomData, &m_pCredentials, nullptr)) {
        Complete(qResult(RESULT_CALL_FAILED));
        return;
    }
    SetStep(Step(&JobNNIDLogin::StepWaitingForGameLogin, ""));
}

// 0x0035D3AC | fefates:bytes [tier B]
void nn::nex::JobNNIDLogin::StepWaitingForGameLogin()
{
    if (Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId) == nullptr) {
        SetToComplete();
        return;
    }
    u8 state = m_CallContext.m_State;
    if (state != CallContext::STATE_SUCCESS && state != CallContext::STATE_FAILURE && state != CallContext::STATE_CANCELLED) {
        SetToWaiting(WAIT_TIME);
        return;
    }
    Complete(m_CallContext.m_Result);
}

} // namespace nex
} // namespace nn
