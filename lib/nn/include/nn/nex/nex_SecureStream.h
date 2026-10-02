#pragma once

#include "decomp.h"
#include "nn/nex/nex_ConnectionOrientedStream.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex12SecureStreamE @ 0x008CE1A0
// vtable 0x008FC670 (vptr 0x008FC678), offset_to_top 0, 40 entries
class SecureStream : public ::nn::nex::ConnectionOrientedStream
{
public:
    virtual ~SecureStream(); // 0x00360EF4 slot 0x00 | slot vf_0x00 of nn::nex::Stream
    // 0x00360E98 slot 0x04 | mk7dlp:bytes (deleting dtor)
    virtual void ReceiveIncomingPacket(unsigned short, unsigned char, nn::nex::Packet*); // 0x00360AA4 slot 0x08 | mk7dlp:callseq
    virtual void DoWork(); // 0x00360C34 slot 0x0C | slot vf_0x0C of nn::nex::Stream
    virtual void ResponsibleForURL(const nn::nex::StationURL*); // 0x00360A78 slot 0x1C | mk7dlp:bytes
    virtual void GetURLType(); // 0x003608D8 slot 0x20 | mk7dlp:bytes
    virtual void OpenEndPoint(const nn::nex::StationURL*); // 0x00360934 slot 0x24 | slot vf_0x24 of nn::nex::ConnectionOrientedStream
    virtual void OpenEndPoint(unsigned); // 0x00360940 slot 0x28 | slot vf_0x28 of nn::nex::ConnectionOrientedStream
    virtual void vf_0x2C(); // 0x003B1C6C slot 0x2C | virtual slot, introduced by nn::nex::ConnectionOrientedStream
    virtual void vf_0x30(); // 0x00360A58 slot 0x30 | virtual slot, introduced by nn::nex::ConnectionOrientedStream
    virtual void vf_0x34(); // 0x00360A68 slot 0x34 | virtual slot, introduced by nn::nex::ConnectionOrientedStream
    virtual void vf_0x38(); // 0x00360A48 slot 0x38 | virtual slot, introduced by nn::nex::ConnectionOrientedStream
    virtual void vf_0x3C(); // 0x0072A5F4 slot 0x3C | virtual slot, introduced by nn::nex::ConnectionOrientedStream
    virtual void CloseEndPoint(nn::nex::EndPoint*); // 0x00360948 slot 0x40 | fefates:bytes
    virtual void vf_0x44(); // 0x00360A38 slot 0x44 | virtual slot, introduced by nn::nex::ConnectionOrientedStream
    virtual void vf_0x48(); // 0x00360A94 slot 0x48 | virtual slot, introduced by nn::nex::ConnectionOrientedStream
    virtual void vf_0x5C(); // 0x00360BFC slot 0x5C | virtual slot, introduced by nn::nex::ConnectionOrientedStream
    virtual void vf_0x68(); // 0x00360CA8 slot 0x68 | virtual slot, introduced by nn::nex::ConnectionOrientedStream
    virtual void CreateEndPoint(const nn::nex::StationURL*); // 0x003609F0 slot 0x80 | fefates:bytes
    virtual void vf_0x84(); // 0x003609C4 slot 0x84 | virtual slot, introduced by nn::nex::SecureStream
    virtual void vf_0x88(); // 0x00360908 slot 0x88 | virtual slot, introduced by nn::nex::SecureStream
    virtual void vf_0x8C(); // 0x003608F0 slot 0x8C | virtual slot, introduced by nn::nex::SecureStream
    virtual void vf_0x90(); // 0x00360C38 slot 0x90 | virtual slot, introduced by nn::nex::SecureStream
    virtual void vf_0x94(); // 0x00360C18 slot 0x94 | virtual slot, introduced by nn::nex::SecureStream
    virtual void vf_0x98(); // 0x00360CB0 slot 0x98 | virtual slot, introduced by nn::nex::SecureStream
    virtual void vf_0x9C(); // 0x00360C8C slot 0x9C | virtual slot, introduced by nn::nex::SecureStream
    void FilterIncomingConnection(nn::nex::Buffer*, nn::nex::Buffer*, nn::nex::EndPoint*); // 0x00360AB4 | fefates:bytes [tier B]
    SecureStream(); // 0x00360CCC | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
