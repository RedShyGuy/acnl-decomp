#pragma once

#include "decomp.h"
#include "nn/nex/nex_RefCountedObject.h"

namespace nn {
namespace nex {
class Buffer;
class EndPoint;

// Receives the events of end points (StreamManager, SecureEndPoint; the names of the functions
// are those of StreamManager).
// RTTI N2nn3nex20EndPointEventHandlerE @ 0x008CE88C
class EndPointEventHandler : public ::nn::nex::RefCountedObject
{
public:
    // (inline)
    EndPointEventHandler() {}
    virtual void Receive(nn::nex::EndPoint* pEndPoint, nn::nex::Buffer* pData, unsigned char substreamId) = 0; // slot 0x08
    virtual void FaultDetection(nn::nex::EndPoint* pEndPoint, unsigned int error) = 0; // slot 0x0C
    virtual void PeerDisconnected(nn::nex::EndPoint* pEndPoint) = 0; // slot 0x10
};
} // namespace nex
} // namespace nn
