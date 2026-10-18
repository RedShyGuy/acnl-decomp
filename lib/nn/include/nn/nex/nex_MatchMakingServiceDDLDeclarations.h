#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLDeclarations.h"

namespace nn {
namespace nex {
// The DDL declarations of a part of nex (the declarations themselves are not in this build: Declare
// is empty).
// RTTI N2nn3nex33MatchMakingServiceDDLDeclarationsE @ 0x008CF2F8
// vtable 0x008FF6A8 (vptr 0x008FF6B0), offset_to_top 0, 3 entries
class MatchMakingServiceDDLDeclarations : public ::nn::nex::DDLDeclarations
{
public:
    MatchMakingServiceDDLDeclarations(); // 0x003CB0FC
    virtual ~MatchMakingServiceDDLDeclarations(); // 0x003CB128 slot 0x00
    // 0x003CB118 slot 0x04 (deleting dtor)
    virtual void Declare(); // 0x003CB0F8 slot 0x08
};
} // namespace nex
} // namespace nn
