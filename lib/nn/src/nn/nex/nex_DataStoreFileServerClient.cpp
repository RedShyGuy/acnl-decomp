#include "nn/nex/nex_DataStoreFileServerClient.h"
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_JobDataStoreFileServer.h"
#include "nn/nex/nex_Scheduler.h"

namespace nn {
namespace nex {
// 0x003B5FA8 | fefates:bytes [tier B]
nn::nex::qResult nn::nex::DataStoreFileServerClient::ApiImpl(nn::nex::CallContext* pContext, const nn::nex::String& url, nn::nex::HttpEventListener* pListener,
                                                            nn::nex::HttpConnection::Method method, unsigned int chunkSize)
{
    const int RESULT_SUCCESS = qResult::SUCCESS;
    const int RESULT_CALL_FAILED = static_cast<int>(0x8001000D);
    if (!pContext->InitiateCall()) {
        return qResult(RESULT_CALL_FAILED);
    }
    JobDataStoreFileServer* pJob = new JobDataStoreFileServer();
    qResult result = pJob->Initialize(pContext->m_Id, url, method, pListener, chunkSize);
    if (!result) {
        pJob->ReleaseRef();
        return result;
    }
    // (the scheduler takes the reference of the job)
    Core* pCore = Core::GetInstance();
    (pCore != nullptr ? pCore->m_pScheduler : nullptr)->Queue(pJob, false);
    return qResult(RESULT_SUCCESS);
}

// 0x003B6188 slot 0x00
// 0x003B6184 (deleting dtor)
nn::nex::DataStoreFileServerClient::~DataStoreFileServerClient()
{
    // nothing to do
}

} // namespace nex
} // namespace nn
