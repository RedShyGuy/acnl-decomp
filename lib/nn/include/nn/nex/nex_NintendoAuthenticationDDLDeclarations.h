#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLDeclarations.h"

namespace nn {
namespace nex {
// The DDL declarations of a part of nex (the declarations themselves are not in this build: Declare
// is empty).
// RTTI N2nn3nex37NintendoAuthenticationDDLDeclarationsE @ 0x008CF3F4
// vtable 0x008FF7DC (vptr 0x008FF7E4), offset_to_top 0, 3 entries
class NintendoAuthenticationDDLDeclarations : public ::nn::nex::DDLDeclarations
{
public:
    NintendoAuthenticationDDLDeclarations(); // 0x003CC36C
    virtual ~NintendoAuthenticationDDLDeclarations(); // 0x003CC398 slot 0x00
    // 0x003CC388 slot 0x04 (deleting dtor)
    virtual void Declare(); // 0x003CC368 slot 0x08
};
} // namespace nex
} // namespace nn
