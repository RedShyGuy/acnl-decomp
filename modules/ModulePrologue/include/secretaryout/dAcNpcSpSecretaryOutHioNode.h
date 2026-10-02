#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"
#include "Ac/dAcNpcSp_AcNpcSpHioNode.h"

namespace secretaryout {
// vtable +0x8B8C in ModulePrologue.cro, offset_to_top 0, 1 entries
class AcNpcSpSecretaryOutHioNode : public ::AcNpcSp::AcNpcSpHioNode
{
public:
    AcNpcSpSecretaryOutHioNode(); // ctor address unknown
};
} // namespace secretaryout
