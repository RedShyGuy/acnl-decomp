#pragma once

#include "decomp.h"
#include "nn/nex/nex_PseudoSingleton.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex19ConnectivityManagerE @ 0x008CE7A8
// vtable 0x008FD660 (vptr 0x008FD668), offset_to_top 0, 2 entries
class ConnectivityManager : public ::nn::nex::PseudoSingleton
{
public:
    ConnectivityManager(); // ctor address unknown
    virtual ~ConnectivityManager(); // 0x00392CAC slot 0x00 | slot vf_0x00 of nn::nex::InstanceControl
    // 0x00392C7C slot 0x04 | slot vf_0x04 of nn::nex::InstanceControl (deleting dtor)
};
} // namespace nex
} // namespace nn
