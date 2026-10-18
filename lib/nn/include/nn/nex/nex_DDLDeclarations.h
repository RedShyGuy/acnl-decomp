#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// The DDL declarations of a part of nex (per context in a PseudoGlobalVariable). The names of the
// members and of the functions other than the constructor are ours.
// RTTI N2nn3nex15DDLDeclarationsE @ 0x008CE484
// vtable 0x008FCE7C (vptr 0x008FCE84), offset_to_top 0, 3 entries
class DDLDeclarations : public ::nn::nex::RootObject
{
public:
    explicit DDLDeclarations(bool unknown0x10); // 0x00377620
    virtual ~DDLDeclarations(); // 0x0037764C slot 0x00
    // 0x00377644 slot 0x04 (deleting dtor)
    virtual void Declare() = 0; // slot 0x08

    u32 m_Unknown0x04;  // 0x04
    bool m_Unknown0x08; // 0x08
    u32 m_Unknown0x0C;  // 0x0C
    bool m_Unknown0x10; // 0x10
};
ASSERT_SIZE(DDLDeclarations, 0x14);
} // namespace nex
} // namespace nn
