#pragma once

#include "decomp.h"
#include "nn/nex/nex_DataStorePrepareUpdateParam.h"
#include "nn/nex/nex_DataStoreReqUpdateInfo.h"
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

// The update of a DataStore object: PrepareUpdateObject at the logic server, the post of the data
// to the file server, then CompleteUpdateObject. The names of the members are ours.
// RTTI N2nn3nex24JobDataStoreUpdateObjectE @ 0x008CEDB0
// vtable 0x008FE7EC (vptr 0x008FE7F4), offset_to_top 0, 14 entries
class JobDataStoreUpdateObject : public ::nn::nex::StepSequenceJob, public ::nn::nex::NonCopyable
{
public:
    JobDataStoreUpdateObject(); // 0x003B4204 | mk7dlp:callseq [tier A]
    virtual ~JobDataStoreUpdateObject(); // 0x003B4378 slot 0x00 | fefates:bytes
    // 0x003B4368 (deleting dtor)
    // minBytesPerSecond: the timeout of the post is the size by this (at least minTimeout ms; 0: none)
    virtual nn::nex::qResult Initialize(unsigned int callContextId, nn::nex::DataStoreLogicServerClientInterface* pClient, const nn::nex::DataStorePrepareUpdateParam& param,
                                        unsigned int threadPriority, bool usesHttps, unsigned int bufferSize, unsigned int minBytesPerSecond, unsigned int minTimeout,
                                        nn::nex::DataStorePostObjectEventListener* pListener, unsigned int chunkSize, const unsigned char* pData,
                                        bool copiesData); // 0x003B3980 slot 0x34
    void CompleteJob(const nn::nex::qResult& result); // 0x003B3B08 | fefates:bytes [tier B]
    void StepFileServerPostObject(); // 0x003B3B88 | fefates:bytes [tier B]
    void StepLogicServerPrepareUpdateObject(); // 0x003B3EA4 | fefates:bytes [tier B]
    void StepLogicServerCompleteUpdateObject(); // 0x003B3FB8 | fefates:bytes [tier B]
    void StepWaitingLogicServerCompleteUpdateObject(); // 0x003B4130 | fefates:bytes [tier B]

    u32 m_CallContextId;                             // 0x068
    DataStoreLogicServerClientInterface* m_pClient;  // 0x06C
    DataStoreFileServerClient* m_pFileServerClient;  // 0x070
    bool m_UsesHttps;                                // 0x074
    u32 m_BufferSize;                                // 0x078
    u32 m_ThreadPriority;                            // 0x07C
    u32 m_MinBytesPerSecond;                         // 0x080
    u32 m_MinTimeout;                                // 0x084 (ms)
    DataStorePostObjectEventListener* m_pListener;   // 0x088
    u32 m_ChunkSize;                                 // 0x08C
    ProtocolCallContext m_ProtocolCallContext;       // 0x090
    DataStorePrepareUpdateParam m_Param;             // 0x108
    DataStoreReqUpdateInfo m_ReqUpdateInfo;          // 0x138
    u8* m_pDataCopy;                                 // 0x170
    const u8* m_pData;                               // 0x174
    qResult m_PostResult;                            // 0x178 (of the post to the file server)
    DataStorePostHttpEventListener* m_pHttpListener; // 0x184
};
ASSERT_OFFSET(JobDataStoreUpdateObject, m_Param, 0x108);
ASSERT_OFFSET(JobDataStoreUpdateObject, m_ReqUpdateInfo, 0x138);
ASSERT_SIZE(JobDataStoreUpdateObject, 0x188);
} // namespace nex
} // namespace nn
