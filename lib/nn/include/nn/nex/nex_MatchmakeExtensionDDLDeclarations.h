#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLDeclarations.h"

namespace nn {
namespace nex {
// The DDL declarations of a part of nex (the declarations themselves are not in this build: Declare
// is empty).
// RTTI N2nn3nex33MatchmakeExtensionDDLDeclarationsE @ 0x008CF304
// vtable 0x008FF6BC (vptr 0x008FF6C4), offset_to_top 0, 3 entries
class MatchmakeExtensionDDLDeclarations : public ::nn::nex::DDLDeclarations
{
public:
    MatchmakeExtensionDDLDeclarations(); // 0x003CB130
    virtual ~MatchmakeExtensionDDLDeclarations(); // 0x003CB15C slot 0x00
    // 0x003CB14C slot 0x04 (deleting dtor)
    virtual void Declare(); // 0x003CB12C slot 0x08
};
} // namespace nex
} // namespace nn
