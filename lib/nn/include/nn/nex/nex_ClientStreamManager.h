#pragma once

#include "decomp.h"
#include "nn/nex/nex_StreamManager.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex19ClientStreamManagerE @ 0x008CE79C
// vtable 0x008FD640 (vptr 0x008FD648), offset_to_top 0, 6 entries
class ClientStreamManager : public ::nn::nex::StreamManager
{
public:
    ClientStreamManager(); // ctor address unknown
    virtual ~ClientStreamManager(); // 0x00392B04 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x00392A88 slot 0x04 | fefates:bytes (deleting dtor)
    virtual void Receive(nn::nex::EndPoint*, nn::nex::Buffer*, unsigned char); // 0x003D912C slot 0x08 | fefates:bytes
    virtual void FaultDetection(nn::nex::EndPoint*, unsigned int); // 0x003929DC slot 0x0C | slot vf_0x0C of nn::nex::StreamManager
    virtual void PeerDisconnected(nn::nex::EndPoint*); // 0x00392A78 slot 0x10 | slot vf_0x10 of nn::nex::StreamManager
    virtual void SetCredentials(nn::nex::Credentials*); // 0x003929EC slot 0x14 | fefates:bytes
};
} // namespace nex
} // namespace nn
