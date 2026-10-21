#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLDeclarations.h"

namespace nn {
namespace nex {
// The DDL declarations of a part of nex (the declarations themselves are not in this build: Declare
// is empty).
// RTTI N2nn3nex31MessagingServiceDDLDeclarationsE @ 0x008CF23C
// vtable 0x008FF528 (vptr 0x008FF530), offset_to_top 0, 3 entries
class MessagingServiceDDLDeclarations : public ::nn::nex::DDLDeclarations
{
public:
    MessagingServiceDDLDeclarations(); // 0x003C6B0C
    virtual ~MessagingServiceDDLDeclarations(); // 0x003C6B38 slot 0x00
    // 0x003C6B28 slot 0x04 (deleting dtor)
    virtual void Declare(); // 0x003C6B08 slot 0x08
};
} // namespace nex
} // namespace nn
