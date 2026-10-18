#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLDeclarations.h"

namespace nn {
namespace nex {
// The DDL declarations of a part of nex (the declarations themselves are not in this build: Declare
// is empty).
// RTTI N2nn3nex21STLExtDDLDeclarationsE @ 0x008CEB44
// vtable 0x008FDEFC (vptr 0x008FDF04), offset_to_top 0, 3 entries
class STLExtDDLDeclarations : public ::nn::nex::DDLDeclarations
{
public:
    STLExtDDLDeclarations(); // 0x0039A968
    virtual ~STLExtDDLDeclarations(); // 0x0039A994 slot 0x00
    // 0x0039A990 slot 0x04 (deleting dtor)
    virtual void Declare(); // 0x0039A964 slot 0x08
};
} // namespace nex
} // namespace nn
