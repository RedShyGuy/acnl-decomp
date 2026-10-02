#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex11InetAddressE @ 0x008CE018
// vtable 0x008FC2AC (vptr 0x008FC2B4), offset_to_top 0, 2 entries
class InetAddress : public ::nn::nex::RootObject
{
public:
    virtual void vf_0x00(); // 0x00357908 slot 0x00 | virtual slot, introduced by nn::nex::InetAddress
    virtual void vf_0x04(); // 0x003578F0 slot 0x04 | virtual slot, introduced by nn::nex::InetAddress
    void SetAddress(const wchar_t*); // 0x003576F8 | mk7dlp:callgraph [tier A]
    InetAddress(const nn::nex::InetAddress&); // 0x0035787C | mk7dlp:bytes [tier A]
    InetAddress(unsigned, unsigned short); // 0x0035789C | mk7dlp:bytes [tier A]
    InetAddress(); // 0x003578CC | mk7dlp:bytes [tier A]
    void operator =(const nn::nex::InetAddress&); // 0x0035790C | mk7dlp:bytes [tier A]
    void GetAddressStr() const; // 0x0072A128 | fefates:bytes [tier B]
    void ToStr(wchar_t*) const; // 0x0072A18C | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
