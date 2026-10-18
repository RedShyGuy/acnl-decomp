#include "nn/nex/nex_IndependentServer.h"
#include "nn/friends/CTR/detail/detail_Api.h"
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_JobIndependentServer.h"
#include "nn/nex/nex_Scheduler.h"

namespace nn {
namespace nex {
namespace {
// an argument is missing
const int RESULT_INVALID_ARGUMENT = static_cast<int>(0x8001000A);
// the friends module is not initialized
const int RESULT_NOT_INITIALIZED = static_cast<int>(0x80650000);
// the call could not be sent
const int RESULT_CALL_FAILED = static_cast<int>(0x8001000D);
} // namespace

// 0x00386D70 (name is ours)
nn::nex::qResult nn::nex::IndependentServer::RequestServiceToken(nn::nex::CallContext* pContext, unsigned int gameServerId, const char* pAccessKey, char* pToken,
                                                                 bool checksRestriction)
{
    // (the debug build warns about the game server ids of the sample servers)
    if (pContext == nullptr || pAccessKey == nullptr || pToken == nullptr) {
        return qResult(RESULT_INVALID_ARGUMENT);
    }
    if (!nn::friends::CTR::detail::IsInitialized()) {
        return qResult(RESULT_NOT_INITIALIZED);
    }
    if (!pContext->InitiateCall()) {
        return qResult(RESULT_CALL_FAILED);
    }
    JobIndependentServer* pJob = new JobIndependentServer();
    pJob->m_CallContextId = pContext->m_Id;
    pJob->m_pServer = this;
    pJob->m_GameServerId = gameServerId;
    pJob->m_pAccessKey = pAccessKey;
    pJob->m_pToken = pToken;
    m_LastErrorCode = 0;
    pJob->m_ChecksRestriction = checksRestriction;
    pJob->SetStep(StepSequenceJob::Step(&JobIndependentServer::Start, ""));
    Scheduler::GetInstanceInline()->Queue(pJob, false);
    return qResult(qResult::SUCCESS);
}

// 0x00386F30
nn::nex::IndependentServer::IndependentServer() : m_LastErrorCode(0)
{
    Core::AcquireInstance();
}

// 0x00386F78 slot 0x00
// 0x00386F54 (deleting dtor)
nn::nex::IndependentServer::~IndependentServer()
{
    Core::ReleaseInstance();
}

} // namespace nex
} // namespace nn
