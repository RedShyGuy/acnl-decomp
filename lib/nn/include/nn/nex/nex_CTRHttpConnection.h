#pragma once

#include "decomp.h"
#include "nn/nex/nex_HttpConnection.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex17CTRHttpConnectionE @ 0x008CE668
// vtable 0x008FD32C (vptr 0x008FD334), offset_to_top 0, 23 entries
class CTRHttpConnection : public ::nn::nex::HttpConnection
{
public:
    CTRHttpConnection(); // ctor candidate(s) 0x003858AC (unverified)
    virtual ~CTRHttpConnection(); // 0x003859E4 slot 0x00 | fefates:callseq
    // 0x003859B4 slot 0x04 | slot vf_0x04 of nn::nex::HttpConnection (deleting dtor)
    virtual void vf_0x08(); // 0x0038483C slot 0x08 | virtual slot, introduced by nn::nex::HttpConnection
    virtual void vf_0x0C(); // 0x00384DFC slot 0x0C | virtual slot, introduced by nn::nex::HttpConnection
    virtual void vf_0x10(); // 0x00384964 slot 0x10 | virtual slot, introduced by nn::nex::HttpConnection
    virtual void vf_0x14(); // 0x00384E58 slot 0x14 | virtual slot, introduced by nn::nex::HttpConnection
    virtual void vf_0x1C(); // 0x0038502C slot 0x1C | virtual slot, introduced by nn::nex::HttpConnection
    virtual void vf_0x20(); // 0x0072C0E8 slot 0x20 | virtual slot, introduced by nn::nex::HttpConnection
    virtual void vf_0x24(); // 0x003849C0 slot 0x24 | mk7dlp:callseq
    virtual void vf_0x28(); // 0x00385804 slot 0x28 | virtual slot, introduced by nn::nex::HttpConnection
    virtual void vf_0x2C(); // 0x0072BD78 slot 0x2C | virtual slot, introduced by nn::nex::HttpConnection
    virtual void vf_0x30(); // 0x003856B4 slot 0x30 | virtual slot, introduced by nn::nex::HttpConnection
    virtual void vf_0x34(); // 0x00384EA4 slot 0x34 | virtual slot, introduced by nn::nex::HttpConnection
    virtual void vf_0x38(); // 0x0072BE0C slot 0x38 | virtual slot, introduced by nn::nex::HttpConnection
    virtual void vf_0x3C(); // 0x0072C1BC slot 0x3C | virtual slot, introduced by nn::nex::HttpConnection
    virtual void vf_0x40(); // 0x0072C12C slot 0x40 | virtual slot, introduced by nn::nex::HttpConnection
    virtual void vf_0x44(); // 0x0072BD04 slot 0x44 | virtual slot, introduced by nn::nex::HttpConnection
    virtual void vf_0x48(); // 0x00384900 slot 0x48 | virtual slot, introduced by nn::nex::HttpConnection
    virtual void vf_0x4C(); // 0x0072BE78 slot 0x4C | virtual slot, introduced by nn::nex::HttpConnection
    virtual void vf_0x50(); // 0x00385070 slot 0x50 | virtual slot, introduced by nn::nex::HttpConnection
    virtual void AddRootCa(const unsigned char*, int); // 0x00385894 slot 0x54 | mk7dlp:bytes
    virtual void vf_0x58(); // 0x003850B4 slot 0x58 | virtual slot, introduced by nn::nex::HttpConnection
};
} // namespace nex
} // namespace nn
