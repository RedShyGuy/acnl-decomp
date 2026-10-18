#include "nn/nex/nex_JobManageAccount.h"
#include "nn/nex/nex_AccountManagementProtocolClient.h"
#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_Data.h"
#include "nn/nex/nex_KerberosEncryption.h"

namespace nn {
namespace nex {
namespace {
// the call could not be sent
const int RESULT_CALL_FAILED = static_cast<int>(0x8001000D);

// (inline) the call context of the job is still registered
inline bool IsCallContextRegistered(u32 id)
{
    return Core::GetInstance()->m_pCallContextRegister->GetCallContext(id) != nullptr;
}

// (inline) the object of the holder is released and deleted
inline void DeleteObject(AnyObjectHolder<Data, String>& holder)
{
    Data* pObject = holder.m_pObject;
    if (pObject != nullptr) {
        holder.m_pObject = nullptr;
        delete pObject;
    }
}

// (inline) a copy of the object of the holder (pDefault if there is none)
inline Data* CloneObject(const AnyObjectHolder<Data, String>* pHolder)
{
    return pHolder != nullptr && pHolder->m_pObject != nullptr ? pHolder->m_pObject->Clone() : nullptr;
}
} // namespace

// 0x00380FD4 | fefates:bytes [tier B]
void nn::nex::JobManageAccount::CompleteJob()
{
    if (!IsCallContextRegistered(m_CallContextId)) {
        Cancel();
        return;
    }
    if (m_CallContext.m_State != CallContext::STATE_SUCCESS) {
        CompleteJobImpl(static_cast<CallContext::State>(m_CallContext.m_State), m_CallContext.m_Result);
    } else {
        CompleteJobImpl(static_cast<CallContext::State>(m_ProtocolCallContext.m_State), m_ProtocolCallContext.m_Result);
    }
}

// 0x0038105C
void nn::nex::JobManageAccount::CompleteJobImpl(nn::nex::CallContext::State state, nn::nex::qResult result)
{
    SetToComplete();
    CallContext* pContext = Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId);
    if (pContext != nullptr) {
        pContext->SetStateImpl(state, result, true);
    }
}

// 0x003810AC | fefates:bytes [tier B]
void nn::nex::JobManageAccount::CallCreateAccount()
{
    if (!IsCallContextRegistered(m_CallContextId)) {
        Cancel();
        return;
    }
    m_ProtocolCallContext.Reset();
    ResumeOnCallCompletion(&m_ProtocolCallContext, new Step(&JobManageAccount::CompleteJob, ""));
    m_ProtocolCallContext.m_pResult = m_pResult;
    bool isCalled = m_pProtocol->CallCreateAccount(&m_ProtocolCallContext, m_pResult, m_PrincipalName, m_Key.ToString(), m_Groups, m_Email);
    if (!isCalled) {
        CompleteJobImpl(CallContext::STATE_FAILURE, qResult(RESULT_CALL_FAILED));
    } else if (m_ProtocolCallContext.m_State == CallContext::STATE_CALL_IN_PROGRESS) {
        SetToSuspended();
    } else {
        SetStep(Step(&JobManageAccount::CompleteJob, ""));
    }
}

// 0x003811FC | fefates:bytes [tier B]
void nn::nex::JobManageAccount::CallUpdateAccount()
{
    if (!IsCallContextRegistered(m_CallContextId)) {
        Cancel();
        return;
    }
    m_ProtocolCallContext.Reset();
    ResumeOnCallCompletion(&m_ProtocolCallContext, new Step(&JobManageAccount::CompleteJob, ""));
    bool isCalled = m_pProtocol->CallUpdateAccount(&m_ProtocolCallContext, m_Key.ToString(), m_Email, m_Data1, m_Data2);
    if (!isCalled) {
        CompleteJobImpl(CallContext::STATE_FAILURE, qResult(RESULT_CALL_FAILED));
    } else if (m_ProtocolCallContext.m_State == CallContext::STATE_CALL_IN_PROGRESS) {
        SetToSuspended();
    } else {
        SetStep(Step(&JobManageAccount::CompleteJob, ""));
    }
}

// 0x00381340 (name is ours)
void nn::nex::JobManageAccount::CallChangePassword()
{
    if (!IsCallContextRegistered(m_CallContextId)) {
        Cancel();
        return;
    }
    m_ProtocolCallContext.Reset();
    ResumeOnCallCompletion(&m_ProtocolCallContext, new Step(&JobManageAccount::CompleteJob, ""));
    m_ProtocolCallContext.m_pIsSuccess = m_pIsSuccess;
    bool isCalled = m_pProtocol->CallChangePassword(&m_ProtocolCallContext, m_pIsSuccess, m_Key.ToString());
    if (!isCalled) {
        CompleteJobImpl(CallContext::STATE_FAILURE, qResult(RESULT_CALL_FAILED));
    } else if (m_ProtocolCallContext.m_State == CallContext::STATE_CALL_IN_PROGRESS) {
        SetToSuspended();
    } else {
        SetStep(Step(&JobManageAccount::CompleteJob, ""));
    }
}

// 0x00381484 | fefates:bytes [tier B]
void nn::nex::JobManageAccount::PasswordInitialHash()
{
    if (!IsCallContextRegistered(m_CallContextId)) {
        Cancel();
        return;
    }
    ResumeOnCallCompletion(&m_CallContext, new Step(&JobManageAccount::SelectCall, ""));
    bool isStarted = KerberosEncryption::InitializeKey(&m_CallContext, m_pPassword, &m_Key);
    if (!isStarted) {
        CompleteJobImpl(CallContext::STATE_FAILURE, qResult(RESULT_CALL_FAILED));
    } else if (m_CallContext.m_State == CallContext::STATE_CALL_IN_PROGRESS) {
        SetToSuspended();
    } else {
        SetStep(Step(&JobManageAccount::CompleteJob, ""));
    }
}

// 0x0038159C | fefates:bytes [tier B]
void nn::nex::JobManageAccount::CallCustomCreateAccount()
{
    if (!IsCallContextRegistered(m_CallContextId)) {
        Cancel();
        return;
    }
    m_ProtocolCallContext.Reset();
    ResumeOnCallCompletion(&m_ProtocolCallContext, new Step(&JobManageAccount::CompleteJob, ""));
    bool isCalled = m_pProtocol->CallCustomCreateAccount(&m_ProtocolCallContext, m_PrincipalName, m_Key.ToString(), m_Groups, m_Email, m_Data3, m_pPid);
    if (!isCalled) {
        CompleteJobImpl(CallContext::STATE_FAILURE, qResult(RESULT_CALL_FAILED));
    } else if (m_ProtocolCallContext.m_State == CallContext::STATE_CALL_IN_PROGRESS) {
        SetToSuspended();
    } else {
        SetStep(Step(&JobManageAccount::CompleteJob, ""));
    }
}

// 0x003816E8 (name is ours)
void nn::nex::JobManageAccount::CallChangePasswordByGuest()
{
    if (!IsCallContextRegistered(m_CallContextId)) {
        Cancel();
        return;
    }
    m_ProtocolCallContext.Reset();
    ResumeOnCallCompletion(&m_ProtocolCallContext, new Step(&JobManageAccount::CompleteJob, ""));
    bool isCalled = m_pProtocol->CallChangePasswordByGuest(&m_ProtocolCallContext, m_PrincipalName, m_Email, m_Key.ToString());
    if (!isCalled) {
        CompleteJobImpl(CallContext::STATE_FAILURE, qResult(RESULT_CALL_FAILED));
    } else if (m_ProtocolCallContext.m_State == CallContext::STATE_CALL_IN_PROGRESS) {
        SetToSuspended();
    } else {
        SetStep(Step(&JobManageAccount::CompleteJob, ""));
    }
}

// 0x0038182C | fefates:bytes [tier B]
void nn::nex::JobManageAccount::CallNintendoCreateAccount()
{
    if (!IsCallContextRegistered(m_CallContextId)) {
        Cancel();
        return;
    }
    m_ProtocolCallContext.Reset();
    ResumeOnCallCompletion(&m_ProtocolCallContext, new Step(&JobManageAccount::CompleteJob, ""));
    bool isCalled = m_pProtocol->CallNintendoCreateAccount(&m_ProtocolCallContext, m_PrincipalName, m_Key.ToString(), m_Groups, m_Email, m_Data3, m_pPid, m_pPidHMAC);
    if (!isCalled) {
        CompleteJobImpl(CallContext::STATE_FAILURE, qResult(RESULT_CALL_FAILED));
    } else if (m_ProtocolCallContext.m_State == CallContext::STATE_CALL_IN_PROGRESS) {
        SetToSuspended();
    } else {
        SetStep(Step(&JobManageAccount::CompleteJob, ""));
    }
}

// 0x00381988 | fefates:bytes [tier B]
void nn::nex::JobManageAccount::CallAccountManagementCommand()
{
    if (!IsCallContextRegistered(m_CallContextId)) {
        Cancel();
        return;
    }
    m_ProtocolCallContext.Reset();
    ResumeOnCallCompletion(&m_ProtocolCallContext, new Step(&JobManageAccount::CompleteJob, ""));
    bool isCalled = m_pCommand->Call(&m_ProtocolCallContext);
    if (!isCalled) {
        CompleteJobImpl(CallContext::STATE_FAILURE, qResult(RESULT_CALL_FAILED));
    } else if (m_ProtocolCallContext.m_State == CallContext::STATE_CALL_IN_PROGRESS) {
        SetToSuspended();
    } else {
        SetStep(Step(&JobManageAccount::CompleteJob, ""));
    }
}

// 0x00381AA4 (name is ours)
void nn::nex::JobManageAccount::SelectCall()
{
    if (!IsCallContextRegistered(m_CallContextId)) {
        Cancel();
        return;
    }
    if (m_CallContext.m_State != CallContext::STATE_SUCCESS) {
        SetStep(Step(&JobManageAccount::CompleteJob, ""));
        return;
    }
    if (m_pCommand != nullptr) {
        m_pCommand->m_Key = m_Key.ToString();
        SetStep(Step(&JobManageAccount::CallAccountManagementCommand, ""));
        return;
    }
    switch (m_FuncType) {
    case FUNC_CREATE_ACCOUNT:
        SetStep(Step(&JobManageAccount::CallCreateAccount, ""));
        break;
    case FUNC_CREATE_ACCOUNT_WITH_CUSTOM_DATA:
        SetStep(Step(&JobManageAccount::CallCreateAccountWithCustomData, ""));
        break;
    case FUNC_CHANGE_PASSWORD:
        SetStep(Step(&JobManageAccount::CallChangePassword, ""));
        break;
    case FUNC_UPDATE_ACCOUNT:
        SetStep(Step(&JobManageAccount::CallUpdateAccount, ""));
        break;
    case FUNC_CHANGE_PASSWORD_BY_GUEST:
        SetStep(Step(&JobManageAccount::CallChangePasswordByGuest, ""));
        break;
    case FUNC_CUSTOM_CREATE_ACCOUNT:
        SetStep(Step(&JobManageAccount::CallCustomCreateAccount, ""));
        break;
    case FUNC_NINTENDO_CREATE_ACCOUNT:
        SetStep(Step(&JobManageAccount::CallNintendoCreateAccount, ""));
        break;
    }
}

// 0x00381D04 | fefates:bytes [tier B]
void nn::nex::JobManageAccount::CallCreateAccountWithCustomData()
{
    if (!IsCallContextRegistered(m_CallContextId)) {
        Cancel();
        return;
    }
    m_ProtocolCallContext.Reset();
    ResumeOnCallCompletion(&m_ProtocolCallContext, new Step(&JobManageAccount::CompleteJob, ""));
    bool isCalled = m_pProtocol->CallCreateAccountWithCustomData(&m_ProtocolCallContext, m_PrincipalName, m_Key.ToString(), m_Groups, m_Email, m_Data1, m_Data2);
    if (!isCalled) {
        CompleteJobImpl(CallContext::STATE_FAILURE, qResult(RESULT_CALL_FAILED));
    } else if (m_ProtocolCallContext.m_State == CallContext::STATE_CALL_IN_PROGRESS) {
        SetToSuspended();
    } else {
        SetStep(Step(&JobManageAccount::CompleteJob, ""));
    }
}

// 0x00381E54 | fefates:bytes [tier B]
void nn::nex::JobManageAccount::Cancel()
{
    DeleteObject(m_Data1);
    DeleteObject(m_Data2);
    m_CallContext.Cancel(CallContext::STATE_CANCELLED);
    if (m_pCommand != nullptr) {
        delete m_pCommand;
        m_pCommand = nullptr;
    }
    SetToComplete();
}

// 0x00381ECC | fefates:callgraph [tier C]
nn::nex::JobManageAccount::JobManageAccount(unsigned int callContextId, nn::nex::AccountManagementProtocolClient* pProtocol, const nn::nex::String& principalName,
                                            const nn::nex::String& password, unsigned int groups, const nn::nex::String& email, nn::nex::qResult* pResult, FuncType funcType,
                                            DataHolder* pData1, DataHolder* pData2, DataHolder* pData3, unsigned int* pPid, nn::nex::String* pPidHMAC)
    : StepSequenceJob(static_cast<JobType>(2), DebugString()), m_CallContextId(callContextId), m_pProtocol(pProtocol), m_PrincipalName(principalName), m_Groups(groups),
      m_Email(email), m_FuncType(funcType), m_pPid(pPid), m_pPidHMAC(pPidHMAC), m_pResult(pResult), m_pIsSuccess(nullptr), m_pCommand(nullptr)
{
    password.CreateCopy(&m_pPassword);
    if (pData1 != nullptr && pData1->m_pObject != nullptr) {
        m_Data1.Reset(CloneObject(pData1));
    }
    if (pData2 != nullptr && pData2->m_pObject != nullptr) {
        m_Data2.Reset(CloneObject(pData2));
    }
    if (pData3 != nullptr && pData3->m_pObject != nullptr) {
        m_Data3.Reset(CloneObject(pData3));
    } else {
        m_Data3.Reset(new Data());
    }
    SetStep(Step(&JobManageAccount::PasswordInitialHash, ""));
}

// 0x003820F8 slot 0x00 | fefates:bytes
// 0x003820E8 (deleting dtor)
nn::nex::JobManageAccount::~JobManageAccount()
{
    String::ReleaseCopy(m_pPassword);
    DeleteObject(m_Data1);
    DeleteObject(m_Data2);
    m_CallContext.Cancel(CallContext::STATE_CANCELLED);
    if (m_pCommand != nullptr) {
        delete m_pCommand;
        m_pCommand = nullptr;
    }
}

} // namespace nex
} // namespace nn
