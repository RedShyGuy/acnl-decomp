#pragma once

#include "decomp.h"
#include "nn/nex/nex_Stream.h"
#include "nn/nex/nex_TransportEventHandler.h"
#include "nn/nex/nex_qResult.h"
#include "nn/nex/nex_qVector.h"

namespace nn {
namespace nex {
class EndPoint;
class EndPointEventHandler;
class RootTransport;
class StationURL;

// A stream with connections (EndPoints) to stations; it listens on a port of its transport. The
// defaults of the base do nothing. The names of the members and of the functions without a
// reference name are ours.
// RTTI N2nn3nex24ConnectionOrientedStreamE @ 0x008CED3C
// vtable 0x008FE68C (vptr 0x008FE694), offset_to_top 0, 32 entries
class ConnectionOrientedStream : public ::nn::nex::Stream
{
public:
    ConnectionOrientedStream(nn::nex::Stream::Type type, nn::nex::RootTransport* pTransport); // 0x003B1D7C | fefates:bytes-fuzzy [tier B]
    virtual ~ConnectionOrientedStream(); // 0x003B1E9C slot 0x00 | mk7dlp:callgraph
    // 0x003B1E1C slot 0x04 | fefates:bytes (deleting dtor)
    // listens on the port (0: the next free one) of the transport; pPort: the port
    virtual bool StartListen(unsigned char port, unsigned char* pPort); // 0x003B1C2C slot 0x14 | slot vf_0x14 of nn::nex::ConnectionOrientedStream
    virtual bool StopListen(); // 0x003B1BF8 slot 0x18 | slot vf_0x18 of nn::nex::ConnectionOrientedStream
    virtual bool ResponsibleForURL(const nn::nex::StationURL* pURL) = 0; // slot 0x1C
    virtual const wchar_t* GetURLType() = 0; // slot 0x20
    virtual nn::nex::EndPoint* OpenEndPoint(const nn::nex::StationURL* pURL) = 0; // slot 0x24
    virtual nn::nex::EndPoint* OpenEndPoint(unsigned connectionId) = 0; // slot 0x28
    // one more reference to an open end point
    virtual bool OpenEndPoint(nn::nex::EndPoint* pEndPoint); // 0x003B1C70 slot 0x2C
    virtual nn::nex::EndPoint* FindEndPointByPrincipalID(unsigned principalId); // 0x003B1CA0 slot 0x30
    virtual u32 GetNumberOfConnectionIDs(); // 0x003B1CA8 slot 0x34
    virtual nn::nex::EndPoint* FindEndPointByConnectionID(unsigned connectionId); // 0x003B1C98 slot 0x38
    virtual nn::nex::qVector<u32> GetConnectionIDs(); // 0x0072D534 slot 0x3C
    virtual void CloseEndPoint(nn::nex::EndPoint* pEndPoint) = 0; // slot 0x40
    virtual void Lock(); // 0x003B1C94 slot 0x44
    virtual void Unlock(); // 0x003B1CB0 slot 0x48
    virtual nn::nex::qResult Initialize(); // 0x003B1BDC slot 0x4C
    virtual nn::nex::qResult Terminate(); // 0x003B1D60 slot 0x50
    virtual nn::nex::qResult RegisterEventHandler(nn::nex::TransportEventHandler* pHandler); // 0x003B1CD0 slot 0x54
    // sends the payload without a connection: to the broadcast address of the local network (or
    // this station) at the well-known port; to the URL (the names are ours)
    virtual nn::nex::qResult SendBroadcast(const nn::nex::StationURL* pURL, nn::nex::Buffer* pPayload); // 0x003B1C78 slot 0x58
    virtual nn::nex::qResult SendTo(const nn::nex::StationURL* pURL, nn::nex::Buffer* pPayload); // 0x003B1D40 slot 0x5C
    virtual nn::nex::qResult vf_0x60(); // 0x003B1CB4 slot 0x60
    virtual nn::nex::qResult vf_0x64(); // 0x003B1D0C slot 0x64
    // the packets of the type are reliable
    virtual bool IsReliableType(unsigned int type) = 0; // slot 0x68
    virtual void vf_0x6C(); // 0x003B1D5C slot 0x6C
    virtual void RegisterPrincipalID(unsigned principalId, nn::nex::EndPoint* pEndPoint); // 0x003B1D2C slot 0x70
    virtual bool UnregisterPrincipalID(unsigned principalId, nn::nex::EndPoint* pEndPoint); // 0x003B1D38 slot 0x74
    virtual void RegisterConnectionID(unsigned connectionId, nn::nex::EndPoint* pEndPoint); // 0x003B1D28 slot 0x78
    virtual bool UnregisterConnectionID(unsigned connectionId, nn::nex::EndPoint* pEndPoint); // 0x003B1D30 slot 0x7C

protected:
    // (for the derived streams that are not decompiled yet)
    ConnectionOrientedStream() : m_pEventHandler(nullptr), m_pTransport(nullptr), m_Port(0) {}

public:
    TransportEventHandler* m_pEventHandler; // 0x08
    RootTransport* m_pTransport;           // 0x0C
    u8 m_Port;                             // 0x10 (0: not listening)
};
ASSERT_SIZE(ConnectionOrientedStream, 0x14);
} // namespace nex
} // namespace nn
