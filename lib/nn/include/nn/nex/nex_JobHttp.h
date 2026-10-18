#pragma once

#include "decomp.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_HttpConnection.h"
#include "nn/nex/nex_NonCopyable.h"
#include "nn/nex/nex_StepSequenceJob.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_qVector.h"

namespace nn {
namespace nex {
class CriticalSection;
class HttpEventListener;

// The job of an HTTP request of HttpClient: it starts the connection, sends the chunked post data
// of the listener and passes the response to it. The names of the members and of the functions
// without a reference name are ours.
// RTTI N2nn3nex7JobHttpE @ 0x008CF654
// vtable 0x008FFB18 (vptr 0x008FFB20), offset_to_top 0, 14 entries
class JobHttp : public ::nn::nex::StepSequenceJob, public ::nn::nex::NonCopyable
{
public:
    // (inline in HttpClient::Start)
    JobHttp()
        : StepSequenceJob(static_cast<JobType>(0), DebugString()), m_CallContextId(0), m_pConnection(nullptr), m_URL(), m_Method(0), m_HasStatus(false), m_pListener(nullptr), m_ChunkSize(0)
    {
        m_PollTime = Core::s_IsSchedulerThreaded ? 10 : 5;
    }
    virtual ~JobHttp(); // 0x003D227C slot 0x00 | fefates:bytes-fuzzy
    // 0x003D224C slot 0x04 (deleting dtor)
    // chunkSize: the size of the parts of the post data and of the body
    virtual nn::nex::qResult Initialize(unsigned int callContextId, const nn::nex::String& url, nn::nex::HttpConnection::Method method, nn::nex::HttpEventListener* pListener,
                                        unsigned int chunkSize); // 0x003D1860 slot 0x34 | fefates:callseq
    void CompleteJob(const nn::nex::qResult& result); // 0x003D1930 | fefates:bytes-fuzzy [tier B]

    // the connection is made and started
    nn::nex::qResult StartConnection(); // 0x003D19E0 (name is ours)
    // sends the post data, then waits for the status and receives the body
    void StepTransfer(); // 0x003D1B68 (name is ours)
    // passes the received body to the listener
    nn::nex::qResult ReceiveBody(); // 0x003D1FC4 (name is ours)
    void StepStart(); // 0x003D217C (name is ours)

    // the connections (the lock is never set in this build)
    static u32 s_ConnectionCount;
    static CriticalSection* s_pConnectionLock;

    u32 m_CallContextId;            // 0x68
    HttpConnection* m_pConnection;  // 0x6C
    String m_URL;                   // 0x70
    u8 m_Method;                    // 0x78
    qVector<u8> m_Buffer;           // 0x7C
    bool m_HasStatus;               // 0x88
    HttpEventListener* m_pListener; // 0x8C
    u32 m_ChunkSize;                // 0x90
    s32 m_PollTime;                 // 0x94 (ms)
};
ASSERT_OFFSET(JobHttp, m_CallContextId, 0x68);
ASSERT_OFFSET(JobHttp, m_URL, 0x70);
ASSERT_OFFSET(JobHttp, m_Buffer, 0x7C);
ASSERT_SIZE(JobHttp, 0x98);
} // namespace nex
} // namespace nn
