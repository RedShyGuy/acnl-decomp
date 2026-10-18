#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLDeclarations.h"

namespace nn {
namespace nex {
// The DDL declarations of a part of nex (the declarations themselves are not in this build: Declare
// is empty).
// RTTI N2nn3nex22UtilityDDLDeclarationsE @ 0x008CEC80
// vtable 0x008FE1E8 (vptr 0x008FE1F0), offset_to_top 0, 3 entries
class UtilityDDLDeclarations : public ::nn::nex::DDLDeclarations
{
public:
    UtilityDDLDeclarations(); // 0x0039ED2C
    virtual ~UtilityDDLDeclarations(); // 0x00377648 slot 0x00
    // 0x0039ED48 slot 0x04 (deleting dtor)
    virtual void Declare(); // 0x0039ED28 slot 0x08
};
} // namespace nex
} // namespace nn
