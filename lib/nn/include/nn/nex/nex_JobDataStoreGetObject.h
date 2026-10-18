#pragma once

#include "decomp.h"
#include "nn/nex/nex_DataStoreApiVersionTraits.h"
#include "nn/nex/nex_DataStoreGetObjectEventListenerTemplate.h"
#include "nn/nex/nex_DataStoreReqGetAdditionalMeta.h"
#include "nn/nex/nex_HttpConnection.h"
#include "nn/nex/nex_HttpEventListener.h"
#include "nn/nex/nex_NonCopyable.h"
#include "nn/nex/nex_ProtocolCallContext.h"
#include "nn/nex/nex_StepSequenceJob.h"
#include "nn/nex/nex_qResult.h"

namespace nn {
namespace nex {
class DataStoreFileServerClient;
class DataStoreLogicServerClientInterface;

// Gets a DataStore object: PrepareGetObject on the logic server (or the request of the caller), then
// the data from the file server, or the meta binary of the object (with getsMeta and no URL). The
// data goes to the listener of the caller or into its buffer. The names of the members and of the
// steps without a reference symbol are ours.
// Instantiations found in the binary:
//   nn::nex::JobDataStoreGetObject<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >  typeinfo 0x008CEA9C  vtable 0x008FDD18
//   nn::nex::JobDataStoreGetObject<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >  typeinfo 0x008CEAC4  vtable 0x008FDD84
template <typename Traits>
class JobDataStoreGetObject : public ::nn::nex::StepSequenceJob, public ::nn::nex::NonCopyable, public ::nn::nex::HttpEventListener
{
public:
    typedef typename Traits::TDataId TDataId;
    typedef typename Traits::TPrepareGetParam TPrepareGetParam;
    typedef typename Traits::TReqGetInfo TReqGetInfo;
    typedef DataStoreGetObjectEventListenerTemplate<TDataId> Listener;

    JobDataStoreGetObject();
    virtual ~JobDataStoreGetObject();
    virtual nn::nex::qResult Initialize(unsigned int callContextId, nn::nex::DataStoreLogicServerClientInterface* pClient, const TPrepareGetParam& param,
                                        unsigned int threadPriority, bool usesHttps, unsigned int httpBufferSize, unsigned int minBytesPerSecond, unsigned int minTimeout,
                                        Listener* pListener, unsigned int chunkSize, unsigned char* pBuffer, unsigned int bufferSize, bool getsMeta, unsigned int* pSize,
                                        const TReqGetInfo* pReqGetInfo); // slot 0x34
    virtual void PrepareRequest(nn::nex::HttpConnection* pConnection); // slot 0x38
    virtual void ProcessResponseHeader(nn::nex::HttpConnection* pConnection, int statusCode); // slot 0x3C
    virtual bool ProcessResponse(const unsigned char* pData, unsigned int size); // slot 0x40
    void CompleteJob(const nn::nex::qResult& result);
    void StepLogicServerPrepareGetObject();
    void StepWaitingLogicServerPrepareGetObject();
    void StepFileServerGetObject();
    void StepWaitingFileServerGetObject();
    void StepProcessMetaBinary();

    u32 m_CallContextId;                            // 0x06C
    DataStoreLogicServerClientInterface* m_pClient; // 0x070
    DataStoreFileServerClient* m_pFileServerClient; // 0x074
    bool m_UsesHttps;                               // 0x078
    u32 m_HttpBufferSize;                           // 0x07C
    u32 m_ThreadPriority;                           // 0x080
    u32 m_MinBytesPerSecond;                        // 0x084 (0: no timeout)
    u32 m_MinTimeout;                               // 0x088 (ms)
    Listener* m_pListener;                          // 0x08C
    u32 m_ChunkSize;                                // 0x090
    ProtocolCallContext m_ProtocolCallContext;      // 0x098
    TPrepareGetParam m_Param;                       // 0x110
    TReqGetInfo m_ReqGetInfo;
    DataStoreReqGetAdditionalMeta m_AdditionalMeta;
    u8* m_pBuffer;
    u32 m_BufferSize;
    bool m_GetsMeta;
    u32* m_pSize; // (of the caller)
    u32 m_ReceivedSize;
    bool m_IsBufferTooSmall;
    bool m_IsSizeMismatch; // (Content-Length is not the size of the object)
    bool m_IsFirst;        // (no data yet)
};
} // namespace nex
} // namespace nn
