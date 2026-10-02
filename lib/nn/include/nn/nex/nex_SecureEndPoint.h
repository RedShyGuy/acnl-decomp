#pragma once

#include "decomp.h"
#include "nn/nex/nex_EndPoint.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex14SecureEndPointE @ 0x008CE3C4
// vtable 0x008FCC68 (vptr 0x008FCC70), offset_to_top 0, 37 entries
class SecureEndPoint : public ::nn::nex::EndPoint
{
public:
    SecureEndPoint(); // ctor address unknown
    virtual ~SecureEndPoint(); // 0x00374220 slot 0x00 | slot vf_0x00 of nn::nex::EndPoint
    // 0x00374204 slot 0x04 | fefates:callseq (deleting dtor)
    virtual void vf_0x08(); // 0x00373D8C slot 0x08 | virtual slot, introduced by nn::nex::EndPoint
    virtual void vf_0x0C(); // 0x0072B2B8 slot 0x0C | virtual slot, introduced by nn::nex::EndPoint
    virtual void vf_0x10(); // 0x0072B320 slot 0x10 | virtual slot, introduced by nn::nex::EndPoint
    virtual void vf_0x14(); // 0x0072B328 slot 0x14 | virtual slot, introduced by nn::nex::EndPoint
    virtual void vf_0x18(); // 0x0072B2C0 slot 0x18 | virtual slot, introduced by nn::nex::EndPoint
    virtual void IsNotConnected(); // 0x00373CA8 slot 0x1C | slot vf_0x1C of nn::nex::EndPoint
    virtual void IsConnecting(); // 0x00373C8C slot 0x20 | fefates:bytes
    virtual void IsDisconnecting(); // 0x00373CC8 slot 0x24 | slot vf_0x24 of nn::nex::EndPoint
    virtual void IsFaulty(); // 0x00374010 slot 0x28 | fefates:bytes
    virtual void IsConnected(); // 0x00373B64 slot 0x2C | fefates:bytes
    virtual void PeerIsConnected(); // 0x00373CE4 slot 0x30 | fefates:bytes
    virtual void vf_0x34(); // 0x00373E70 slot 0x34 | virtual slot, introduced by nn::nex::EndPoint
    virtual void SetKeepAliveTimeout(unsigned int); // 0x00373EC8 slot 0x3C | fefates:bytes
    virtual void SetMaxSilenceTime(unsigned int); // 0x00373D90 slot 0x40 | fefates:bytes
    virtual void GetKeepAliveTimeout(); // 0x00373EAC slot 0x44 | slot vf_0x44 of nn::nex::EndPoint
    virtual void GetMaxSilenceTime(); // 0x00373D70 slot 0x48 | slot vf_0x48 of nn::nex::EndPoint
    virtual void SetPeerConnected(); // 0x00373D38 slot 0x4C | slot vf_0x4C of nn::nex::EndPoint
    virtual void SetPeerDisconnected(); // 0x00373EE4 slot 0x50 | slot vf_0x50 of nn::nex::EndPoint
    virtual void GetConnectionState() const; // 0x0072B2C8 slot 0x54 | slot vf_0x54 of nn::nex::EndPoint
    virtual void SetConnectionState(nn::nex::EndPoint::_ConnectionState); // 0x00373E90 slot 0x58 | fefates:bytes
    virtual void RegisterEventHandler(nn::nex::EndPointEventHandler*); // 0x00373F1C slot 0x5C | mk7dlp:callseq
    virtual void IsAnyPacketProcessed(); // 0x00373F00 slot 0x60 | fefates:bytes
    virtual void GetSignaledFaultError() const; // 0x0072B2E4 slot 0x64 | fefates:bytes
    virtual void vf_0x68(); // 0x0072B32C slot 0x68 | virtual slot, introduced by nn::nex::EndPoint
    virtual void vf_0x6C(); // 0x0072B29C slot 0x6C | virtual slot, introduced by nn::nex::EndPoint
    virtual void _Connect(nn::nex::Buffer*, nn::nex::Buffer*, void (*)(nn::nex::EndPoint*,nn::nex::qResult,const nn::nex::UserContext*), const nn::nex::UserContext&, unsigned int); // 0x0037402C slot 0x70 | fefates:bytes
    virtual void _Disconnect(void (*)(nn::nex::EndPoint*,nn::nex::qResult,const nn::nex::UserContext*), const nn::nex::UserContext&, unsigned int); // 0x00373B80 slot 0x74 | fefates:bytes
    virtual void _Send(nn::nex::Buffer*, unsigned int, bool, unsigned char, unsigned int); // 0x00373F90 slot 0x78 | slot vf_0x78 of nn::nex::EndPoint
    virtual void GetSupportedFunctionsFlags() const; // 0x0072B300 slot 0x7C | fefates:bytes
    virtual void SignalFaultEvent(unsigned int, bool); // 0x00373D54 slot 0x88 | fefates:bytes
    virtual void SetEncryptionKey(const nn::nex::Key&); // 0x00373D1C slot 0x8C | fefates:bytes
    virtual void GetEncryptionKey(); // 0x00373D00 slot 0x90 | slot vf_0x90 of nn::nex::EndPoint
};
} // namespace nex
} // namespace nn
