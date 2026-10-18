#pragma once

#include "decomp.h"
#include "nn/nex/nex_StreamManager.h"

namespace nn {
namespace nex {
class ClientProtocolRequestBroker;

// The StreamManager of a client: the received messages go to the client broker as jobs. The member
// names are ours.
// RTTI N2nn3nex19ClientStreamManagerE @ 0x008CE79C
// vtable 0x008FD640 (vptr 0x008FD648), offset_to_top 0, 6 entries
class ClientStreamManager : public ::nn::nex::StreamManager
{
public:
    // (inline in the creation of the back end)
    ClientStreamManager(nn::nex::Stream::Type type, nn::nex::RootTransport* pTransport) : StreamManager(type, pTransport), m_pCredentials(nullptr) {}
    virtual ~ClientStreamManager(); // 0x00392B04 slot 0x00
    // 0x00392A88 slot 0x04 | fefates:bytes (deleting dtor)
    virtual void Receive(nn::nex::EndPoint* pEndPoint, nn::nex::Buffer* pData, unsigned char substreamId); // 0x003D912C slot 0x08 | fefates:bytes
    virtual void FaultDetection(nn::nex::EndPoint* pEndPoint, unsigned int error); // 0x003929DC slot 0x0C
    virtual void PeerDisconnected(nn::nex::EndPoint* pEndPoint); // 0x00392A78 slot 0x10
    virtual void SetCredentials(nn::nex::Credentials* pCredentials); // 0x003929EC slot 0x14 | fefates:bytes

    Credentials* m_pCredentials; // 0x28
};
ASSERT_SIZE(ClientStreamManager, 0x2C);
} // namespace nex
} // namespace nn
