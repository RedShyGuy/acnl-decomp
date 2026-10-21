#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLDeclarations.h"

namespace nn {
namespace nex {
// The DDL declarations of a part of nex (the declarations themselves are not in this build: Declare
// is empty).
// RTTI N2nn3nex24DataStoreDDLDeclarationsE @ 0x008CED5C
// vtable 0x008FE724 (vptr 0x008FE72C), offset_to_top 0, 3 entries
class DataStoreDDLDeclarations : public ::nn::nex::DDLDeclarations
{
public:
    DataStoreDDLDeclarations(); // 0x003B22F0
    virtual ~DataStoreDDLDeclarations(); // 0x003B231C slot 0x00
    // 0x003B230C slot 0x04 (deleting dtor)
    virtual void Declare(); // 0x003B22EC slot 0x08
};
} // namespace nex
} // namespace nn
