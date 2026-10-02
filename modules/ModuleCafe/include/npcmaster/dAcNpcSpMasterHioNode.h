#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"
#include "Ac/dAcNpcSp_AcNpcSpHioNode.h"

namespace npcmaster {
// vtable +0xE004 in ModuleCafe.cro, offset_to_top 0, 1 entries
class AcNpcSpMasterHioNode : public ::AcNpcSp::AcNpcSpHioNode
{
public:
    AcNpcSpMasterHioNode(); // ctor address unknown
};
} // namespace npcmaster
