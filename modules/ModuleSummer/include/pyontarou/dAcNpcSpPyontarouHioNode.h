#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"
#include "Ac/dAcNpcSp_AcNpcSpHioNode.h"

namespace pyontarou {
// vtable +0x4DC0 in ModuleSummer.cro, offset_to_top 0, 1 entries
class AcNpcSpPyontarouHioNode : public ::AcNpcSp::AcNpcSpHioNode
{
public:
    AcNpcSpPyontarouHioNode(); // ctor address unknown
};
} // namespace pyontarou
