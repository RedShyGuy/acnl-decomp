#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLDeclarations.h"

namespace nn {
namespace nex {
// The DDL declarations of a part of nex (the declarations themselves are not in this build: Declare
// is empty).
// RTTI N2nn3nex33ProtocolFoundationDDLDeclarationsE @ 0x008CF310
// vtable 0x008FF6D0 (vptr 0x008FF6D8), offset_to_top 0, 3 entries
class ProtocolFoundationDDLDeclarations : public ::nn::nex::DDLDeclarations
{
public:
    ProtocolFoundationDDLDeclarations(); // 0x003CB164
    virtual ~ProtocolFoundationDDLDeclarations(); // 0x003CB190 slot 0x00
    // 0x003CB180 slot 0x04 (deleting dtor)
    virtual void Declare(); // 0x003CB160 slot 0x08
};
} // namespace nex
} // namespace nn
