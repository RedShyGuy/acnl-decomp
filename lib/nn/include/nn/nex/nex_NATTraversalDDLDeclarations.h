#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLDeclarations.h"

namespace nn {
namespace nex {
// The DDL declarations of a part of nex (the declarations themselves are not in this build: Declare
// is empty).
// RTTI N2nn3nex27NATTraversalDDLDeclarationsE @ 0x008CF048
// vtable 0x008FEFA8 (vptr 0x008FEFB0), offset_to_top 0, 3 entries
class NATTraversalDDLDeclarations : public ::nn::nex::DDLDeclarations
{
public:
    NATTraversalDDLDeclarations(); // 0x003BD2BC
    virtual ~NATTraversalDDLDeclarations(); // 0x003BD2E8 slot 0x00
    // 0x003BD2D8 slot 0x04 (deleting dtor)
    virtual void Declare(); // 0x003BD2B8 slot 0x08
};
} // namespace nex
} // namespace nn
