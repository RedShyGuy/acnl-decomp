#pragma once

#include "decomp.h"
#include "nn/nex/nex_RefCountedObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex15GlobalVariablesE @ 0x008CE49C
// vtable 0x008FCE90 (vptr 0x008FCE98), offset_to_top 0, 2 entries
class GlobalVariables : public ::nn::nex::RefCountedObject
{
public:
    GlobalVariables(); // ctor address unknown
    virtual ~GlobalVariables(); // 0x00377B84 slot 0x00 | fefates:callseq
    // 0x00377B54 slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
};
} // namespace nex
} // namespace nn
