#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
class Buffer;
class ConnectionOrientedStream;
class EndPoint;
class StationURL;

// Receives the events of the connection oriented streams (ConnectionManager; the slot names are
// those of ConnectionManager, the others are ours).
// RTTI N2nn3nex21TransportEventHandlerE @ 0x008CEB5C
class TransportEventHandler : public ::nn::nex::RootObject
{
public:
    // (inline)
    TransportEventHandler() {}
    virtual ~TransportEventHandler() {}
    // data without a connection
    virtual void Receive(nn::nex::ConnectionOrientedStream* pStream, nn::nex::Buffer* pData, const nn::nex::StationURL* pURL) = 0; // slot 0x08
    virtual void vf_0x0C() = 0; // slot 0x0C
    virtual bool SwitchToRoutingConnection(nn::nex::EndPoint* pEndPoint) = 0; // slot 0x10
    virtual void vf_0x14() = 0; // slot 0x14
    // a connection request on the end point: true if it is accepted
    virtual bool ConnectionRequest(nn::nex::ConnectionOrientedStream* pStream, const nn::nex::StationURL* pURL, nn::nex::Buffer* pData, nn::nex::EndPoint* pEndPoint) = 0; // slot 0x18
    virtual bool FilterConnectionRequest(nn::nex::ConnectionOrientedStream* pStream, const nn::nex::StationURL* pURL, nn::nex::Buffer* pData, nn::nex::EndPoint* pEndPoint) = 0; // slot 0x1C
};
} // namespace nex
} // namespace nn
