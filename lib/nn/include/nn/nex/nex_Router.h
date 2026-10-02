#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex6RouterE @ 0x008CF5B8
// vtable 0x008FFA2C (vptr 0x008FFA34), offset_to_top 0, 2 entries
class Router : public ::nn::nex::RootObject
{
public:
    virtual ~Router(); // 0x003D0988 slot 0x00 | fefates:bytes-fuzzy
    // 0x003D0958 slot 0x04 | slot vf_0x04 of nn::nex::Router (deleting dtor)
    void ShouldRoute(const nn::nex::InetAddress*); // 0x003D05FC | fefates:bytes-fuzzy [tier B]
    Router(); // 0x003D07C8 | fefates:bytes-fuzzy [tier B]
};
} // namespace nex
} // namespace nn
