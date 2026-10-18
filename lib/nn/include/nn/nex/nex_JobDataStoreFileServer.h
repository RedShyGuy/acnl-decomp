#pragma once

#include "decomp.h"
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_HttpClient.h"
#include "nn/nex/nex_HttpConnection.h"
#include "nn/nex/nex_HttpEventListener.h"
#include "nn/nex/nex_NonCopyable.h"
#include "nn/nex/nex_StepSequenceJob.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_qVector.h"

namespace nn {
namespace nex {
// An HTTP request to the DataStore file server: the root CAs of the file server, redirects and
// retries, and the body of an error. The listener of the caller gets the request and the response.
// The names of the members are ours.
// RTTI N2nn3nex22JobDataStoreFileServerE @ 0x008CEBBC
// vtable 0x008FE024 (vptr 0x008FE02C), offset_to_top 0, 18 entries
// vtable 0x008FE074 (vptr 0x008FE07C), offset_to_top -104, 6 entries
class JobDataStoreFileServer : public ::nn::nex::StepSequenceJob, public ::nn::nex::NonCopyable, public ::nn::nex::HttpEventListener
{
public:
    // the root CAs of the file server
    static const u32 ROOT_CA_COUNT = 4;
    static const char* s_RootCaNames[ROOT_CA_COUNT];

    // (inline in DataStoreFileServerClient::ApiImpl)
    JobDataStoreFileServer()
        : StepSequenceJob(static_cast<JobType>(2), DebugString()), m_CallContextId(0), m_URL(L""), m_Method(0), m_pListener(nullptr), m_RedirectCount(0), m_StatusCode(0),
          m_IsTooLarge(false), m_Location(L""), m_ChunkSize(0)
    {
    }
    virtual ~JobDataStoreFileServer(); // 0x0039CEB0 slot 0x00
    // 0x0039CE40 (deleting dtor)
    virtual nn::nex::qResult Initialize(unsigned int callContextId, const nn::nex::String& url, nn::nex::HttpConnection::Method method, nn::nex::HttpEventListener* pListener,
                                        unsigned int chunkSize); // 0x0039C4D4 slot 0x34 | fefates:callseq
    virtual void PrepareRequest(nn::nex::HttpConnection* pConnection); // 0x0039C6B8 slot 0x38
    virtual unsigned int PostChunkedBuffer(nn::nex::HttpConnection* pConnection, unsigned char* pBuffer, unsigned int size, bool* pIsLast); // 0x0039C88C slot 0x3C
    virtual void ProcessResponseHeader(nn::nex::HttpConnection* pConnection, int statusCode); // 0x0039CC00 slot 0x40
    virtual bool ProcessResponse(const unsigned char* pData, unsigned int size); // 0x0039C79C slot 0x44
    void CompleteJob(const nn::nex::qResult& result); // 0x0039C638 | fefates:bytes [tier B]
    void StepWaitingForHttp(); // 0x0039C924 | fefates:bytes [tier B]
    void StepHttp(); // 0x0039CCBC | fefates:bytes [tier B]

    u32 m_CallContextId;            // 0x06C
    String m_URL;                   // 0x070
    u8 m_Method;                    // 0x078
    HttpEventListener* m_pListener; // 0x07C
    CallContext m_CallContext;      // 0x080 (of the request)
    HttpClient m_HttpClient;        // 0x0E0
    u32 m_RedirectCount;            // 0x0E4 (the requests)
    s32 m_StatusCode;               // 0x0E8
    qVector<u8> m_ErrorBody;        // 0x0EC (the body of an error response)
    bool m_IsTooLarge;              // 0x0F8 (the error body)
    String m_Location;              // 0x0FC (of a redirect)
    u32 m_ChunkSize;                // 0x104
    qVector<u8> m_RootCas[ROOT_CA_COUNT]; // 0x108
};
ASSERT_OFFSET(JobDataStoreFileServer, m_CallContext, 0x80);
ASSERT_OFFSET(JobDataStoreFileServer, m_RootCas, 0x108);
ASSERT_SIZE(JobDataStoreFileServer, 0x138);
} // namespace nex
} // namespace nn
