#pragma once

#include "decomp.h"
#include "nn/nex/nex_Buffer.h"
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_DataStoreKeyValue.h"
#include "nn/nex/nex_HttpClient.h"
#include "nn/nex/nex_HttpConnection.h"
#include "nn/nex/nex_HttpEventListener.h"
#include "nn/nex/nex_NonCopyable.h"
#include "nn/nex/nex_RetrieveGameAuthTokenClient.h"
#include "nn/nex/nex_StepSequenceJob.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_qVector.h"

namespace nn {
namespace nex {
class ClientProtocol;
class DataStorePostHttpEventListener;

// A DataStore call over HTTP (HPP): the call message is posted with the token and the key of
// RetrieveGameAuthToken and two HMAC signatures in the header fields; the response is the result
// message of the call. Not used in this game: the linker kept only the steps (from the table of
// the steps) and CompleteJob, there is no constructor, vtable or RTTI. The layout is from the
// steps (like JobDataStoreFileServer); the names of the members are ours.
class JobDataStoreHpp : public ::nn::nex::StepSequenceJob, public ::nn::nex::NonCopyable, public ::nn::nex::HttpEventListener
{
public:
    void StepWaitingForHttp(); // 0x00378700 | fefates:bytes [tier B]
    // the result of the call in the response (and its results to the protocol)
    nn::nex::qResult ProcessFinalResponse(const nn::nex::qVector<unsigned char>& response); // 0x00378918 | fefates:bytes [tier B]
    void PrepareRequestHeaders(); // 0x00378A88 | fefates:bytes-fuzzy [tier B]
    void StepRetrieveGameAuthToken(); // 0x003792DC | fefates:bytes [tier B]
    void StepWaitingRetrieveGameAuthToken(); // 0x00379400 | fefates:bytes [tier B]
    void StepHttp(); // 0x003794BC | fefates:bytes [tier B]
    void CompleteJob(const nn::nex::qResult& result); // 0x003CC5B0 | fefates:bytes [tier B]

    u32 m_CallContextId;                            // 0x06C
    String m_URL;                                   // 0x070
    u8 m_Method;                                    // 0x078
    Buffer m_Message;                               // 0x07C (the call)
    ClientProtocol* m_pProtocol;                    // 0x09C (of the call)
    CallContext m_CallContext;                      // 0x0A0
    HttpClient m_HttpClient;                        // 0x100
    u32 m_RequestCount;                             // 0x104
    s32 m_StatusCode;                               // 0x108
    qVector<u8> m_Response;                         // 0x10C
    bool m_IsTooLarge;                              // 0x118 (the response)
    String m_Location;                              // 0x11C (of a redirect)
    u32 m_ThreadPriority;                           // 0x124
    u32 m_PrincipalId;                              // 0x128
    String m_Password;                              // 0x12C
    String m_AccessKey;                             // 0x134 (the key of signature1)
    u32 m_Timeout;                                  // 0x13C (ms)
    RetrieveGameAuthTokenClient m_AuthTokenClient;  // 0x140
    String m_Token;                                 // 0x144
    String m_Key;                                   // 0x14C (the key of signature2)
    DataStorePostHttpEventListener* m_pHttpListener; // 0x154
    qVector<DataStoreKeyValue> m_HeaderFields;      // 0x158
    qVector<DataStoreKeyValue> m_FormFields;        // 0x164
    qVector<u8> m_RootCa;                           // 0x170
    u8 m_PriorityType;                              // 0x17C (0: no header field)
};
ASSERT_OFFSET(JobDataStoreHpp, m_CallContextId, 0x6C);
ASSERT_OFFSET(JobDataStoreHpp, m_CallContext, 0xA0);
ASSERT_OFFSET(JobDataStoreHpp, m_HttpClient, 0x100);
ASSERT_OFFSET(JobDataStoreHpp, m_ThreadPriority, 0x124);
ASSERT_OFFSET(JobDataStoreHpp, m_AuthTokenClient, 0x140);
ASSERT_OFFSET(JobDataStoreHpp, m_HeaderFields, 0x158);
ASSERT_OFFSET(JobDataStoreHpp, m_PriorityType, 0x17C);
} // namespace nex
} // namespace nn
