#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLDeclarations.h"

namespace nn {
namespace nex {
// The DDL declarations of a part of nex (the declarations themselves are not in this build: Declare
// is empty).
// RTTI N2nn3nex38SecureConnectionServiceDDLDeclarationsE @ 0x008CF418
// vtable 0x008FF810 (vptr 0x008FF818), offset_to_top 0, 3 entries
class SecureConnectionServiceDDLDeclarations : public ::nn::nex::DDLDeclarations
{
public:
    SecureConnectionServiceDDLDeclarations(); // 0x003CC464
    virtual ~SecureConnectionServiceDDLDeclarations(); // 0x003CC490 slot 0x00
    // 0x003CC480 slot 0x04 (deleting dtor)
    virtual void Declare(); // 0x003CC460 slot 0x08
};
} // namespace nex
} // namespace nn
