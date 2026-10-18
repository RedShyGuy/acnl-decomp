#include "nn/nex/nex_JobRetrieveGameAuthToken.h"
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_KerberosEncryption.h"
#include "nn/nex/nex_Key.h"
#include "nn/nex/nex_Scheduler.h"

#include <cstring>

namespace nn {
namespace nex {
// the key of the last principal ID (globals of the DataStore file, after g_HppSettings)
// 0x0095F5F0 (name is ours)
u32 g_HppKeyPrincipalId = 0;
// 0x00ADD900 (name is ours)
char g_HppKey[64];

namespace {
// the results (the names are ours)
const int RESULT_SUCCESS = qResult::SUCCESS;
const int RESULT_CANCELLED = static_cast<int>(0x80010004);

// the lock of the scheduler
inline CriticalSection& GetSchedulerLock()
{
    Core* pCore = Core::GetInstance();
    return (pCore != nullptr ? pCore->m_pScheduler : nullptr)->m_Lock;
}

// (inline) the call context of the caller
inline CallContext* GetCallerContext(u32 id)
{
    return Core::GetInstance()->m_pCallContextRegister->GetCallContext(id);
}
} // namespace

// 0x003B4458 | fefates:bytes [tier B]
nn::nex::qResult nn::nex::JobRetrieveGameAuthToken::Initialize(unsigned int callContextId, unsigned int principalId, const nn::nex::String& password,
                                                              nn::nex::String* pToken, nn::nex::String* pKey)
{
    ScopedCS lock(GetSchedulerLock());
    m_CallContextId = callContextId;
    m_PrincipalId = principalId;
    m_Password = password;
    m_pKey = pKey;
    *pToken = L"normaltoken";
    if (g_HppKeyPrincipalId == principalId && strncmp(g_HppKey, "", sizeof(g_HppKey)) != 0) {
        // the key is cached
        *m_pKey = g_HppKey;
        SetStep(Step(&JobRetrieveGameAuthToken::StepDummyToComplete, ""));
    } else {
        SetStep(Step(&JobRetrieveGameAuthToken::StepDeriveKey, ""));
    }
    bool isSuccess = true;
    return qResult(isSuccess);
}

// 0x003B45B8 | fefates:bytes [tier B]
void nn::nex::JobRetrieveGameAuthToken::CompleteJob(const nn::nex::qResult& result)
{
    CallContext* pContext = GetCallerContext(m_CallContextId);
    if (pContext != nullptr) {
        if (result) {
            char* pKey = nullptr;
            m_pKey->CreateCopy(&pKey);
            strncpy(g_HppKey, pKey, sizeof(g_HppKey));
            String::ReleaseCopy(pKey);
            pContext->SignalSuccess(result);
        } else {
            pContext->SignalFailure(result);
        }
    }
    SetToComplete();
}

// 0x003B465C | fefates:bytes [tier B]
void nn::nex::JobRetrieveGameAuthToken::StepDeriveKey()
{
    if (GetCallerContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    char* pPassword = nullptr;
    m_Password.CreateCopy(&pPassword);
    *m_pKey = KerberosEncryption::CreateKey(m_PrincipalId, pPassword).ToString();
    String::ReleaseCopy(pPassword);
    g_HppKeyPrincipalId = m_PrincipalId;
    CompleteJob(qResult(RESULT_SUCCESS));
}

// 0x003B473C | fefates:bytes [tier B]
void nn::nex::JobRetrieveGameAuthToken::StepDummyToComplete()
{
    if (GetCallerContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    CompleteJob(qResult(RESULT_SUCCESS));
}

// 0x003B47DC slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
// 0x003B47B8 (deleting dtor)
nn::nex::JobRetrieveGameAuthToken::~JobRetrieveGameAuthToken()
{
    // nothing to do
}

} // namespace nex
} // namespace nn
