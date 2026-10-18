#pragma once

#include "decomp.h"
#include "nn/nex/nex_DataStoreReqGetNotificationUrlInfo.h"
#include "nn/nex/nex_HttpConnection.h"
#include "nn/nex/nex_HttpEventListener.h"
#include "nn/nex/nex_NonCopyable.h"
#include "nn/nex/nex_ProtocolCallContext.h"
#include "nn/nex/nex_StepSequenceJob.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_qResult.h"
#include "nn/nex/nex_qVector.h"

namespace nn {
namespace nex {
class DataStoreFileServerClient;
class DataStoreLogicServerClientInterface;

// Checks the notification file of DataStore: without a URL (or with "invalidurl") the URL comes
// from the logic server (GetNotificationUrl), else the file is read from the file server. The file
// is "<data ID>,<principal ID>,<interval>"; a file of another principal is invalid (the caller gets
// "invalidurl"). The names of the members are ours.
// RTTI N2nn3nex29JobDataStoreCheckNotificationE @ 0x008CF0FC
// vtable 0x008FF168 (vptr 0x008FF170), offset_to_top 0, 17 entries
// vtable 0x008FF1B4 (vptr 0x008FF1BC), offset_to_top -104, 6 entries
class JobDataStoreCheckNotification : public ::nn::nex::StepSequenceJob, public ::nn::nex::NonCopyable, public ::nn::nex::HttpEventListener
{
public:
    JobDataStoreCheckNotification(); // 0x003C028C | fefates:bytes [tier B]
    virtual ~JobDataStoreCheckNotification(); // 0x003C038C slot 0x00 | fefates:bytes
    // 0x003C0374 slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
    virtual nn::nex::qResult Initialize(unsigned int callContextId, nn::nex::DataStoreLogicServerClientInterface* pClient, unsigned int threadPriority, int timeout,
                                        const nn::nex::String& url, nn::nex::String* pUrl, unsigned long long lastDataId, unsigned long long* pLastDataId,
                                        unsigned int interval, unsigned int* pInterval, unsigned int principalId, bool* pIsLogicServerError,
                                        bool getsUrl); // 0x003BF818 slot 0x34 | fefates:callseq
    virtual void PrepareRequest(nn::nex::HttpConnection* pConnection); // 0x003BFAC4 slot 0x38
    virtual void ProcessResponseHeader(nn::nex::HttpConnection* pConnection, int statusCode); // 0x003BFBFC slot 0x3C
    virtual bool ProcessResponse(const unsigned char* pData, unsigned int size); // 0x003BFB4C slot 0x40
    void CompleteJob(const nn::nex::qResult& result); // 0x003BF9D0 | fefates:bytes [tier B]
    void StepLogicServerGetNotificationUrl(); // 0x003BFC04 (name is ours)
    void StepFileServerCheckNotificationFile(); // 0x003BFD94 | fefates:bytes [tier B]
    void StepWaitingLogicServerGetNotificationUrl(); // 0x003BFECC | fefates:bytes [tier B]
    void StepWaitingFileServerCheckNotificationFile(); // 0x003C006C | fefates:bytes [tier B]

    u32 m_CallContextId;                            // 0x06C
    DataStoreLogicServerClientInterface* m_pClient; // 0x070
    DataStoreFileServerClient* m_pFileServerClient; // 0x074
    String* m_pUrl;                                 // 0x078 (of the caller)
    String m_Url;                                   // 0x07C
    u32 m_ThreadPriority;                           // 0x084 (of the HTTP connection)
    u32 m_Timeout;                                  // 0x088 (ms)
    ProtocolCallContext m_ProtocolCallContext;      // 0x090
    u64* m_pLastDataId;                             // 0x108 (of the caller)
    u64 m_LastDataId;                               // 0x110 (the largest)
    u32* m_pInterval;                               // 0x118 (of the caller)
    u32 m_Interval;                                 // 0x11C
    u32 m_PrincipalId;                                     // 0x120
    DataStoreReqGetNotificationUrlInfo m_UrlInfo;   // 0x124
    qVector<u8> m_Response;                         // 0x150
    s32 m_StatusCode;                               // 0x15C
    bool m_IsTooLarge;                              // 0x160 (the response: a new URL is needed)
    bool m_GetsUrl;                                 // 0x161
    bool* m_pIsLogicServerError;                    // 0x164 (of the caller)
    bool m_IsInvalid;                               // 0x168 (the file)
};
ASSERT_OFFSET(JobDataStoreCheckNotification, m_ProtocolCallContext, 0x90);
ASSERT_OFFSET(JobDataStoreCheckNotification, m_pLastDataId, 0x108);
ASSERT_OFFSET(JobDataStoreCheckNotification, m_UrlInfo, 0x124);
ASSERT_OFFSET(JobDataStoreCheckNotification, m_IsInvalid, 0x168);
ASSERT_SIZE(JobDataStoreCheckNotification, 0x170);
} // namespace nex
} // namespace nn
