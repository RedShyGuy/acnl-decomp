#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLDeclarations.h"

namespace nn {
namespace nex {
// The DDL declarations of a part of nex (the declarations themselves are not in this build: Declare
// is empty).
// RTTI N2nn3nex28HealthServiceDDLDeclarationsE @ 0x008CF090
// vtable 0x008FF00C (vptr 0x008FF014), offset_to_top 0, 3 entries
class HealthServiceDDLDeclarations : public ::nn::nex::DDLDeclarations
{
public:
    HealthServiceDDLDeclarations(); // 0x003BDA98
    virtual ~HealthServiceDDLDeclarations(); // 0x003BDAC4 slot 0x00
    // 0x003BDAB4 slot 0x04 (deleting dtor)
    virtual void Declare(); // 0x003BDA94 slot 0x08
};
} // namespace nex
} // namespace nn
