#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLDeclarations.h"

namespace nn {
namespace nex {
// The DDL declarations of a part of nex (the declarations themselves are not in this build: Declare
// is empty).
// RTTI N2nn3nex39AccountManagementServiceDDLDeclarationsE @ 0x008CF424
// vtable 0x008FF824 (vptr 0x008FF82C), offset_to_top 0, 3 entries
class AccountManagementServiceDDLDeclarations : public ::nn::nex::DDLDeclarations
{
public:
    AccountManagementServiceDDLDeclarations(); // 0x003CC498
    virtual ~AccountManagementServiceDDLDeclarations(); // 0x003CC4C4 slot 0x00
    // 0x003CC4B4 slot 0x04 (deleting dtor)
    virtual void Declare(); // 0x003CC494 slot 0x08
};
} // namespace nex
} // namespace nn
