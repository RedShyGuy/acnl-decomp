#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"
#include "Ac/dAcNpcSp_AcNpcSpHioNode.h"

namespace uomasa {
// vtable +0x5980 in ModuleFsInsAward.cro, offset_to_top 0, 1 entries
class AcNpcSpUomasaHioNode : public ::AcNpcSp::AcNpcSpHioNode
{
public:
    AcNpcSpUomasaHioNode(); // ctor address unknown
};
} // namespace uomasa
