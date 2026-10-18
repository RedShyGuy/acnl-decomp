#include "nn/nex/nex_RetrieveGameAuthTokenClient.h"
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_JobRetrieveGameAuthToken.h"
#include "nn/nex/nex_Scheduler.h"

namespace nn {
namespace nex {
// 0x003BD2EC | fefates:bytes [tier B]
nn::nex::qResult nn::nex::RetrieveGameAuthTokenClient::RetrieveGameAuthToken(nn::nex::CallContext* pContext, unsigned int principalId, const nn::nex::String& password,
                                                                           nn::nex::String* pToken, nn::nex::String* pKey)
{
    const int RESULT_SUCCESS = qResult::SUCCESS;
    const int RESULT_CALL_FAILED = static_cast<int>(0x8001000D);
    if (!pContext->InitiateCall()) {
        return qResult(RESULT_CALL_FAILED);
    }
    JobRetrieveGameAuthToken* pJob = new JobRetrieveGameAuthToken();
    qResult result = pJob->Initialize(pContext->m_Id, principalId, password, pToken, pKey);
    if (!result) {
        pJob->ReleaseRef();
        return result;
    }
    // (the scheduler takes the reference of the job)
    Core* pCore = Core::GetInstance();
    (pCore != nullptr ? pCore->m_pScheduler : nullptr)->Queue(pJob, false);
    return qResult(RESULT_SUCCESS);
}

} // namespace nex
} // namespace nn
