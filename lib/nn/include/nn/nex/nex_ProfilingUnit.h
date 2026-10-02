#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_Time.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex13ProfilingUnitE @ 0x008CE254
// vtable 0x008FC8C4 (vptr 0x008FC8CC), offset_to_top 0, 2 entries
class ProfilingUnit : public ::nn::nex::RootObject
{
public:
    ProfilingUnit(); // ctor address unknown
    virtual void vf_0x00(); // 0x00369448 slot 0x00 | fefates:callseq
    virtual ~ProfilingUnit(); // 0x00369418 slot 0x04 | slot vf_0x04 of nn::nex::ProfilingUnit
    void Log(); // 0x00369094 | mk7dlp:bytes-fuzzy [tier A]
    void Reset(nn::nex::Time); // 0x00369264 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
