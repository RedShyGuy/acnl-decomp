#pragma once

#include "decomp.h"
#include "nn/nex/nex_EndPointEventHandler.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex13StreamManagerE @ 0x008CE29C
// vtable 0x008FC9C0 (vptr 0x008FC9C8), offset_to_top 0, 6 entries
class StreamManager : public ::nn::nex::EndPointEventHandler
{
public:
    StreamManager(); // ctor candidate(s) 0x0036E6C4 (unverified)
    virtual ~StreamManager(); // 0x0036E988 slot 0x00 | fefates:bytes-fuzzy
    // 0x0036E978 slot 0x04 | fefates:callseq (deleting dtor)
    virtual void Receive(nn::nex::EndPoint*, nn::nex::Buffer*, unsigned char); // 0x0011C12F slot 0x08 | slot vf_0x00 of ChangeRentalBase
    virtual void FaultDetection(nn::nex::EndPoint*, unsigned int); // 0x0011C12F slot 0x0C | slot vf_0x00 of ChangeRentalBase
    virtual void PeerDisconnected(nn::nex::EndPoint*); // 0x0011C12F slot 0x10 | slot vf_0x00 of ChangeRentalBase
    virtual void SetCredentials(nn::nex::Credentials*); // 0x0036E670 slot 0x14 | slot vf_0x14 of nn::nex::StreamManager
    void Initialize(unsigned short, unsigned char, unsigned int); // 0x0036E478 | fefates:bytes-fuzzy [tier B]
    void AssociateSecureStream(nn::nex::ConnectionOrientedStream*, bool (*)(const nn::nex::UserContext&,nn::nex::Buffer*,nn::nex::Buffer*,nn::nex::EndPoint*)); // 0x0036E68C | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
