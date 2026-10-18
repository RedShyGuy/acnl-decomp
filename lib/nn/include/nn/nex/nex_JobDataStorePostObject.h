#pragma once

#include "decomp.h"
#include "nn/nex/nex_DataStoreApiVersionTraits.h"
#include "nn/nex/nex_NonCopyable.h"
#include "nn/nex/nex_ProtocolCallContext.h"
#include "nn/nex/nex_StepSequenceJob.h"
#include "nn/nex/nex_qResult.h"

namespace nn {
namespace nex {
class DataStoreFileServerClient;
class DataStoreLogicServerClientInterface;
class DataStorePostHttpEventListener;
class DataStorePostObjectEventListener;

// Posts a DataStore object: PreparePostObject on the logic server, the data to the file server (from
// memory or from the listener of the caller), then CompletePostObject with the result of the post
// (not with the flag 0x40 of the parameters). The names of the members and of the steps without a
// reference symbol are ours.
// Instantiations found in the binary:
//   nn::nex::JobDataStorePostObject<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >  typeinfo 0x008CEBE4  vtable 0x008FE094
//   nn::nex::JobDataStorePostObject<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >  typeinfo 0x008CEC04  vtable 0x008FE0D4
template <typename Traits>
class JobDataStorePostObject : public ::nn::nex::StepSequenceJob, public ::nn::nex::NonCopyable
{
public:
    typedef typename Traits::TPreparePostParam TPreparePostParam;
    typedef typename Traits::TPreparePostParam::TDataId TDataId;
    typedef typename Traits::TReqPostInfo TReqPostInfo;
    typedef typename Traits::TCompletePostParam TCompletePostParam;

    JobDataStorePostObject();
    virtual ~JobDataStorePostObject();
    virtual nn::nex::qResult Initialize(unsigned int callContextId, nn::nex::DataStoreLogicServerClientInterface* pClient, const TPreparePostParam& param,
                                        unsigned int threadPriority, bool usesHttps, unsigned int bufferSize, unsigned int minBytesPerSecond, unsigned int minTimeout,
                                        TDataId* pDataId, nn::nex::DataStorePostObjectEventListener* pListener, unsigned int chunkSize, const unsigned char* pData,
                                        bool copiesData); // slot 0x34
    void CompleteJob(const nn::nex::qResult& result);
    void StepLogicServerPreparePostObject();
    void StepFileServerPostObject();
    void StepLogicServerCompletePostObject();
    void StepWaitingLogicServerCompletePostObject();

    u32 m_CallContextId;                               // 0x068
    DataStoreLogicServerClientInterface* m_pClient;    // 0x06C
    DataStoreFileServerClient* m_pFileServerClient;    // 0x070
    bool m_UsesHttps;                                  // 0x074
    u32 m_BufferSize;                                  // 0x078
    u32 m_ThreadPriority;                              // 0x07C
    u32 m_MinBytesPerSecond;                           // 0x080 (0: no timeout)
    u32 m_MinTimeout;                                  // 0x084 (ms)
    DataStorePostObjectEventListener* m_pListener;     // 0x088
    u32 m_ChunkSize;                                   // 0x08C
    ProtocolCallContext m_ProtocolCallContext;         // 0x090
    TPreparePostParam m_Param;                         // 0x108
    TDataId* m_pDataId;                                // (of the caller)
    TReqPostInfo m_ReqPostInfo;
    u8* m_pDataCopy;
    const u8* m_pData;
    qResult m_PostResult;
    DataStorePostHttpEventListener* m_pHttpListener;
};
} // namespace nex
} // namespace nn
