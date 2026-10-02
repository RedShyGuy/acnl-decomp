#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex6SocketE @ 0x008CF5C4
// vtable 0x008FFA3C (vptr 0x008FFA44), offset_to_top 0, 6 entries
class Socket : public ::nn::nex::RootObject
{
public:
    Socket(); // ctor address unknown
    virtual ~Socket(); // 0x003D0FF8 slot 0x00 | slot vf_0x00 of nn::nex::Socket
    // 0x003D0F78 slot 0x04 | slot vf_0x04 of nn::nex::Socket (deleting dtor)
    virtual void vf_0x08(); // 0x003D0C08 slot 0x08 | fefates:callseq-callee
    virtual void vf_0x0C(); // 0x003D0CDC slot 0x0C | virtual slot, introduced by nn::nex::Socket
    virtual void vf_0x10(); // 0x003D0B98 slot 0x10 | virtual slot, introduced by nn::nex::Socket
    virtual void vf_0x14(); // 0x003D0DE4 slot 0x14 | virtual slot, introduced by nn::nex::Socket
    void Open(bool); // 0x003D0C8C | mk7dlp:bytes [tier A]
    void Close(); // 0x003D0E58 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
