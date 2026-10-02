#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex8URLProbeE @ 0x008CF72C
// vtable 0x008FFCE4 (vptr 0x008FFCEC), offset_to_top 0, 2 entries
class URLProbe : public ::nn::nex::RootObject
{
public:
    URLProbe(); // ctor candidate(s) 0x0038DE30, 0x0038F7EC, 0x003D6110, 0x00830350 (unverified)
    virtual void vf_0x00(); // 0x003D61F0 slot 0x00 | virtual slot, introduced by nn::nex::URLProbe
    virtual void vf_0x04(); // 0x003D61BC slot 0x04 | virtual slot, introduced by nn::nex::URLProbe
    URLProbe(const nn::nex::StationURL&, int, int, unsigned); // 0x003D6110 | mk7dlp:bytes-fuzzy [tier A]
};
} // namespace nex
} // namespace nn
