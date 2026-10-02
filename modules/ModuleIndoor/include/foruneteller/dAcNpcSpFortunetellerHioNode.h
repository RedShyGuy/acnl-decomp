#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"
#include "Ac/dAcNpcSp_AcNpcSpHioNode.h"

namespace foruneteller {
// vtable +0x68060 in ModuleIndoor.cro, offset_to_top 0, 1 entries
class AcNpcSpFortunetellerHioNode : public ::AcNpcSp::AcNpcSpHioNode
{
public:
    AcNpcSpFortunetellerHioNode(); // ctor address unknown
};
} // namespace foruneteller
