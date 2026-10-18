#pragma once

#include "decomp.h"
#include "nn/nex/nex_DataStoreRatingInfo.h"
#include "nn/nex/nex_DataStoreRatingInfoWithSlot.h"
#include "nn/nex/nex_NonCopyable.h"
#include "nn/nex/nex_ProtocolCallContext.h"
#include "nn/nex/nex_StepSequenceJob.h"
#include "nn/nex/nex_qMap.h"
#include "nn/nex/nex_qResult.h"
#include "nn/nex/nex_qVector.h"

namespace nn {
namespace nex {
class DataStoreLogicServerClientInterface;

// The ratings of DataStore objects (GetRatings), converted into maps by slot. The names of the
// members and of the steps without a reference name are ours.
// RTTI N2nn3nex21JobDataStoreGetRatingE @ 0x008CEAEC
// vtable 0x008FDDF0 (vptr 0x008FDDF8), offset_to_top 0, 15 entries
class JobDataStoreGetRating : public ::nn::nex::StepSequenceJob, public ::nn::nex::NonCopyable
{
public:
    JobDataStoreGetRating(); // 0x00399C0C | fefates:bytes [tier B]
    virtual ~JobDataStoreGetRating(); // 0x00399D1C slot 0x00
    // 0x00399C88 (deleting dtor) | fefates:bytes
    // the ratings of one object
    virtual nn::nex::qResult Initialize(unsigned int callContextId, nn::nex::DataStoreLogicServerClientInterface* pClient, const unsigned long long& dataId,
                                        nn::nex::qMap<signed char, nn::nex::DataStoreRatingInfo>* pRatings, unsigned long long accessPassword); // 0x00398C94 slot 0x34 | fefates:callseq
    // the ratings of several objects
    virtual nn::nex::qResult Initialize(unsigned int callContextId, nn::nex::DataStoreLogicServerClientInterface* pClient, const nn::nex::qVector<u64>& dataIds,
                                        nn::nex::qVector<nn::nex::qMap<signed char, nn::nex::DataStoreRatingInfo> >* pRatings,
                                        nn::nex::qVector<nn::nex::qResult>* pResults); // 0x00398BAC slot 0x38
    void CompleteJob(const nn::nex::qResult& result); // 0x00398DC8 | fefates:bytes [tier B]
    void StepGetRatings(); // 0x0039906C (name is ours)
    void StepConvertType(); // 0x00399194 | fefates:bytes [tier B]

    u32 m_CallContextId;                                          // 0x068
    DataStoreLogicServerClientInterface* m_pClient;               // 0x06C
    qVector<u64> m_DataIds;                                       // 0x070
    qVector<qVector<DataStoreRatingInfoWithSlot> > m_Ratings;     // 0x07C
    qVector<qMap<s8, DataStoreRatingInfo> >* m_pRatingMaps;       // 0x088
    qMap<s8, DataStoreRatingInfo>* m_pRatingMap;                  // 0x08C
    qVector<qResult>* m_pResults;                                 // 0x090
    qVector<qResult> m_Results;                                   // 0x094
    bool m_IsSingle;                                              // 0x0A0
    ProtocolCallContext m_ProtocolCallContext;                    // 0x0A8
    u64 m_AccessPassword;                                         // 0x120
};
ASSERT_OFFSET(JobDataStoreGetRating, m_ProtocolCallContext, 0xA8);
ASSERT_SIZE(JobDataStoreGetRating, 0x128);
} // namespace nex
} // namespace nn
