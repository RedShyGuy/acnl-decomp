#pragma once

#include "decomp.h"
#include "nn/nex/nex_EndPoint.h"
#include "nn/nex/nex_qResult.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex13PRUDPEndPointE @ 0x008CE248
// vtable 0x008FC828 (vptr 0x008FC830), offset_to_top 0, 37 entries
class PRUDPEndPoint : public ::nn::nex::EndPoint
{
public:
    PRUDPEndPoint(); // ctor candidate(s) 0x003682D4 (unverified)
    virtual ~PRUDPEndPoint(); // 0x003689C8 slot 0x00 | slot vf_0x00 of nn::nex::EndPoint
    // 0x00368998 slot 0x04 | slot vf_0x04 of nn::nex::EndPoint (deleting dtor)
    virtual void vf_0x08(); // 0x003636AC slot 0x08 | virtual slot, introduced by nn::nex::EndPoint
    virtual void vf_0x0C(); // 0x0072A65C slot 0x0C | virtual slot, introduced by nn::nex::EndPoint
    virtual void vf_0x10(); // 0x0072A8E0 slot 0x10 | virtual slot, introduced by nn::nex::EndPoint
    virtual void vf_0x14(); // 0x0072A8F0 slot 0x14 | virtual slot, introduced by nn::nex::EndPoint
    virtual void vf_0x18(); // 0x0072A8B0 slot 0x18 | virtual slot, introduced by nn::nex::EndPoint
    virtual void IsNotConnected(); // 0x00362EB4 slot 0x1C | fefates:bytes
    virtual void IsConnecting(); // 0x00362C18 slot 0x20 | slot vf_0x20 of nn::nex::EndPoint
    virtual void IsDisconnecting(); // 0x00362FE0 slot 0x24 | fefates:bytes
    virtual void IsFaulty(); // 0x00367CF0 slot 0x28 | fefates:bytes
    virtual void IsConnected(); // 0x00362730 slot 0x2C | fefates:bytes
    virtual void PeerIsConnected(); // 0x00362FF4 slot 0x30 | slot vf_0x30 of nn::nex::EndPoint
    virtual void vf_0x34(); // 0x00363984 slot 0x34 | virtual slot, introduced by nn::nex::EndPoint
    virtual void SetKeepAliveTimeout(unsigned int); // 0x003646BC slot 0x3C | slot vf_0x3C of nn::nex::EndPoint
    virtual void SetMaxSilenceTime(unsigned int); // 0x003636F0 slot 0x40 | slot vf_0x40 of nn::nex::EndPoint
    virtual void GetKeepAliveTimeout(); // 0x00363A88 slot 0x44 | slot vf_0x44 of nn::nex::EndPoint
    virtual void GetMaxSilenceTime(); // 0x00363248 slot 0x48 | slot vf_0x48 of nn::nex::EndPoint
    virtual void SetPeerConnected(); // 0x00363200 slot 0x4C | slot vf_0x4C of nn::nex::EndPoint
    virtual void SetPeerDisconnected(); // 0x00364718 slot 0x50 | slot vf_0x50 of nn::nex::EndPoint
    virtual void GetConnectionState() const; // 0x0072A8C0 slot 0x54 | slot vf_0x54 of nn::nex::EndPoint
    virtual void SetConnectionState(nn::nex::EndPoint::_ConnectionState); // 0x00363998 slot 0x58 | fefates:bytes-fuzzy
    virtual void IsAnyPacketProcessed(); // 0x00364724 slot 0x60 | fefates:bytes
    virtual void GetSignaledFaultError() const; // 0x0072A8C8 slot 0x64 | slot vf_0x64 of nn::nex::EndPoint
    virtual void vf_0x68(); // 0x0072A98C slot 0x68 | virtual slot, introduced by nn::nex::EndPoint
    virtual void vf_0x6C(); // 0x0072A650 slot 0x6C | virtual slot, introduced by nn::nex::EndPoint
    virtual void _Connect(nn::nex::Buffer*, nn::nex::Buffer*, void (*)(nn::nex::EndPoint*,nn::nex::qResult,const nn::nex::UserContext*), const nn::nex::UserContext&, unsigned int); // 0x00367DA8 slot 0x70 | fefates:bytes-fuzzy
    virtual void _Disconnect(void (*)(nn::nex::EndPoint*,nn::nex::qResult,const nn::nex::UserContext*), const nn::nex::UserContext&, unsigned int); // 0x00362744 slot 0x74 | slot vf_0x74 of nn::nex::EndPoint
    virtual void _Send(nn::nex::Buffer*, unsigned int, bool, unsigned char, unsigned int); // 0x003670DC slot 0x78 | slot vf_0x78 of nn::nex::EndPoint
    virtual void GetSupportedFunctionsFlags() const; // 0x0072A8D0 slot 0x7C | slot vf_0x7C of nn::nex::EndPoint
    virtual void SignalFaultEvent(unsigned int, bool); // 0x0036320C slot 0x88 | fefates:bytes
    virtual void SetEncryptionKey(const nn::nex::Key&); // 0x0035D984 slot 0x8C | fefates:bytes
    virtual void GetEncryptionKey(); // 0x00362FFC slot 0x90 | slot vf_0x90 of nn::nex::EndPoint
    void CompleteIO(nn::nex::qResult); // 0x003620B0 | fefates:bytes [tier B]
    void SendPacket(nn::nex::PacketOut*); // 0x003621AC | fefates:bytes-fuzzy [tier B]
    void SendAggregateACK(); // 0x0036311C | fefates:bytes [tier B]
    void InitUnreliableSeq(nn::nex::PacketOut*); // 0x00363250 | fefates:bytes [tier B]
    void IsDuplicateReorderingPacket(nn::nex::PacketIn*); // 0x003656A8 | fefates:bytes [tier B]
    void Defrag(nn::nex::PacketIn*); // 0x00367718 | fefates:bytes [tier B]
    void StartPing(); // 0x00368204 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
