#pragma once

#include "decomp.h"
#include "nn/nex/nex_URLProbe.h"
#include "nn/nex/nex_qList.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex12URLProbeListE @ 0x008CE1E8
// vtable 0x008FC774 (vptr 0x008FC77C), offset_to_top 0, 2 entries
class URLProbeList : public ::nn::nex::qList<nn::nex::URLProbe>
{
public:
    URLProbeList(); // ctor address unknown
    virtual ~URLProbeList(); // 0x003617A0 slot 0x00 | fefates:bytes
    // 0x00361770 slot 0x04 | slot vf_0x04 of nn::nex::URLProbeList (deleting dtor)
    void FindProbe(const nn::nex::StationURL&); // 0x00361678 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
