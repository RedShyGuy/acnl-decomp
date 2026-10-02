#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex12RootCaReaderE @ 0x008CE194
// vtable 0x008FC660 (vptr 0x008FC668), offset_to_top 0, 2 entries
class RootCaReader : public ::nn::nex::RootObject
{
public:
    RootCaReader(); // ctor address unknown
    virtual void vf_0x00(); // 0x00360378 slot 0x00 | virtual slot, introduced by nn::nex::RootCaReader
    virtual void vf_0x04(); // 0x00360374 slot 0x04 | virtual slot, introduced by nn::nex::RootCaReader
    void Read(nn::nex::qVector<unsigned char>*, const char**, unsigned int); // 0x0035FAA4 | fefates:bytes-fuzzy [tier B]
    void ReadImpl(nn::nex::qVector<unsigned char>*, const char**, unsigned int); // 0x0035FD78 | fefates:bytes-fuzzy [tier B]
};
} // namespace nex
} // namespace nn
