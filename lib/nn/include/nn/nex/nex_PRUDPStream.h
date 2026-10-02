#pragma once

#include "decomp.h"
#include "nn/nex/nex_ConnectionOrientedStream.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex11PRUDPStreamE @ 0x008CE050
// vtable 0x008FC334 (vptr 0x008FC33C), offset_to_top 0, 34 entries
class PRUDPStream : public ::nn::nex::ConnectionOrientedStream
{
public:
    PRUDPStream(); // ctor candidate(s) 0x0035AC5C (unverified)
    virtual ~PRUDPStream(); // 0x0035AF8C slot 0x00 | slot vf_0x00 of nn::nex::Stream
    // 0x0035AF5C slot 0x04 | slot vf_0x04 of nn::nex::Stream (deleting dtor)
    virtual void ReceiveIncomingPacket(unsigned short, unsigned char, nn::nex::Packet*); // 0x00359CD0 slot 0x08 | slot vf_0x08 of nn::nex::Stream
    virtual void DoWork(); // 0x0035AADC slot 0x0C | slot vf_0x0C of nn::nex::Stream
    virtual void IsDuplicateReorderingPacket(nn::nex::Packet*); // 0x0035A45C slot 0x10 | fefates:bytes-fuzzy
    virtual void StopListen(); // 0x003589C0 slot 0x18 | slot vf_0x18 of nn::nex::ConnectionOrientedStream
    virtual void ResponsibleForURL(const nn::nex::StationURL*); // 0x00359688 slot 0x1C | fefates:bytes
    virtual void GetURLType(); // 0x0035896C slot 0x20 | mk7dlp:bytes
    virtual void OpenEndPoint(const nn::nex::StationURL*); // 0x00358AC0 slot 0x24 | slot vf_0x24 of nn::nex::ConnectionOrientedStream
    virtual void OpenEndPoint(unsigned); // 0x00358BAC slot 0x28 | slot vf_0x28 of nn::nex::ConnectionOrientedStream
    virtual void vf_0x2C(); // 0x00358B3C slot 0x2C | virtual slot, introduced by nn::nex::ConnectionOrientedStream
    virtual void vf_0x30(); // 0x003595C8 slot 0x30 | virtual slot, introduced by nn::nex::ConnectionOrientedStream
    virtual void vf_0x34(); // 0x00359678 slot 0x34 | virtual slot, introduced by nn::nex::ConnectionOrientedStream
    virtual void vf_0x38(); // 0x00359518 slot 0x38 | virtual slot, introduced by nn::nex::ConnectionOrientedStream
    virtual void vf_0x3C(); // 0x0072A298 slot 0x3C | virtual slot, introduced by nn::nex::ConnectionOrientedStream
    virtual void CloseEndPoint(nn::nex::EndPoint*); // 0x00358BD4 slot 0x40 | slot vf_0x40 of nn::nex::ConnectionOrientedStream
    virtual void vf_0x44(); // 0x00359488 slot 0x44 | virtual slot, introduced by nn::nex::ConnectionOrientedStream
    virtual void vf_0x48(); // 0x003596B0 slot 0x48 | virtual slot, introduced by nn::nex::ConnectionOrientedStream
    virtual void vf_0x4C(); // 0x00358980 slot 0x4C | virtual slot, introduced by nn::nex::ConnectionOrientedStream
    virtual void vf_0x50(); // 0x0035AC04 slot 0x50 | virtual slot, introduced by nn::nex::ConnectionOrientedStream
    virtual void vf_0x58(); // 0x00358C60 slot 0x58 | virtual slot, introduced by nn::nex::ConnectionOrientedStream
    virtual void vf_0x5C(); // 0x0035A7D4 slot 0x5C | virtual slot, introduced by nn::nex::ConnectionOrientedStream
    virtual void vf_0x68(); // 0x0035AC20 slot 0x68 | virtual slot, introduced by nn::nex::ConnectionOrientedStream
    virtual void vf_0x70(); // 0x0035A3F8 slot 0x70 | virtual slot, introduced by nn::nex::ConnectionOrientedStream
    virtual void vf_0x74(); // 0x0035A698 slot 0x74 | virtual slot, introduced by nn::nex::ConnectionOrientedStream
    virtual void vf_0x78(); // 0x0035A360 slot 0x78 | virtual slot, introduced by nn::nex::ConnectionOrientedStream
    virtual void vf_0x7C(); // 0x0035A4CC slot 0x7C | virtual slot, introduced by nn::nex::ConnectionOrientedStream
    virtual void vf_0x80(); // 0x003591F8 slot 0x80 | virtual slot, introduced by nn::nex::PRUDPStream
    virtual void vf_0x84(); // 0x00359A28 slot 0x84 | virtual slot, introduced by nn::nex::PRUDPStream
    void VersionNegotiation(nn::nex::Packet*, nn::nex::Packet*); // 0x003596D4 | fefates:bytes-fuzzy [tier B]
    void ReleaseEndPointImpl(nn::nex::PRUDPEndPoint*); // 0x003597F8 | fefates:bytes-fuzzy [tier B]
    void Send(unsigned short, unsigned char, nn::nex::PacketOut*, nn::nex::Key*); // 0x0035A918 | fefates:bytes-fuzzy [tier B]
};
} // namespace nex
} // namespace nn
