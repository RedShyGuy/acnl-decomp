#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLDeclarations.h"

namespace nn {
namespace nex {
// The DDL declarations of a part of nex (the declarations themselves are not in this build: Declare
// is empty).
// RTTI N2nn3nex39AuthenticationFoundationDDLDeclarationsE @ 0x008CF430
// vtable 0x008FF838 (vptr 0x008FF840), offset_to_top 0, 3 entries
class AuthenticationFoundationDDLDeclarations : public ::nn::nex::DDLDeclarations
{
public:
    AuthenticationFoundationDDLDeclarations(); // 0x003CC4CC
    virtual ~AuthenticationFoundationDDLDeclarations(); // 0x003CC4F8 slot 0x00
    // 0x003CC4E8 slot 0x04 (deleting dtor)
    virtual void Declare(); // 0x003CC4C8 slot 0x08
};
} // namespace nex
} // namespace nn
