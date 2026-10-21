#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLDeclarations.h"

namespace nn {
namespace nex {
// The DDL declarations of a part of nex (the declarations themselves are not in this build: Declare
// is empty).
// RTTI N2nn3nex25OnlineCoreDDLDeclarationsE @ 0x008CEEDC
// vtable 0x008FEB28 (vptr 0x008FEB30), offset_to_top 0, 3 entries
class OnlineCoreDDLDeclarations : public ::nn::nex::DDLDeclarations
{
public:
    OnlineCoreDDLDeclarations(); // 0x003B8524
    virtual ~OnlineCoreDDLDeclarations(); // 0x003B8550 slot 0x00
    // 0x003B854C slot 0x04 (deleting dtor)
    virtual void Declare(); // 0x003B8520 slot 0x08
};
} // namespace nex
} // namespace nn
