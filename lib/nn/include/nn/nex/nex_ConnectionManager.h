#pragma once

#include "decomp.h"
#include "nn/nex/nex_ComponentState.h"
#include "nn/nex/nex_InetAddress.h"
#include "nn/nex/nex_Job.h"
#include "nn/nex/nex_StreamTable.h"
#include "nn/nex/nex_TransportEventHandler.h"
#include "nn/nex/nex_UserContext.h"
#include "nn/nex/nex_qList.h"
#include "nn/nex/nex_qResult.h"

namespace nn {
namespace nex {
class Buffer;
class CallContext;
class ConnectionOrientedStream;
class EndPoint;
class EndPointEventHandler;
class JobConnectEndPoint;
class StationURL;

// Connects and disconnects the end points of the streams of a StreamManager and accepts the
// connection requests. The names of the members and of the functions without a reference name are
// ours.
// RTTI N2nn3nex17ConnectionManagerE @ 0x008CE68C
// vtable 0x008FD390 (vptr 0x008FD398), offset_to_top 0, 8 entries
class ConnectionManager : public ::nn::nex::TransportEventHandler
{
public:
    // decides on a connection request (pData: the connection data, pReply: the reply data)
    typedef bool (*ConnectionFilter)(const UserContext& context, Buffer* pData, Buffer* pReply, EndPoint* pEndPoint);

    // isRoutingEnabled: the router of the transport is enabled
    ConnectionManager(nn::nex::EndPointEventHandler* pHandler, nn::nex::Job::JobType jobType, bool unknown, bool isRoutingEnabled); // 0x0038678C | fefates:bytes-fuzzy [tier B]
    virtual ~ConnectionManager(); // 0x00386A98 slot 0x00
    // 0x00386A2C (deleting dtor)
    virtual void Receive(nn::nex::ConnectionOrientedStream* pStream, nn::nex::Buffer* pData, const nn::nex::StationURL* pURL); // 0x0039AE44 slot 0x08
    virtual void vf_0x0C(); // 0x0039AE38 slot 0x0C
    virtual bool SwitchToRoutingConnection(nn::nex::EndPoint* pEndPoint); // 0x0039AE3C slot 0x10
    virtual void vf_0x14(); // 0x0039AE34 slot 0x14
    // a connection request on the end point of the stream: true if it is accepted
    virtual bool ConnectionRequest(nn::nex::ConnectionOrientedStream* pStream, const nn::nex::StationURL* pURL, nn::nex::Buffer* pData, nn::nex::EndPoint* pEndPoint); // 0x00386518 slot 0x18
    virtual bool FilterConnectionRequest(nn::nex::ConnectionOrientedStream* pStream, const nn::nex::StationURL* pURL, nn::nex::Buffer* pData, nn::nex::EndPoint* pEndPoint); // 0x003865A4 slot 0x1C | mk7dlp:callseq

    bool Connect(nn::nex::CallContext* pContext, const nn::nex::StationURL& url, nn::nex::EndPoint** ppEndPoint, unsigned int timeout, nn::nex::JobConnectEndPoint** ppJob); // 0x00386630 | fefates:bytes [tier B]
    bool ConnectImpl(nn::nex::CallContext* pContext, nn::nex::Buffer* pConnectData, nn::nex::Buffer* pReplyData, const nn::nex::qList<nn::nex::StationURL>& urls, nn::nex::EndPoint** ppEndPoint,
                     unsigned int timeout, bool unknown, nn::nex::JobConnectEndPoint** ppJob); // 0x00386334
    bool Disconnect(nn::nex::CallContext* pContext, nn::nex::EndPoint* pEndPoint); // 0x0038625C
    void AttachStream(nn::nex::ConnectionOrientedStream* pStream, bool unknown); // 0x00386430
    void RegisterConnectionFilter(ConnectionFilter pFilter, nn::nex::UserContext context); // 0x00386620
    bool IsTerminated() const; // 0x0072C1E4 | fefates:bytes [tier B]
    // the end of the disconnection: the call context gets the result, the end point is closed
    // (armlink placed it far from the others)
    static void DisconnectCompletion(nn::nex::EndPoint* pEndPoint, nn::nex::qResult result, const nn::nex::UserContext* pContext); // 0x005290F4 (name is ours)

    // the timeout of a disconnection (ms)
    static u32 s_DisconnectTimeout;

    u32 m_Context;                     // 0x04 (of the PseudoSingleton)
    ComponentState m_State;            // 0x08
    InetAddress m_Address;             // 0x28
    ConnectionFilter m_pFilter;        // 0x48
    UserContext m_FilterContext;       // 0x4C
    bool m_IsAccepting;                // 0x50 (connection requests are accepted)
    EndPointEventHandler* m_pHandler;  // 0x54
    StreamTable m_StreamTable;         // 0x58
    bool m_Unknown0x74;                // 0x74
    bool m_IsRoutingEnabled;           // 0x75
    u8 m_JobType;                      // 0x76
};
ASSERT_OFFSET(ConnectionManager, m_Address, 0x28);
ASSERT_OFFSET(ConnectionManager, m_StreamTable, 0x58);
ASSERT_SIZE(ConnectionManager, 0x78);
} // namespace nex
} // namespace nn
