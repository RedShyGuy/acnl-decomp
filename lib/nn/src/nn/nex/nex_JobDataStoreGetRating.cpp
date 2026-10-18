#include "nn/nex/nex_JobDataStoreGetRating.h"
#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_DataStoreLogicServerClientInterface.h"
#include "nn/nex/nex_Scheduler.h"

namespace nn {
namespace nex {
namespace {
// the results (the names are ours)
const int RESULT_SUCCESS = qResult::SUCCESS;
const int RESULT_CANCELLED = static_cast<int>(0x80010004);
const int RESULT_CALL_FAILED = static_cast<int>(0x8001000D);
const int RESULT_INVALID_RESULTS = static_cast<int>(0x80690001);

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

// (inline) the ratings of an object by slot
inline void ConvertRatings(qMap<s8, DataStoreRatingInfo>* pMap, const qVector<DataStoreRatingInfoWithSlot>& ratings)
{
    for (const DataStoreRatingInfoWithSlot* p = ratings.begin(); p != ratings.end(); p++) {
        pMap->insert(std::pair<const s8, DataStoreRatingInfo>(p->m_Slot, p->m_Rating));
    }
}
} // namespace

// 0x00398BAC slot 0x38
nn::nex::qResult nn::nex::JobDataStoreGetRating::Initialize(unsigned int callContextId, nn::nex::DataStoreLogicServerClientInterface* pClient,
                                                           const nn::nex::qVector<u64>& dataIds,
                                                           nn::nex::qVector<nn::nex::qMap<signed char, nn::nex::DataStoreRatingInfo> >* pRatings,
                                                           nn::nex::qVector<nn::nex::qResult>* pResults)
{
    ScopedCS lock(GetSchedulerLock());
    m_CallContextId = callContextId;
    m_DataIds = dataIds;
    m_pClient = pClient;
    m_pRatingMaps = pRatings;
    m_pResults = pResults;
    m_ProtocolCallContext.SetCredentials(static_cast<ProtocolCallContext*>(GetCallerContext(m_CallContextId))->m_pCredentials);
    SetStep(Step(&JobDataStoreGetRating::StepGetRatings, ""));
    bool isSuccess = true;
    return qResult(isSuccess);
}

// 0x00398C94 slot 0x34 | fefates:callseq
nn::nex::qResult nn::nex::JobDataStoreGetRating::Initialize(unsigned int callContextId, nn::nex::DataStoreLogicServerClientInterface* pClient, const unsigned long long& dataId,
                                                           nn::nex::qMap<signed char, nn::nex::DataStoreRatingInfo>* pRatings, unsigned long long accessPassword)
{
    ScopedCS lock(GetSchedulerLock());
    m_CallContextId = callContextId;
    m_DataIds.push_back(dataId);
    m_pClient = pClient;
    m_pRatingMap = pRatings;
    m_pResults = nullptr;
    m_IsSingle = true;
    m_AccessPassword = accessPassword;
    m_ProtocolCallContext.SetCredentials(static_cast<ProtocolCallContext*>(GetCallerContext(m_CallContextId))->m_pCredentials);
    SetStep(Step(&JobDataStoreGetRating::StepGetRatings, ""));
    bool isSuccess = true;
    return qResult(isSuccess);
}

// 0x00398DC8 | fefates:bytes [tier B]
void nn::nex::JobDataStoreGetRating::CompleteJob(const nn::nex::qResult& result)
{
    CallContext* pContext = GetCallerContext(m_CallContextId);
    if (pContext != nullptr) {
        if (result) {
            if (m_pResults != nullptr) {
                *m_pResults = m_Results;
            }
            pContext->SignalSuccess(result);
        } else {
            pContext->SignalFailure(result);
        }
    } else {
        m_ProtocolCallContext.Cancel(CallContext::STATE_CANCELLED);
    }
    SetToComplete();
}

// 0x0039906C (name is ours)
void nn::nex::JobDataStoreGetRating::StepGetRatings()
{
    if (GetCallerContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    m_ProtocolCallContext.Reset();
    if (!m_pClient->GetRatings(&m_ProtocolCallContext, m_DataIds, &m_Ratings, &m_Results, m_AccessPassword)) {
        CompleteJob(qResult(RESULT_CALL_FAILED));
        return;
    }
    Core::GetInstance()->m_pCallContextRegister->AssociateCallContext(m_CallContextId, m_ProtocolCallContext.m_Id);
    SetToSuspended();
    ResumeOnCallCompletion(&m_ProtocolCallContext, new Step(&JobDataStoreGetRating::StepConvertType, ""));
}

// 0x00399194 | fefates:bytes [tier B]
void nn::nex::JobDataStoreGetRating::StepConvertType()
{
    if (GetCallerContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    if (m_ProtocolCallContext.m_State != CallContext::STATE_SUCCESS) {
        CompleteJob(m_ProtocolCallContext.m_Result);
        return;
    }
    if (m_IsSingle) {
        if (m_Results.size() != 1) {
            CompleteJob(qResult(RESULT_INVALID_RESULTS));
            return;
        }
        if (!m_Results[0]) {
            CompleteJob(m_Results[0]);
            return;
        }
    }
    if (m_pRatingMap != nullptr && m_Ratings.size() != 0) {
        m_pRatingMap->clear();
        ConvertRatings(m_pRatingMap, m_Ratings[0]);
    }
    if (m_pRatingMaps != nullptr) {
        m_pRatingMaps->clear();
        for (u32 i = 0; i < m_Ratings.size(); i++) {
            qMap<s8, DataStoreRatingInfo> ratings;
            ConvertRatings(&ratings, m_Ratings[i]);
            m_pRatingMaps->push_back(ratings);
        }
    }
    CompleteJob(qResult(RESULT_SUCCESS));
}

// 0x00399C0C | fefates:bytes [tier B]
nn::nex::JobDataStoreGetRating::JobDataStoreGetRating()
    : StepSequenceJob(static_cast<JobType>(2), DebugString()), m_CallContextId(0), m_pClient(nullptr), m_pRatingMaps(nullptr), m_pRatingMap(nullptr), m_pResults(nullptr),
      m_IsSingle(false), m_AccessPassword(0)
{
    // nothing to do
}

// 0x00399D1C slot 0x00
// 0x00399C88 (deleting dtor) | fefates:bytes
nn::nex::JobDataStoreGetRating::~JobDataStoreGetRating()
{
    // nothing to do
}

} // namespace nex
} // namespace nn
