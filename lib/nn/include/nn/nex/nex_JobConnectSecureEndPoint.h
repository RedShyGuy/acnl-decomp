#pragma once

#include "decomp.h"
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_ConnectionData.h"
#include "nn/nex/nex_EndPoint.h"
#include "nn/nex/nex_Job.h"
#include "nn/nex/nex_ProtocolCallContext.h"
#include "nn/nex/nex_StationURL.h"
#include "nn/nex/nex_Time.h"
#include "nn/nex/nex_UserContext.h"
#include "nn/nex/nex_qList.h"
#include "nn/nex/nex_qResult.h"

namespace nn {
namespace nex {
class BitStream;
class Buffer;
class JobConnectEndPoint;
class SecureEndPoint;
class Ticket;

// The job of SecureEndPoint::_Connect: the ticket for the principal of the URL and its connection
// data (the URLs of the station) from the secure connection server, then the connection of the end
// point below with the ticket and a random number (the reply has the number + 1). The names of the
// members and of the functions without a reference name are ours.
// RTTI N2nn3nex24JobConnectSecureEndPointE @ 0x008CEDA4
// vtable 0x008FE7B4 (vptr 0x008FE7BC), offset_to_top 0, 12 entries
class JobConnectSecureEndPoint : public ::nn::nex::Job
{
public:
    // the steps of Execute
    enum Step : u8
    {
        STEP_PARSE_URL = 0,
        STEP_REQUEST_CONNECTION_DATA = 1,
        STEP_CHECK_CONNECTION_DATA = 2,
        STEP_CONNECT = 3,
        STEP_PROCESS_CONNECT_RESULT = 4,
        STEP_DONE = 5
    };

    // pConnectData: not used (the job sends the ticket); the context of the callback is the
    // JobConnectEndPoint that waits (the job is its sub job)
    JobConnectSecureEndPoint(nn::nex::SecureEndPoint* pEndPoint, const nn::nex::StationURL* pURL, nn::nex::Buffer* pConnectData, nn::nex::Buffer* pReplyData,
                             nn::nex::EndPoint::CompletionCallback pCallback, const nn::nex::UserContext& context, unsigned int timeout); // 0x003B35FC | mk7dlp:callseq [tier A]
    virtual ~JobConnectSecureEndPoint(); // 0x003B3788 slot 0x00
    // 0x003B3778 slot 0x04 (deleting dtor)
    virtual void Execute(); // 0x003B32A8 slot 0x0C | fefates:callseq
    virtual void CancelJob(); // 0x003B35E0 slot 0x24 | fefates:bytes

    // the connection data of the URLs as StationURLs with the connection ids
    void BuildListURLs(nn::nex::qList<nn::nex::StationURL>* pURLs); // 0x003B2A8C | mk7dlp:callseq [tier A]
    void PerformConnect(); // 0x003B2BAC | fefates:bytes-fuzzy [tier B]
    void ProcessConnectResult(); // 0x003B2D58 | fefates:bytes-fuzzy [tier B]
    void RequestConnectionData(); // 0x003B2EAC | fefates:bytes-fuzzy [tier B]
    // the request of the connection: the ticket data and the encrypted ids and random number
    void InitializeBufferRequest(); // 0x003B3070 | fefates:bytes [tier B]
    void ParseURL(); // 0x003B33A0 | mk7dlp:callseq [tier A]
    // the completion of the connection of the end point below (the context is the job)
    static void OnConnectCompleted(nn::nex::CallContext* pContext, const nn::nex::UserContext* pUserContext); // 0x003B31CC
    // the completion of a call of the job: it is ready again if it waits (armlink placed it far away)
    static void OnCallCompleted(nn::nex::CallContext* pContext, const nn::nex::UserContext* pUserContext); // 0x00176598

    u8 m_Step;                           // 0x03A (Step)
    ProtocolCallContext m_TicketContext; // 0x040
    Ticket* m_pTicket;                   // 0x0B8
    BitStream* m_pRequest;               // 0x0BC
    BitStream* m_pReply;                 // 0x0C0
    u32 m_Random;                        // 0x0C4
    ProtocolCallContext m_URLContext;    // 0x0C8 (of RequestConnectionData)
    CallContext m_ConnectContext;        // 0x140
    SecureEndPoint* m_pEndPoint;         // 0x1A0
    EndPoint* m_pConnectedEndPoint;      // 0x1A4 (the end point below)
    qList<ConnectionData> m_URLs;        // 0x1A8
    StationURL m_Unknown0x1C0;           // 0x1C0
    qResult m_Result;                    // 0x228 (of the connection)
    StationURL m_URL;                    // 0x234
    Buffer* m_pReplyData;                // 0x29C
    EndPoint::CompletionCallback m_pCallback; // 0x2A0
    UserContext m_Context;               // 0x2A4
    Time m_Deadline;                     // 0x2A8
    u32 m_PrincipalId;                   // 0x2B0
    u32 m_ConnectionId;                  // 0x2B4
    JobConnectEndPoint* m_pConnectJob;   // 0x2B8
};
ASSERT_OFFSET(JobConnectSecureEndPoint, m_TicketContext, 0x40);
ASSERT_OFFSET(JobConnectSecureEndPoint, m_URLContext, 0xC8);
ASSERT_OFFSET(JobConnectSecureEndPoint, m_URLs, 0x1A8);
ASSERT_OFFSET(JobConnectSecureEndPoint, m_Result, 0x228);
ASSERT_OFFSET(JobConnectSecureEndPoint, m_Deadline, 0x2A8);
ASSERT_SIZE(JobConnectSecureEndPoint, 0x2C0);
} // namespace nex
} // namespace nn
