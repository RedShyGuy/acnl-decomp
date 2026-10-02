#pragma once

#include "decomp.h"
#include "nn/nex/nex_EndPointInfoInterface.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex8EndPointE @ 0x008CF6B0
// vtable 0x008FFBB0 (vptr 0x008FFBB8), offset_to_top 0, 37 entries
class EndPoint : public ::nn::nex::EndPointInfoInterface, public ::nn::nex::RootObject
{
public:
    struct _ConnectionState { u32 _unknown; }; // TODO: real type unknown (placeholder)
    EndPoint(); // ctor candidate(s) 0x003D4DAC (unverified)
    virtual ~EndPoint(); // 0x003D4E24 slot 0x00 | mk7dlp:bytes-fuzzy
    // 0x003D4DF4 slot 0x04 | slot vf_0x04 of nn::nex::EndPoint (deleting dtor)
    virtual void vf_0x08(); // 0x0011C12F slot 0x08 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x0C(); // 0x0011C12F slot 0x0C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x10(); // 0x0011C12F slot 0x10 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x14(); // 0x0011C12F slot 0x14 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x18(); // 0x0011C12F slot 0x18 | slot vf_0x00 of ChangeRentalBase
    virtual void IsNotConnected(); // 0x0011C12F slot 0x1C | slot vf_0x00 of ChangeRentalBase
    virtual void IsConnecting(); // 0x0011C12F slot 0x20 | slot vf_0x00 of ChangeRentalBase
    virtual void IsDisconnecting(); // 0x0011C12F slot 0x24 | slot vf_0x00 of ChangeRentalBase
    virtual void IsFaulty(); // 0x0011C12F slot 0x28 | slot vf_0x00 of ChangeRentalBase
    virtual void IsConnected(); // 0x0011C12F slot 0x2C | slot vf_0x00 of ChangeRentalBase
    virtual void PeerIsConnected(); // 0x0011C12F slot 0x30 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x34(); // 0x0011C12F slot 0x34 | slot vf_0x00 of ChangeRentalBase
    virtual void DisableKeepAlive(); // 0x003D4D28 slot 0x38 | slot vf_0x38 of nn::nex::EndPoint
    virtual void SetKeepAliveTimeout(unsigned int); // 0x0011C12F slot 0x3C | slot vf_0x00 of ChangeRentalBase
    virtual void SetMaxSilenceTime(unsigned int); // 0x0011C12F slot 0x40 | slot vf_0x00 of ChangeRentalBase
    virtual void GetKeepAliveTimeout(); // 0x0011C12F slot 0x44 | slot vf_0x00 of ChangeRentalBase
    virtual void GetMaxSilenceTime(); // 0x0011C12F slot 0x48 | slot vf_0x00 of ChangeRentalBase
    virtual void SetPeerConnected(); // 0x0011C12F slot 0x4C | slot vf_0x00 of ChangeRentalBase
    virtual void SetPeerDisconnected(); // 0x0011C12F slot 0x50 | slot vf_0x00 of ChangeRentalBase
    virtual void GetConnectionState() const; // 0x0011C12F slot 0x54 | slot vf_0x00 of ChangeRentalBase
    virtual void SetConnectionState(nn::nex::EndPoint::_ConnectionState); // 0x0011C12F slot 0x58 | slot vf_0x00 of ChangeRentalBase
    virtual void RegisterEventHandler(nn::nex::EndPointEventHandler*); // 0x003D4D48 slot 0x5C | mk7dlp:callseq-callee
    virtual void IsAnyPacketProcessed(); // 0x0011C12F slot 0x60 | slot vf_0x00 of ChangeRentalBase
    virtual void GetSignaledFaultError() const; // 0x0011C12F slot 0x64 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x68(); // 0x0011C12F slot 0x68 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x6C(); // 0x0011C12F slot 0x6C | slot vf_0x00 of ChangeRentalBase
    virtual void _Connect(nn::nex::Buffer*, nn::nex::Buffer*, void (*)(nn::nex::EndPoint*,nn::nex::qResult,const nn::nex::UserContext*), const nn::nex::UserContext&, unsigned int); // 0x0011C12F slot 0x70 | slot vf_0x00 of ChangeRentalBase
    virtual void _Disconnect(void (*)(nn::nex::EndPoint*,nn::nex::qResult,const nn::nex::UserContext*), const nn::nex::UserContext&, unsigned int); // 0x0011C12F slot 0x74 | slot vf_0x00 of ChangeRentalBase
    virtual void _Send(nn::nex::Buffer*, unsigned int, bool, unsigned char, unsigned int); // 0x0011C12F slot 0x78 | slot vf_0x00 of ChangeRentalBase
    virtual void GetSupportedFunctionsFlags() const; // 0x0011C12F slot 0x7C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x80(); // 0x003D4C68 slot 0x80 | virtual slot, introduced by nn::nex::EndPoint
    virtual void vf_0x84(); // 0x003D4D3C slot 0x84 | virtual slot, introduced by nn::nex::EndPoint
    virtual void SignalFaultEvent(unsigned int, bool); // 0x003D4D38 slot 0x88 | slot vf_0x88 of nn::nex::EndPoint
    virtual void SetEncryptionKey(const nn::nex::Key&); // 0x0011C12F slot 0x8C | slot vf_0x00 of ChangeRentalBase
    virtual void GetEncryptionKey(); // 0x0011C12F slot 0x90 | slot vf_0x00 of ChangeRentalBase
    void SetPrincipalID(unsigned); // 0x003D4C70 | mk7dlp:bytes-fuzzy [tier A]
    void SetConnectionID(unsigned); // 0x003D4CCC | mk7dlp:bytes-fuzzy [tier A]
    void Open(); // 0x003D4D84 | mk7dlp:bytes [tier A]
    EndPoint(nn::nex::ConnectionOrientedStream*, const nn::nex::StationURL*); // 0x003D4DAC | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
