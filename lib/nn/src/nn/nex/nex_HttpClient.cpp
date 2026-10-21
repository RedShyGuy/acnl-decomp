#include "nn/nex/nex_HttpClient.h"
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_JobHttp.h"
#include "nn/nex/nex_Scheduler.h"

namespace nn {
namespace nex {
namespace {
// the call could not be started (the name is ours)
const int RESULT_CALL_FAILED = static_cast<int>(0x8001000D);
} // namespace

// 0x003550C0
nn::nex::HttpClient::HttpClient()
{
    // nothing to do
}

// 0x003550D0 slot 0x00
// 0x002F8500 (deleting dtor)
nn::nex::HttpClient::~HttpClient()
{
    // nothing to do
}

// 0x00354F30 | fefates:bytes-fuzzy [tier B]
nn::nex::qResult nn::nex::HttpClient::Start(nn::nex::CallContext* pContext, const nn::nex::String& url, nn::nex::HttpConnection::Method method,
                                            nn::nex::HttpEventListener* pListener, unsigned int chunkSize)
{
    if (!pContext->InitiateCall()) {
        return qResult(RESULT_CALL_FAILED);
    }
    JobHttp* pJob = new JobHttp();
    qResult result = pJob->Initialize(pContext->m_Id, url, method, pListener, chunkSize);
    if (!(result.m_Code >= 0)) {
        pJob->ReleaseRef();
        return qResult(RESULT_CALL_FAILED);
    }
    Core* pCore = Core::GetInstance();
    (pCore != nullptr ? pCore->m_pScheduler : nullptr)->Queue(pJob, false);
    return qResult(qResult::SUCCESS);
}

} // namespace nex
} // namespace nn
