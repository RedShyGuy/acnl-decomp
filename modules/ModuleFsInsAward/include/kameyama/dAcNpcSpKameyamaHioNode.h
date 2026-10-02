#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"
#include "Ac/dAcNpcSp_AcNpcSpHioNode.h"

namespace kameyama {
// vtable +0x598C in ModuleFsInsAward.cro, offset_to_top 0, 1 entries
class AcNpcSpKameyamaHioNode : public ::AcNpcSp::AcNpcSpHioNode
{
public:
    AcNpcSpKameyamaHioNode(); // ctor address unknown
};
} // namespace kameyama
